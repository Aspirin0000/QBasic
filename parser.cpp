
#include "parser.h"

#include <stack>
#include <stdexcept>

Parser::~Parser()
{
    delete tokenizer;
    delete expressionTree;
}

bool Parser::checkStringValid()
{
    return true;
}

Expression * Parser::constructExpressionTree()
{
    if(checkStringValid() == false)
    {
        throw MyException("Invalid Expression", INVALIDEXPRESSION);
    }
    tokenizer = new Tokenizer(expressionString);
    tokenizer -> constructTokens();
    std::vector<token> tokens = tokenizer->getTokens();
    for(auto token : tokens)
    {
        Expression * node;
        if(token.getType() == TokenType::CONSTANTS)
        {
            node = new ConstantExp(stoi(token.getValue()));
        }
        else if(token.getType() == TokenType::VARIANT)
        {
            node = new IdentifierExp(token.getValue());
        }
        else if(token.getType() == TokenType::OPERATOR)
        {
            node = new CompoundExp(token.getValue());
        }
        else if(token.getType() == TokenType::LEFT_BRACKETS)
        {
            node = new LeftBracketExp;
        }
        else if(token.getType() == TokenType::RIGHT_BRACKETS)
        {
            node = new RightBracketExp;
        }
        nodeList.push_back(node);
    }
    std::stack<Expression*> operatorStack;
    std::stack<Expression*> numStack;
    auto calc = [&numStack](Expression * op) {
        Expression* num2 = numStack.top();
        numStack.pop();
        Expression* num1 = numStack.top();
        numStack.pop();
        op->setLHS(num1);
        op->setRHS(num2);
        numStack.push(op);
    };
    for(int i = 0; i < nodeList.size(); i++)
    {
        if(tokens[i].getType() == TokenType::CONSTANTS || tokens[i].getType() == TokenType::VARIANT)
        {
            numStack.push(nodeList[i]);
        }
        else if(tokens[i].getType() == TokenType::LEFT_BRACKETS)
        {
            operatorStack.push(nodeList[i]);
        }
        else if(tokens[i].getType() == TokenType::RIGHT_BRACKETS)
        {
            Expression* op = operatorStack.top();
            operatorStack.pop();
            while(op->type() != ExpressionType::LEFT_BRACKET)
            {
                calc(op);
                op = operatorStack.top();
                operatorStack.pop();
            }
        }
        else if(tokens[i].getType() == TokenType::OPERATOR)
        {
            if(!operatorStack.empty()) {
                Expression* op = operatorStack.top();
                if(op -> type() == ExpressionType::LEFT_BRACKET)
                {
                    operatorStack.push(nodeList[i]);
                    continue;
                }
                while(!operatorStack.empty() && op->type() != ExpressionType::LEFT_BRACKET &&
                    precedenceTable[op->toString()] >= precedenceTable[nodeList[i]->toString()])
                {
                    operatorStack.pop();
                    calc(op);
                    if(!operatorStack.empty())op = operatorStack.top();
                    else break;
                }
            }
            operatorStack.push(nodeList[i]);
        }
    }
    while(!operatorStack.empty())
    {
        Expression * op = operatorStack.top();
        operatorStack.pop();
        calc(op);
    }
    expressionTree = numStack.top();
    return expressionTree;
}
const std::string fourSpace = "    ";

std::string Parser::constructExpressionSyntaxTree(Expression* root, int num) {
    if(root == nullptr)return "";
    string answer = "\n";
    for(int i = 0; i < num; i++) {
        answer += fourSpace;
    }
    answer += root -> toString();
    answer += constructExpressionSyntaxTree(root -> getLHS(), num + 1);
    answer += constructExpressionSyntaxTree(root -> getRHS(), num + 1);
    return answer;
}
