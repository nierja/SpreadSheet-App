#ifndef CCELL_H
#define CCELL_H

#include "includes.h"
#include "CPosition.h"

using CValue = std::variant<std::monostate, double, std::string>;

class CExpression;
using ptrExpression = std::unique_ptr<CExpression>;

class CCell
/**
 * @brief Cell representation. We track the cells contents, whether it is a formul, whether it was parsed
 *        and optionally an unique pointer to an AST root node.
 */
{
    public:
                                        CCell                                   (  );
                                        CCell                                   ( const CCell & other );
        CCell &                         operator=                               ( const CCell & other );
                                        CCell                                   ( std::string contents  );
        CValue                          getValue                                (  ) const;
        bool                            getIsFormula                            (  ) const;
        bool                            getIsParsed                             (  ) const;
        bool                            getIsEmpty                              (  ) const;
        bool                            getHasNumber                            (  ) const;
        bool                            getHasString                            (  ) const;
        const std::string &             getContents                             (  ) const;
        void                            setASTPtr                               ( ptrExpression AST );
        void                            updateAST                               ( int dRow, int dCol );
        void                            setIsParsed                             ( bool parsed );
        friend std::ostream &           operator<<                              ( std::ostream & os, const CCell & cell );
        int offsetRow;          // would be better as private, I was short on time
        int offsetCol;          // would be better as private, I was short on time    
        State state = EMPTY;    // would be better as private, I was short on time    
    private:
        CValue val;
        std::string contents;
        bool isFormula;
        bool isParsed;
        ptrExpression ptr;
};

#endif // CCELL_H