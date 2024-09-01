#include "CSpreadsheet.h"
#include "CExpression.h"

using CValue = std::variant<std::monostate, double, std::string>;



/* ---------------------------- CLiteral ---------------------------- */

                                CLiteral::CLiteral            ( CValue val )
{ 
    try {
        double num = std::stod(std::get<std::string>(val));
        this->val = num;
    } catch ( const std::exception & ) {
        this->val = val;
    }
};
CValue                          CLiteral::evaluate            (  ) const
{
    return this->val;
};
void                            CLiteral::update              ( int dRow, int dCol )
{
    (void)dRow;
    (void)dCol;
    return;
}



/* ---------------------------- CReference ---------------------------- */

                                CReference::CReference          ( CValue val, CSpreadsheet & s )
                                                                 : val(val),
                                                                   spreadsheet(s),
                                                                   referencedPos(CPosition( std::get<std::string>(this->val) ))
                                                                {  };
CValue                          CReference::evaluate            (  ) const
{
    /* Recursively get a value from referenced cell */
    return spreadsheet.getValue(referencedPos);
}
void                            CReference::update              ( int dRow, int dCol )
{   
    /* update offset to a referenced cell */
    if ( ! referencedPos.getColAbs() ) referencedPos.setCol( referencedPos.getCol() + dCol );
    if ( ! referencedPos.getRowAbs() ) referencedPos.setRow( referencedPos.getRow() + dRow );
}


/* ---------------------------- CAddition ---------------------------- */

                                CAddition::CAddition ( ptrExpression l, ptrExpression r )
                                                     : l(std::move(l)), 
                                                       r(std::move(r)) {  };
void                            CAddition::update              ( int dRow, int dCol )
{
    l->update(dRow,dCol); r->update(dRow,dCol); 
}
CValue                          CAddition::evaluate (  ) const
{
    CValue lVal = l->evaluate();
    CValue rVal = r->evaluate();

    if (std::holds_alternative<double>(lVal) && std::holds_alternative<double>(rVal)) {
        return std::get<double>(lVal) + std::get<double>(rVal);
    } else if (std::holds_alternative<std::string>(lVal) && std::holds_alternative<double>(rVal)) {
        return std::get<std::string>(lVal) + std::to_string(std::get<double>(rVal));
    } else if (std::holds_alternative<double>(lVal) && std::holds_alternative<std::string>(rVal)) {
        return std::to_string(std::get<double>(lVal)) + std::get<std::string>(rVal);
    } else if (std::holds_alternative<std::string>(lVal) && std::holds_alternative<std::string>(rVal)) {
        return std::get<std::string>(lVal) + std::get<std::string>(rVal);
    } else { 
        return std::monostate();
    }
}


/* ---------------------------- CSubtraction ---------------------------- */
                            CSubtraction::CSubtraction          ( ptrExpression l, ptrExpression r )
                                                                 : l(std::move(l)), 
                                                                   r(std::move(r)) {  };
CValue                      CSubtraction::evaluate              (  ) const
{
    CValue lVal = l->evaluate();
    CValue rVal = r->evaluate();

    if (std::holds_alternative<double>(lVal) && std::holds_alternative<double>(rVal)) {
        return std::get<double>(lVal) - std::get<double>(rVal);
    } else {
        return std::monostate();
    }
}
void                        CSubtraction::update                ( int dRow, int dCol )
{
    l->update(dRow,dCol); r->update(dRow,dCol); 
}


/* ---------------------------- CMultiplication ---------------------------- */
                            CMultiplication::CMultiplication     ( ptrExpression l, ptrExpression r )
                                                                  : l(std::move(l)), 
                                                                    r(std::move(r)) {  };
CValue                      CMultiplication::evaluate            (  ) const
{
    CValue lVal = l->evaluate();
    CValue rVal = r->evaluate();

    if (std::holds_alternative<double>(lVal) && std::holds_alternative<double>(rVal)) {
        return std::get<double>(lVal) * std::get<double>(rVal);
    } else {
        return std::monostate();
    }
}
void                        CMultiplication::update              ( int dRow, int dCol )
{
    l->update(dRow,dCol); r->update(dRow,dCol); 
}

/* ---------------------------- CDivision ---------------------------- */
                            CDivision::CDivision            ( ptrExpression l, ptrExpression r )
                                                             : l(std::move(l)), 
                                                               r(std::move(r)) {  };
CValue                      CDivision::evaluate             (  ) const
{
    CValue lVal = l->evaluate();
    CValue rVal = r->evaluate();
    if ( std::holds_alternative<double>(rVal) && std::get<double>(rVal)==0 ) return std::monostate();

    if (std::holds_alternative<double>(lVal) && std::holds_alternative<double>(rVal)) {
        return std::get<double>(lVal) / std::get<double>(rVal);
    } else {
        return std::monostate();
    }
}
void                        CDivision::update              ( int dRow, int dCol )
{
    l->update(dRow,dCol); r->update(dRow,dCol); 
}

/* ---------------------------- CPower ---------------------------- */
                            CPower::CPower                  ( ptrExpression l, ptrExpression r )
                                                             : l(std::move(l)), 
                                                               r(std::move(r)) {  };
CValue                      CPower::evaluate                (  ) const
{
    CValue lVal = l->evaluate();
    CValue rVal = r->evaluate();

    if (std::holds_alternative<double>(lVal) && std::holds_alternative<double>(rVal)) {
        return pow(std::get<double>(lVal), std::get<double>(rVal));
    } else {
        return std::monostate();
    }
}       
void                        CPower::update                  ( int dRow, int dCol )
{
    l->update(dRow,dCol); r->update(dRow,dCol); 
}

/* ---------------------------- CNegation ---------------------------- */
                            CNegation::CNegation            ( ptrExpression r )
                                                             : r(std::move(r)) {  };
CValue                      CNegation::evaluate             (  ) const
{
    CValue rVal = r->evaluate();

    if (std::holds_alternative<double>(rVal)) {
        return -std::get<double>(rVal);
    } else {
        return std::monostate();
    }
}       
void                        CNegation::update               ( int dRow, int dCol )
{
    r->update(dRow,dCol); 
}

/* ---------------------------- CEq ---------------------------- */
                            CEq::CEq          ( ptrExpression l, ptrExpression r )
                                                                 : l(std::move(l)), 
                                                                   r(std::move(r)) {  };
CValue                      CEq::evaluate              (  ) const
{
    CValue lVal = l->evaluate();
    CValue rVal = r->evaluate();

    if (std::holds_alternative<double>(lVal) && std::holds_alternative<double>(rVal)) {
        return CValue(static_cast<double>(std::get<double>(lVal) == std::get<double>(rVal)));
    } else if (std::holds_alternative<std::string>(lVal) && std::holds_alternative<std::string>(rVal)) {
        return CValue(static_cast<double>(std::get<std::string>(lVal) == std::get<std::string>(rVal)));
    } else {
        return CValue(0.0);
    }
}
void                        CEq::update                ( int dRow, int dCol )
{
    l->update(dRow,dCol); r->update(dRow,dCol); 
}

/* ---------------------------- CNe ---------------------------- */
                            CNe::CNe          ( ptrExpression l, ptrExpression r )
                                                                 : l(std::move(l)), 
                                                                   r(std::move(r)) {  };
CValue                      CNe::evaluate              (  ) const
{
    CValue lVal = l->evaluate();
    CValue rVal = r->evaluate();

    if (std::holds_alternative<double>(lVal) && std::holds_alternative<double>(rVal)) {
        return CValue(static_cast<double>(std::get<double>(lVal) != std::get<double>(rVal)));
    } else if (std::holds_alternative<std::string>(lVal) && std::holds_alternative<std::string>(rVal)) {
        return CValue(static_cast<double>(std::get<std::string>(lVal) != std::get<std::string>(rVal)));
    } else {
        return CValue(1.0);
    }
}
void                        CNe::update                ( int dRow, int dCol )
{
    l->update(dRow,dCol); r->update(dRow,dCol); 
}

/* ---------------------------- CLt ---------------------------- */
                            CLt::CLt          ( ptrExpression l, ptrExpression r )
                                                                 : l(std::move(l)), 
                                                                   r(std::move(r)) {  };
CValue                      CLt::evaluate              (  ) const
{
    CValue lVal = l->evaluate();
    CValue rVal = r->evaluate();

    if (std::holds_alternative<double>(lVal) && std::holds_alternative<double>(rVal)) {
        return CValue(static_cast<double>(std::get<double>(lVal) < std::get<double>(rVal)));
    } else if (std::holds_alternative<std::string>(lVal) && std::holds_alternative<std::string>(rVal)) {
        return CValue(static_cast<double>(std::get<std::string>(lVal) < std::get<std::string>(rVal)));
    } else {
        return CValue(0.0);
    }
}
void                        CLt::update                ( int dRow, int dCol )
{
    l->update(dRow,dCol); r->update(dRow,dCol); 
}

/* ---------------------------- CLe ---------------------------- */
                            CLe::CLe          ( ptrExpression l, ptrExpression r )
                                                                 : l(std::move(l)), 
                                                                   r(std::move(r)) {  };
CValue                      CLe::evaluate              (  ) const
{
    CValue lVal = l->evaluate();
    CValue rVal = r->evaluate();

    if (std::holds_alternative<double>(lVal) && std::holds_alternative<double>(rVal)) {
        return CValue(static_cast<double>(std::get<double>(lVal) <= std::get<double>(rVal)));
    } else if (std::holds_alternative<std::string>(lVal) && std::holds_alternative<std::string>(rVal)) {
        return CValue(static_cast<double>(std::get<std::string>(lVal) <= std::get<std::string>(rVal)));
    } else {
        return CValue(0.0);
    }
}
void                        CLe::update                ( int dRow, int dCol )
{
    l->update(dRow,dCol); r->update(dRow,dCol); 
}

/* ---------------------------- CGt ---------------------------- */
                            CGt::CGt          ( ptrExpression l, ptrExpression r )
                                                                 : l(std::move(l)), 
                                                                   r(std::move(r)) {  };
CValue                      CGt::evaluate              (  ) const
{
    CValue lVal = l->evaluate();
    CValue rVal = r->evaluate();

    if (std::holds_alternative<double>(lVal) && std::holds_alternative<double>(rVal)) {
        return CValue(static_cast<double>(std::get<double>(lVal) > std::get<double>(rVal)));
    } else if (std::holds_alternative<std::string>(lVal) && std::holds_alternative<std::string>(rVal)) {
        return CValue(static_cast<double>(std::get<std::string>(lVal) > std::get<std::string>(rVal)));
    } else {
        return CValue(0.0);
    }
}
void                        CGt::update                ( int dRow, int dCol )
{
    l->update(dRow,dCol); r->update(dRow,dCol); 
}

/* ---------------------------- CGe ---------------------------- */
                            CGe::CGe          ( ptrExpression l, ptrExpression r )
                                                                 : l(std::move(l)), 
                                                                   r(std::move(r)) {  };
CValue                      CGe::evaluate              (  ) const
{
    CValue lVal = l->evaluate();
    CValue rVal = r->evaluate();

    if (std::holds_alternative<double>(lVal) && std::holds_alternative<double>(rVal)) {
        return CValue(static_cast<double>(std::get<double>(lVal) >= std::get<double>(rVal)));
    } else if (std::holds_alternative<std::string>(lVal) && std::holds_alternative<std::string>(rVal)) {
        return CValue(static_cast<double>(std::get<std::string>(lVal) >= std::get<std::string>(rVal)));
    } else {
        return CValue(0.0);
    }
}
void                        CGe::update                ( int dRow, int dCol )
{
    l->update(dRow,dCol); r->update(dRow,dCol); 
}











                CExpressionBuilder::CExpressionBuilder      ( CSpreadsheet & s )
                                                             : spreadsheet(s) 
/**
 * @brief Constructor. Passes a reference to a given spreadsheet
 *        Neccessary for managing references
 */
{  };


void            CExpressionBuilder::opAdd                   (  )
{
    /* Add + node to AST */
    if ( stack.size() < 2 ) return;

    auto right = std::move(stack.top()); stack.pop();
    auto left  = std::move(stack.top()); stack.pop();
    stack.push(std::make_unique<CAddition>(std::move(left), std::move(right)));
}

void            CExpressionBuilder::opSub                   (  )
{
    /* Add - node to AST */
    if ( stack.size() < 2 ) return;

    auto right = std::move(stack.top()); stack.pop();
    auto left  = std::move(stack.top()); stack.pop();
    stack.push(std::make_unique<CSubtraction>(std::move(left), std::move(right)));
}

void            CExpressionBuilder::opMul                   (  )
{
    /* Add * node to AST */
    if ( stack.size() < 2 ) return;

    auto right = std::move(stack.top()); stack.pop();
    auto left  = std::move(stack.top()); stack.pop();
    stack.push(std::make_unique<CMultiplication>(std::move(left), std::move(right)));
}

void            CExpressionBuilder::opDiv                   (  )
{
    /* Add / node to AST */
    if ( stack.size() < 2 ) return;

    auto right = std::move(stack.top()); stack.pop();
    auto left  = std::move(stack.top()); stack.pop();
    stack.push(std::make_unique<CDivision>(std::move(left), std::move(right)));
}

void            CExpressionBuilder::opPow                   (  )
{
    /* Add ^ node to AST */
    if ( stack.size() < 2 ) return;

    auto right = std::move(stack.top()); stack.pop();
    auto left  = std::move(stack.top()); stack.pop();
    stack.push(std::make_unique<CPower>(std::move(left), std::move(right)));
}

void            CExpressionBuilder::opNeg                   (  )
{
    /* Add != node to AST */
    if ( stack.size() < 1 ) return;

    auto right = std::move(stack.top()); stack.pop();
    stack.push(std::make_unique<CNegation>(std::move(right)));
}

void            CExpressionBuilder::opEq                    (  )
{
    /* Add == node to AST */
    if ( stack.size() < 2 ) return;

    auto right = std::move(stack.top()); stack.pop();
    auto left  = std::move(stack.top()); stack.pop();
    stack.push(std::make_unique<CEq>(std::move(left), std::move(right)));
}

void            CExpressionBuilder::opNe                    (  )
{
    /* Add <> node to AST */
    if ( stack.size() < 2 ) return;

    auto right = std::move(stack.top()); stack.pop();
    auto left  = std::move(stack.top()); stack.pop();
    stack.push(std::make_unique<CNe>(std::move(left), std::move(right)));
}

void            CExpressionBuilder::opLt                    (  )
{
    /* Add <= node to AST */
    if ( stack.size() < 2 ) return;

    auto right = std::move(stack.top()); stack.pop();
    auto left  = std::move(stack.top()); stack.pop();
    stack.push(std::make_unique<CLt>(std::move(left), std::move(right)));
}

void            CExpressionBuilder::opLe                    (  )
{
    /* Add <= node to AST */
    if ( stack.size() < 2 ) return;

    auto right = std::move(stack.top()); stack.pop();
    auto left  = std::move(stack.top()); stack.pop();
    stack.push(std::make_unique<CLe>(std::move(left), std::move(right)));
}

void            CExpressionBuilder::opGt                    (  )
{
    /* Add > node to AST */
    if ( stack.size() < 2 ) return;

    auto right = std::move(stack.top()); stack.pop();
    auto left  = std::move(stack.top()); stack.pop();
    stack.push(std::make_unique<CGt>(std::move(left), std::move(right)));
}

void            CExpressionBuilder::opGe                    (  )
{
    /* Add >= node to AST */
    if ( stack.size() < 2 ) return;

    auto right = std::move(stack.top()); stack.pop();
    auto left  = std::move(stack.top()); stack.pop();
    stack.push(std::make_unique<CGe>(std::move(left), std::move(right)));
}

void            CExpressionBuilder::valNumber               ( double val  )
{
    /* Add number value node to AST */
    stack.push(std::make_unique<CLiteral>(val));
}

void            CExpressionBuilder::valString               ( std::string val )
{
    /* Add string value node to AST */
    stack.push(std::make_unique<CLiteral>(val));
}

void            CExpressionBuilder::valReference            ( std::string val )
{
    /* Add reference node to AST */
    stack.push(std::make_unique<CReference>(val, this->spreadsheet));
}

void            CExpressionBuilder::valRange                ( std::string val )
{
    /* NOT IMPLEMENTED */
    (void)val;
}

void            CExpressionBuilder::funcCall                ( std::string fnName, int paramCount)
{
    /* NOT IMPLEMENTED */
    (void)fnName;
    (void)paramCount;
}

void            CExpressionBuilder::assignAST               ( CCell & cell )
{
    /**
     * @brief Move a unique pointer of a root AST node to a given cell
     * 
     */
    if (!stack.empty()) {
        ptrExpression AST = std::move(stack.top());
        stack.pop();
        cell.setASTPtr(std::move(AST));
    }
}