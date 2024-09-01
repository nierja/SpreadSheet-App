#include "includes.h"
#include "CPosition.h"
#include "CExpression.h"
#include "CCell.h"

                                CCell::CCell                                    (  )
                                                                                : offsetRow(0),
                                                                                  offsetCol(0),
                                                                                  val(CValue()),
                                                                                  contents(""),
                                                                                  isFormula(false), 
                                                                                  isParsed(false),
                                                                                  ptr(nullptr)
/**
 * @brief Default constructor
 * 
 */
                                                                                {}

                                CCell::CCell                                    ( const CCell & other ) 
                                                                                 : offsetRow(other.offsetRow),
                                                                                   offsetCol(other.offsetCol),
                                                                                   val(other.val), 
                                                                                   contents(other.contents),
                                                                                   isFormula(other.isFormula), 
                                                                                   isParsed(false), 
                                                                                   ptr(nullptr) 
/**
 * @brief Copy constructor
 * 
 */
{  };

CCell &                         CCell::operator=                                ( const CCell & other ) 
/**
 * @brief Copy assignment operator
 * 
 */
{
    if ( this != & other ) {
        val = other.val;
        contents = other.contents;
        isFormula = other.isFormula;
        isParsed = false;
        ptr = nullptr;
        offsetRow = other.offsetRow;
        offsetCol = other.offsetCol;        
    }
    return *this;
}

                                CCell::CCell                                   ( std::string contents  )
                                                                                : contents(contents),
                                                                                  isFormula(false), 
                                                                                  isParsed(false)
/**
 * @brief Constructor from string representation of cell contents
 * 
 */
{
    isFormula = false;
    offsetRow = 0;
    offsetCol = 0;  
    if ( contents.empty() ) this->val = CValue();
    if ( contents.front() == '=' ) {
        isFormula = true;
        this->val = std::move(contents);
    } else {
        try {
            this->val = std::stod(contents);
        } catch ( const std::exception & ) {
            this->val = std::move(contents);
        }
    }  
}

CValue                          CCell::getValue                                (  ) const
{
    /**
     * @brief Extract vaalue from a cell object
     * 
     * @param isParsed - indicate whether cell contains a parsed formula
     */
    if ( isFormula && isParsed ) {
        return ptr->evaluate();
    }
    return this -> val;
}

void                            CCell::updateAST                               ( int dRow, int dCol )
{
    /**
     * @brief Update AST tree references
     * 
     * @param dRow - ofset to the referenced cell
     * @param dCol - ofset to the referenced cell
     */
    if ( isFormula ) {
        return ptr->update( dRow, dCol );
    }
}

bool                            CCell::getIsFormula                            (  ) const 
{
    return this->isFormula;
}

bool                            CCell::getIsParsed                             (  ) const 
{
    return this->isParsed;
}

bool                            CCell::getIsEmpty                              (  ) const
{
    /**
     * @brief Indicate whether a given cell contains std::monostate()
     * 
     */
    if (std::holds_alternative<std::monostate>(val))
        return true;
    return false;
}

bool                            CCell::getHasNumber                            (  ) const
{
    if (std::holds_alternative<double>(val))
        return true;
    return false;
}

bool                            CCell::getHasString                            (  ) const
{
    if (std::holds_alternative<std::string>(val))
        return true;
    return false;
}

const std::string &             CCell::getContents                             (  ) const
{
    return this->contents;
}

void                            CCell::setASTPtr                               ( ptrExpression AST )
{
    /**
     * @brief Move the unique pointer to a cell
     * 
     */
    this->ptr=std::move(AST);
}

void                            CCell::setIsParsed                             ( bool parsed )
{
    this->isParsed = parsed;
}

std::ostream &           operator<<                              ( std::ostream & os, const CCell & cell )
{   
    /**
     * @brief Helper overloaded << for debugging
     * @param ostream & os - where to print
     * @param const CCell & cell - which cell
     */
    os << "Cell:" << std::endl;
    os << "{" << std::endl;
    os << "    getHasNumber() : " << cell.getHasNumber() << std::endl;
    os << "    getHasString() : " << cell.getHasString() << std::endl;
    os << "    getIsEmpty()   : " << cell.getIsEmpty() << std::endl;
    os << "    getIsFormula() : " << cell.getIsFormula() << std::endl;
    os << "    getIsParsed()  : " << cell.getIsParsed() << std::endl;
    os << "    offsetRow      : " << cell.offsetRow << std::endl;
    os << "    offsetCol      : " << cell.offsetCol << std::endl;
    os << "}";
    return os;
}