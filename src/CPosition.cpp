#include "includes.h"
#include "CPosition.h"

using CValue = std::variant<std::monostate, double, std::string>;


                    CPosition::CPosition                              ( size_t row, size_t col )
                                                             : offsetRow(0),
                                                               offsetCol(0),
                                                               row(row),
                                                               col(col),
                                                               rowAbs(false),
                                                               colAbs(false) 
/**
 * @brief CPosition constructir from table coordinates.
 * 
 */
{

};

                    CPosition::CPosition                              ( std::string_view str ) 
                                                             : offsetRow(0),
                                                               offsetCol(0),
                                                               row(0),
                                                               col(0),
                                                               rowAbs(false),
                                                               colAbs(false)
/**
 * @brief CPosition constructor from string that represents a cell identifier
 * 
 */
{
    size_t idx = 0;

    if ( str[idx] == '$' ) {
        colAbs = true;
        idx++;
    }

    while ( std::isalpha(str[idx]) ) idx++;

    if ( str[idx] == '$' ) {
        rowAbs = true;
        idx++;
    }

    if ( idx == 0 || idx == str.length() || !std::isdigit(str[idx]) )
        throw std::invalid_argument("Unable to parse invalid cell identifier.");

    this->row = std::stod(std::string(str.substr(idx,std::string::npos)));

    // modified Horner's method
    for ( size_t i = ((str[0] == '$') ? 1 : 0); i < ((str[idx-1] == '$') ? idx-1 : idx); i++ ) {
        this -> col = this -> col * 26 + std::toupper(str[i]) - 'A' + 1;
    }
}
size_t              CPosition::getRow                (  ) const
{
    return this -> row; 
};
size_t              CPosition::getCol                (  ) const
{
    return this -> col; 
};
void                CPosition::setRow                ( size_t row )
{
    this -> row = row; 
};
void                CPosition::setCol                ( size_t col )
{
    this -> col = col; 
};
bool                CPosition::getRowAbs             (  ) const
{
    return this -> rowAbs;
}
bool                CPosition::getColAbs             (  ) const
{
    return this -> colAbs;
}
void                CPosition::setRowAbs             ( bool rowAbs )
{
    this -> rowAbs = rowAbs; 
};
void                CPosition::setColAbs             ( bool colAbs )
{
    this -> colAbs = colAbs; 
};
