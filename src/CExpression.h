#ifndef CEXPRESSION_H
#define CEXPRESSION_H

#include "includes.h"
#include "CPosition.h"
#include "CCell.h"
#include "expression.h"


using CValue = std::variant<std::monostate, double, std::string>;
// forward declaration to make sure references in this file work
class CCell;
class CSpreadsheet;

class CExpression {
    /**
     * @brief Parent for polymorphic implementation of ASTs
     * 
     */
public:
    virtual        ~CExpression() = default;
    virtual CValue  evaluate() const = 0;
    virtual void    update( int dRow, int dCol ) = 0;
};


class CExpressionBuilder : public CExprBuilder {
    /**
     * @brief Modified interface.
     *        Added a stack of ptrExpressions for evaluation and pointer to a CSpreadsheet
     */
public:
                    CExpressionBuilder      ( CSpreadsheet & s );
    void            opAdd                   (  ) override;
    void            opSub                   (  ) override;
    void            opMul                   (  ) override;
    void            opDiv                   (  ) override;
    void            opPow                   (  ) override;
    void            opNeg                   (  ) override;
    void            opEq                    (  ) override;
    void            opNe                    (  ) override;
    void            opLt                    (  ) override;
    void            opLe                    (  ) override;
    void            opGt                    (  ) override;
    void            opGe                    (  ) override;
    void            valNumber               ( double val  ) override;
    void            valString               ( std::string val ) override;
    void            valReference            ( std::string val ) override;
    void            valRange                ( std::string val ) override;
    void            funcCall                ( std::string fnName, int paramCount) override;
    void            assignAST               ( CCell & cell );

private:
    CSpreadsheet & spreadsheet;
    std::stack<ptrExpression> stack; 
};


class CLiteral : public CExpression {
    /**
     * @brief Polymorphic AST node representing a literal.
     * 
     */
public:
                                CLiteral            ( CValue val );
    CValue                      evaluate            (  ) const override;
    void                        update              ( int dRow, int dCol ) override;
private:
    CValue val;
};

class CReference : public CExpression {
    /**
     * @brief Polymorphic AST node representing a reference.
     * We need to remember a Cspreadsheet reference for evaluation.
     */
public:
                                CReference          ( CValue val, CSpreadsheet & s );
    CValue                      evaluate            (  ) const override;
    void                        update              ( int dRow, int dCol ) override;
private:
    CValue val;
    CSpreadsheet & spreadsheet;
    CPosition referencedPos;
};

class CAddition : public CExpression {
    /**
     * @brief Polymorphic AST node representing addition (+).
     * 
     */
public:
                                CAddition           ( ptrExpression l, ptrExpression r  );
    CValue                      evaluate            (  ) const override;
    void                        update              ( int dRow, int dCol ) override;
private:
    ptrExpression l;
    ptrExpression r;
};

class CSubtraction : public CExpression {
    /**
     * @brief Polymorphic AST node representing subtraction (-).
     * 
     */
public:
                                CSubtraction        ( ptrExpression l, ptrExpression r  );
    CValue                      evaluate            (  ) const override;
    void                        update              ( int dRow, int dCol ) override;
private:
    ptrExpression l;
    ptrExpression r;
};

class CMultiplication : public CExpression {
    /**
     * @brief Polymorphic AST node representing multiplication (*).
     * 
     */
public:
                                CMultiplication     ( ptrExpression l, ptrExpression r );
    CValue                      evaluate            (  ) const override;
    void                        update              ( int dRow, int dCol ) override;
private:
    ptrExpression l;
    ptrExpression r;
};

class CDivision : public CExpression {
    /**
     * @brief Polymorphic AST node representing float division (/).
     * 
     */
public:
                                CDivision           ( ptrExpression l, ptrExpression r );
    CValue                      evaluate            (  ) const override;
    void                        update              ( int dRow, int dCol ) override;
private:
    ptrExpression l;
    ptrExpression r;
};

class CPower : public CExpression {
    /**
     * @brief Polymorphic AST node representing the power operator (^).
     * 
     */
public:
                                CPower              ( ptrExpression l, ptrExpression r );
    CValue                      evaluate            (  ) const override;
    void                        update              ( int dRow, int dCol ) override;
private:
    ptrExpression l;
    ptrExpression r;
};

class CNegation : public CExpression {
    /**
     * @brief Polymorphic AST node representing unary negation (a := -a).
     * 
     */
public:
                                CNegation           ( ptrExpression r );
    CValue                      evaluate            (  ) const override;
    void                        update              ( int dRow, int dCol ) override;
private:
    ptrExpression r;
};

class CEq : public CExpression {
    /**
     * @brief Polymorphic AST node representing equivalence (==).
     * 
     */
public:
                                CEq           ( ptrExpression l, ptrExpression r );
    CValue                      evaluate            (  ) const override;
    void                        update              ( int dRow, int dCol ) override;
private:
    ptrExpression l;
    ptrExpression r;
};

class CNe : public CExpression {
    /**
     * @brief Polymorphic AST node representing nonequivalence (<>).
     * 
     */
public:
                                CNe           ( ptrExpression l, ptrExpression r );
    CValue                      evaluate            (  ) const override;
    void                        update              ( int dRow, int dCol ) override;
private:
    ptrExpression l;
    ptrExpression r;
};

class CLt : public CExpression {
    /**
     * @brief Polymorphic AST node representing operator <.
     * 
     */
public:
                                CLt           ( ptrExpression l, ptrExpression r );
    CValue                      evaluate            (  ) const override;
    void                        update              ( int dRow, int dCol ) override;
private:
    ptrExpression l;
    ptrExpression r;
};

class CLe : public CExpression {
    /**
     * @brief Polymorphic AST node representing operator <=.
     * 
     */
public:
                                CLe           ( ptrExpression l, ptrExpression r );
    CValue                      evaluate            (  ) const override;
    void                        update              ( int dRow, int dCol ) override;
private:
    ptrExpression l;
    ptrExpression r;
};

class CGt : public CExpression {
    /**
     * @brief Polymorphic AST node representing operator >.
     * 
     */
public:
                                CGt           ( ptrExpression l, ptrExpression r );
    CValue                      evaluate            (  ) const override;
    void                        update              ( int dRow, int dCol ) override;
private:
    ptrExpression l;
    ptrExpression r;
};

class CGe : public CExpression {
    /**
     * @brief Polymorphic AST node representing operator >=.
     * 
     */
public:
                                CGe           ( ptrExpression l, ptrExpression r );
    CValue                      evaluate            (  ) const override;
    void                        update              ( int dRow, int dCol ) override;
private:
    ptrExpression l;
    ptrExpression r;
};

#endif // CEXPRESSION_H
