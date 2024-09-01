#include "includes.h"
#include "CPosition.h"
#include "CCell.h"
#include "CExpression.h"
#include "CSpreadsheet.h"

class CExpressionBuilder;

                                    CSpreadsheet::CSpreadsheet          (  ) 
                                                                         : builder(*this)
{
    /**
     * @brief Default constructor
     * 
     */
}

                                    CSpreadsheet::CSpreadsheet          ( const CSpreadsheet & other ) 
                                                                         : builder(*this) 
{
    /**
     * @brief Copy constructor
     * 
     */
    for ( const auto & [ key, cell ] : other.table ) {
        table[key] = CCell(cell);
    }
}

CSpreadsheet &                      CSpreadsheet::operator=             ( const CSpreadsheet & other ) {
    /**
     * @brief Copy assignment operator
     * 
     */
    if ( this != & other ) {
        table.clear();
        for ( const auto & [ key, cell ] : other.table ) {
            table[key] = CCell(cell);
        }
    }
    return *this;
}

bool                                CSpreadsheet::load                  ( std::istream & is ) 
{
    /**
     * @brief Load an input stream istream & is that contains on every line a single cell
     *        Parameters of a cell are separated by ';'
     */
    std::string record = "";
    std::vector<std::string> records;
    size_t originalChecksum = 0;
    size_t computedChecksum = 0;
    size_t row = 0;
    size_t col = 0;
    size_t offRow = 0;
    size_t offCol = 0;    
    std::istringstream iss;
    std::string contents;
    std::string unescaped;
    char delimiter;
    table.clear();

    // std::cout << "DISPLAYING RAW LOADED FILE: " << std::endl;
    while ( std::getline(is, record) ) 
    {
        if ( record.find( "CHECKSUM" ) == 0 ) {
            std::string checksum = record.substr(8);
            originalChecksum = static_cast<size_t>(std::stoul(checksum));
        } else {
            // std::cout << "< | " << record << std::endl;
            records.push_back(record);
            for ( char c : record ) computedChecksum += c;
            computedChecksum += '\n';
        }
    }
    // std::cout << "LOADED CHECKSUM: " << computedChecksum << std::endl;
    if ( originalChecksum != computedChecksum ) return false;

    for ( const auto & record : records ) {
        iss.str(record);
        iss.clear();
        if ( ! ( iss >> row >> delimiter && delimiter == ';'    && iss >> col >> delimiter && delimiter == ';'
              && iss >> offRow >> delimiter && delimiter == ';' && iss >> offCol >> delimiter && delimiter == ';' ) ) {
           // std::cout << "Failed parsing the record" << std::endl;
            return false;
        }


        if ( ! std::getline ( iss, contents ) ) {
            setCell( CPosition ( row, col ), "" );
            CCell & cref = getCell ( CPosition ( row, col ) );
            cref.offsetRow = offRow;
            cref.offsetCol = offCol;
            getValue ( CPosition ( row, col ) )  ;
            cref.updateAST( offRow, offCol );
            continue;
        }
        
        // std::cout << "Escaped contents : " << contents << std::endl;
        unescaped.clear();
        for ( size_t i = 0; i < contents.length(); i++ ) {
            if ( contents [ i ] == '\\' && ( i + 1 ) < contents.length() ) {
                switch (contents[i + 1]) {
                    case 'n'  : unescaped += '\n';   i++; break;
                    case '\\' : unescaped += '\\';   i++; break;
                    default   : unescaped += contents[i]; break;
                }
            } else {
                unescaped += contents[i];
            }
        }
        // std::cout << "Setting cell with contents : " << unescaped << std::endl;
        setCell( CPosition ( row, col ), unescaped );
        CCell & cref = getCell ( CPosition ( row, col ) );
        cref.offsetRow = offRow;
        cref.offsetCol = offCol;
        getValue ( CPosition ( row, col ) )  ;
        cref.updateAST( offRow, offCol );
    }
    return true;
}

bool                                CSpreadsheet::save                  ( std::ostream & os ) const 
{
    /**
     * @brief Save a spreadsheet into ostream
     *        Every line represents a cell, cell attributes are separated by ';'
     *        Last line contains a CHECKSUM of all the characters saved
     */
    size_t checksum = 0;
    std::stringstream ss;
    std::string escaped;

    // std::cout << "DISPLAYING RAW SAVED FILE: " << std::endl;
    for ( const auto & [ key, cell ] : table ) {
        const auto & [ row, col ] = key;
        escaped.clear();
        for ( char c : cell.getContents() ){
            if (c == '\\') {
                escaped += "\\\\";
            } else if (c == '\n') {
                escaped += "\\n";
            } else {
                escaped += c;
            }
        }
        ss.str("");
        ss << row << ';' << col << ';' << cell.offsetRow << ';' << cell.offsetCol << ';' << escaped << std::endl;
        std::string record = ss.str();
        for ( char c : record ) checksum += c;
        // std::cout << "> | " << record;
        os << record;
    }
    os << "CHECKSUM" << checksum << std::endl;
    // std::cout << "SAVED CHECKSUM: " << checksum << std::endl;
    return true;
}

bool                                CSpreadsheet::setCell               ( CPosition pos, std::string contents ) 
{
    /**
     * @brief Set cell with coordinates CPosition pos with std::string contents.
     *        Return true if succesful, otherwise false
     */
    std::pair<size_t,size_t> key = std::make_pair(pos.getRow(), pos.getCol());
    this->table[key] = CCell ( contents );

    if ( this->table[key].getIsFormula() ) {
        CValue val = this->table[key].getValue();
        try {
            // std::cout << "Trying to parse the expression and asigning AST" << std::endl;
            parseExpression( std::get<std::string>(val), this->builder );
        } catch ( std::invalid_argument & ) {
            return false;
        } 
        this->builder.assignAST( this->table[key] );
        this->table[key].setIsParsed(true);
    }
    return true;
}

CValue                              CSpreadsheet::getValue              ( CPosition pos ) 
{
    /**
     * @brief Return value from a cell at position CPosition pos
     *        If a cell doesnt exist, return CValue()
     */
    std::pair<size_t,size_t> key = std::make_pair(pos.getRow(), pos.getCol());

    auto it = this->table.find(key);
    if ( it == this->table.end() ) return CValue();
    // if ( std::holds_alternative<std::monostate>(this->table[key].getValue()) ) 
    if ( this->table[key].state == EVALUATING ) {
        this->table[key].state = EVALUATED;
        return std::monostate();
    }
    this->table[key].state = EVALUATING;

    // handle copy assignment
    if ( this->table[key].getIsFormula() && !this->table[key].getIsParsed() ) {
        CValue val = this->table[key].getValue();
        try {
            parseExpression( std::get<std::string>(val), this->builder );
        } catch ( std::invalid_argument & ) {
            this->table[key].state = EVALUATED;
            return std::monostate();
        } 
        this->builder.assignAST( this->table[key] );
        this->table[key].setIsParsed(true);
    }

    if ( ! this->table[key].getIsFormula() ) {
        this->table[key].state = EVALUATED;
        CValue val = this->table[key].getValue();
        return val;
    } else {
        // the cell contains a formula and we will have fun now
        if ( this->table[key].getIsParsed() ) {
            CValue toReturn = this->table[key].getValue();
            this->table[key].state = EVALUATED;
            return toReturn;
        } else {
            this->table[key].state = EVALUATED;
            return std::monostate();
        }
    }
}

void                                CSpreadsheet::copyRect              ( CPosition dst, CPosition src, int w, int h ) 
{
    /**
     * @brief Copy a block of size (w*h) from the src position to dst
     *        dst and src are left upper cells of the coppied block
     */
    if ( w < 1 || h < 1 ) return;
    int dRow = dst . getRow() - src . getRow();
    int dCol = dst . getCol() - src . getCol();
    std::pair<size_t,size_t> key, key_from, key_to;
    std::map<std::pair<size_t,size_t>, CCell> tmpSave;
    CValue tmp;

    for ( size_t r = src . getRow(); r < src . getRow() + h; r++ ) {
        for ( size_t c = src . getCol(); c < src . getCol() + w; c++ ) {
            key = std::make_pair(r,c);
            tmpSave[key] = this->table[key];
            tmpSave[key].offsetCol += dCol;
            tmpSave[key].offsetRow += dRow;
        }
    }

    for ( size_t r = dst.getRow(); r < dst.getRow() + h ; r++ ) {
        for ( size_t c = dst.getCol(); c < dst.getCol() + w; c++ ) {
            key_from = std::make_pair(r-dRow,c-dCol); // important
            key_to = std::make_pair(r,c);
            this->table[key_to] = tmpSave[key_from];
            if ( this->table[key_to].getIsFormula() && !this->table[key_to].getIsParsed() ) {
                tmp = getValue( CPosition ( r, c ) ); // first this will build the AST
                this->table[key_to].updateAST( this->table[key_to].offsetRow, this->table[key_to].offsetCol );
            }
        }
    }
}

CCell &                       CSpreadsheet::getCell              ( CPosition pos )
{   
    /**
     * @brief Return reference to a cell at a given position
     * 
     */
    std::pair<size_t,size_t> key = std::make_pair(pos.getRow(), pos.getCol());
    return this->table[key];
}
