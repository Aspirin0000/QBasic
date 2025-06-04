#pragma once

#include <string>
#include <map>

using std::string;
using std::map;
/* Forward reference */

class EvaluationContext;

/*
 * Type: ExpressionType
 * --------------------
 * This enumerated type is used to differentiate the three different
 * expression types: CONSTANT, IDENTIFIER, and COMPOUND.
 */

enum ExpressionType { CONSTANT, IDENTIFIER, COMPOUND, LEFT_BRACKET, RIGHT_BRACKET};

/*
 * Class: Expression
 * -----------------
 * This class is used to represent a node in an expression tree.
 * Expression itself is an abstract class.  Every Expression object
 * is therefore created using one of the three concrete subclasses:
 * ConstantExp, IdentifierExp, or CompoundExp.
 */

class Expression {

public:

    Expression();
    virtual ~Expression() {}
    virtual int eval(EvaluationContext & context) = 0;
    virtual std::string toString() = 0;
    virtual ExpressionType type() = 0;

    /* Getter methods for convenience */

    virtual int getConstantValue() = 0;
    virtual std::string getIdentifierName() = 0;
    virtual std::string getOperator() = 0;
    virtual Expression *getLHS() = 0;
    virtual Expression *getRHS() = 0;
    virtual void setLHS(Expression* exp) = 0;
    virtual void setRHS(Expression* exp) = 0;


};

/*
 * Class: ConstantExp
 * ------------------
 * This subclass represents a constant integer expression.
 */

class ConstantExp: public Expression {

public:

    ConstantExp(int val) : value(val) {}
    virtual ~ConstantExp() {}
    virtual int eval(EvaluationContext & context);
    virtual std::string toString();
    virtual ExpressionType type();

    virtual int getConstantValue();
    virtual std::string getIdentifierName() { return "";}
    virtual std::string getOperator() { return ""; }
    virtual Expression *getLHS() { return nullptr; }
    virtual Expression *getRHS() { return nullptr; }
    virtual void setLHS(Expression* exp) {}
    virtual void setRHS(Expression* exp) {}

private:

    int value;

};

/*
 * Class: IdentifierExp
 * --------------------
 * This subclass represents a expression corresponding to a variable.
 */

class IdentifierExp: public Expression {

public:

    IdentifierExp(string name) : name(name) {}
    virtual ~IdentifierExp() {}
    virtual int eval(EvaluationContext & context);
    virtual std::string toString();
    virtual ExpressionType type();

    virtual int getConstantValue() { return 0;}
    virtual std::string getIdentifierName();
    virtual std::string getOperator() { return ""; }
    virtual Expression *getLHS() { return nullptr; }
    virtual Expression *getRHS() { return nullptr; }
    virtual void setLHS(Expression* exp) {}
    virtual void setRHS(Expression* exp) {}

private:

    std::string name;

};

/*
 * Class: CompoundExp
 * ------------------
 * This subclass represents a compound expression.
 */

class CompoundExp: public Expression {

public:

    CompoundExp(string op, Expression *lhs = nullptr, Expression *rhs = nullptr) : op(op), lhs(lhs), rhs(rhs) {}
    virtual ~CompoundExp() {}
    virtual int eval(EvaluationContext & context);
    virtual std::string toString();
    virtual ExpressionType type();

    virtual int getConstantValue() { return 0;}
    virtual std::string getIdentifierName() {return ""; }
    virtual std::string getOperator();
    virtual Expression *getLHS();
    virtual Expression *getRHS();
    virtual void setLHS(Expression* exp);
    virtual void setRHS(Expression* exp);

private:

    std::string op;
    Expression *lhs, *rhs;

};

class LeftBracketExp: public Expression
{
public:

    LeftBracketExp(){}
    ~LeftBracketExp(){}
    virtual int eval(EvaluationContext & context) {return 0;}
    virtual std::string toString() {return "(";}
    virtual ExpressionType type() {return LEFT_BRACKET;}

    virtual int getConstantValue() { return 0;}
    virtual std::string getIdentifierName() { return "";}
    virtual std::string getOperator() { return ""; }
    virtual Expression *getLHS() { return nullptr; }
    virtual Expression *getRHS() { return nullptr; }
    virtual void setLHS(Expression* exp){}
    virtual void setRHS(Expression* exp){}
};

class RightBracketExp: public Expression
{
public:

    RightBracketExp(){}
    ~RightBracketExp(){}
    virtual int eval(EvaluationContext & context) {return 0;}
    virtual std::string toString() {return ")";}
    virtual ExpressionType type() {return RIGHT_BRACKET;}

    virtual int getConstantValue() { return 0;}
    virtual std::string getIdentifierName() { return "";}
    virtual std::string getOperator() { return ""; }
    virtual Expression *getLHS() { return nullptr; }
    virtual Expression *getRHS() { return nullptr; }
    virtual void setLHS(Expression* exp){}
    virtual void setRHS(Expression* exp){}
};

/*
 * Class: EvaluationContext
 * ------------------------
 * This class encapsulates the information that the evaluator needs to
 * know in order to evaluate an expression.
 */

class EvaluationContext {

public:

    void setValue(const std::string& var, int value);
    int getValue(const std::string& var);
    bool isDefined(const std::string& var);

    map<std::string,int> symbolTable;

};
