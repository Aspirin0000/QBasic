#include "expression.h"
#include <cmath>

#include "myexceptions.h"
Expression::Expression() {}



int ConstantExp::eval(EvaluationContext & context)
{
    return value;
}

string ConstantExp::toString()
{
    return std::to_string(value);
}

ExpressionType ConstantExp::type()
{
    return CONSTANT;
}

int ConstantExp::getConstantValue()
{
    return value;
}

int IdentifierExp::eval(EvaluationContext & context)
{
    if (!context.isDefined(name)) throw MyException(name + " is undefined", UNDEFINEDVARIANT);
    return context.getValue(name);
}

string IdentifierExp::toString()
{
    return std::string(name);
}

ExpressionType IdentifierExp::type()
{
    return IDENTIFIER;
}

string IdentifierExp::getIdentifierName()
{
    return name;
}

int CompoundExp::eval(EvaluationContext & context)
{
    int right = rhs->eval(context);
    int left = lhs->eval(context);
    if (op == "+") return left + right;
    if (op == "-") return left - right;
    if (op == "*") return left * right;
    if (op == "/")
    {
        if (right == 0) throw std::string("Division by 0");
        return left / right;
    }
    if (op == "MOD")
    {
        int result = left % right;
        if(right > 0 && result < 0)result += right;
        if(right < 0 && result > 0)result += right;
        return result;
    }
    if (op == "**")
    {
        return std::pow(left, right);
    }
    return 0;
}

string CompoundExp::toString()
{
    return op;
}

ExpressionType CompoundExp::type()
{
    return COMPOUND;
}

string CompoundExp::getOperator()
{
    return op;
}

Expression *CompoundExp::getLHS()
{
    return lhs;
}

Expression *CompoundExp::getRHS()
{
    return rhs;
}
void CompoundExp::setLHS(Expression* exp)
{
    lhs = exp;
}
void CompoundExp::setRHS(Expression* exp)
{
    rhs = exp;
}

void EvaluationContext::setValue(const std::string& var, int value)
{
    symbolTable[var] = value;
}

int EvaluationContext::getValue(const std::string& var)
{
    return symbolTable[var];
}

bool EvaluationContext::isDefined(const std::string& var)
{
    if(symbolTable.find(var) == symbolTable.end())
        return false;
    return true;

}
