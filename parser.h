
#ifndef PARSER_H
#define PARSER_H

#include "expression.h"
#include "tokenizer.h"
#include "myexceptions.h"
#include <unordered_map>

inline std::unordered_map<std::string, int> precedenceTable =
    {
        {"+",0},
        {"-",0},
        {"*",1},
        {"/",1},
        {"mod",1},
        {"**",2},
    };

class Parser {
public:
    Parser(std::string str) : expressionString(str), tokenizer(nullptr), expressionTree(nullptr) {}
    ~Parser();
    bool checkStringValid();
    Expression* constructExpressionTree();
    Expression* getExpressionTree() { return expressionTree; }
    std::string constructExpressionSyntaxTree(Expression* root, int num);
private:
    std::string expressionString;
    Tokenizer *tokenizer;
    Expression *expressionTree;
    std::vector<Expression *> nodeList;
};



#endif //PARSER_H
