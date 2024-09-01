#ifndef CSPREADSHEET_H
#define CSPREADSHEET_H

#include "includes.h"
#include "CPosition.h"
#include "CCell.h"
#include "CExpression.h"

using CValue = std::variant<std::monostate, double, std::string>;

class CSpreadsheet
/**
 * @brief The core class.
 *        Specifies an interface through which user interacts with a spreadsheet.
 *        All unnecesary details are hidden from user.
 */
{
public:
                                        CSpreadsheet                            ();
                                        CSpreadsheet                            ( const CSpreadsheet & other );
    CSpreadsheet &                      operator=                               ( const CSpreadsheet & other );
    bool                                load                                    ( std::istream                        & is );
    bool                                save                                    ( std::ostream                        & os ) const;
    bool                                setCell                                 ( CPosition                                  pos,
                                                                                  std::string                           contents );
    CValue                              getValue                                ( CPosition                                  pos );
    void                                copyRect                                ( CPosition                                  dst,
                                                                                  CPosition                                  src,
                                                                                  int                                   w = 1,
                                                                                  int                                   h = 1 );
    CCell &                             getCell                                 ( CPosition                                  pos );
private:
    CExpressionBuilder builder;
    std::map<std::pair<size_t,size_t>, CCell> table;
};

# endif // CSPREADSHEET_H