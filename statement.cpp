//
// Created by 26403 on 25-1-2.
//

#include "statement.h"

#include <algorithm>
#include <cmath>
#include <cmath>
#include <stdexcept>
#include <sstream>

#include "checkIsValid.h"

const std::string fourSpace = "    ";




void RemStmt::parseLineString() {
    std::string temp;
    std::stringstream ss(getLineString());
    ss >> temp;
    setLineNumber(std::stoi(temp));
    ss >> temp;
    char ch;
    while(ss.get(ch)) {
        remString += ch;
    }
}

std::string RemStmt::syntaxTreeString() {
    return std::to_string(getLineNumber()) + " REM\n" + fourSpace + remString;
}

int RemStmt::executeStatement(int &nextLineNumber, evalState* evalState, int optionalArgs) {
    return 0;
}

void LetStmt::parseLineString()
{
    std::string temp;
    std::stringstream ss(getLineString());
    ss >> temp;
    setLineNumber(std::stoi(temp));
    ss >> temp;
    ss >> temp;
    leftVariant = temp;
    checkVariantName checker(leftVariant);
    if(checker.checkVariantNameValid() == false) {
        throw MyException("Invalid VariantName", INVALIDVARIANT);
    }
    ss >> temp;
    char ch;
    string line;
    while(ss.get(ch)) {
        line += ch;
    }
    parser = new Parser(line);
    parser -> constructExpressionTree();
}

std::string LetStmt::syntaxTreeString() {
    std::string answer = std::to_string(getLineNumber()) + " LET =\n" + fourSpace + leftVariant;
    answer += parser->constructExpressionSyntaxTree(parser->getExpressionTree(),1);
    return answer;
}

int LetStmt::executeStatement(int &nextLineNumber, evalState* evalState, int optionalArgs) {
    evalState->setValue(leftVariant, parser->getExpressionTree()->eval(*evalState->getEvaluationContext()));
    return 0;
}

void PrintStmt::parseLineString() {
    std::string temp;
    std::stringstream ss(getLineString());
    ss >> temp;
    setLineNumber(std::stoi(temp));
    ss >> temp;
    char ch;
    string line;
    while(ss.get(ch)) {
        line += ch;
    }
    parser = new Parser(line);
    parser -> constructExpressionTree();
}

std::string PrintStmt::syntaxTreeString() {
    std::string answer = std::to_string(getLineNumber()) + " PRINT" ;
    answer += parser->constructExpressionSyntaxTree(parser->getExpressionTree(),1);
    return answer;
}

int PrintStmt::executeStatement(int &nextLineNumber, evalState *evalState, int optionalArgs) {
    return parser->getExpressionTree()->eval(*evalState->getEvaluationContext());
}

void InputStmt::parseLineString() {
    std::string temp;
    std::stringstream ss(getLineString());
    ss >> temp;
    setLineNumber(std::stoi(temp));
    ss >> temp;
    ss >> temp;
    leftVariant = temp;
    checkVariantName checker(leftVariant);
    if(checker.checkVariantNameValid() == false) {
        throw MyException("Invalid VariantName", INVALIDVARIANT);
    }
}

int InputStmt::executeStatement(int &nextLineNumber, evalState *evalState, int optionalArgs) {
    evalState->setValue(leftVariant, optionalArgs);
    return 0;
}

void GotoStmt::parseLineString() {
    std::string temp;
    std::stringstream ss(getLineString());
    ss >> temp;
    setLineNumber(std::stoi(temp));
    ss >> temp;
    ss >> temp;
    nextLineNum = std::stoi(temp);
}

std::string GotoStmt::syntaxTreeString() {
    return std::to_string(getLineNumber()) + " GOTO\n" + fourSpace + std::to_string(nextLineNum);
}

int GotoStmt::executeStatement(int &nextLineNumber, evalState *evalState, int optionalArgs) {
    nextLineNumber = nextLineNum;
    return 0;
}


//IF exp1 op exp2 THEN n
void IfStmt::parseLineString() {
    std::string temp;
    std::stringstream ss(getLineString());
    ss >> temp;
    setLineNumber(std::stoi(temp));
    ss >> temp;
    std::string line = getLineString();
    int index1 = line.find_first_of("IF") + 2;
    int index3 = line.find_last_of("THEN") - 4;
    int index2 =  line.find_first_of('<');
    op = '<';
    if(index2 == -1) {
        index2 = line.find_last_of('>');
        op = '>';
        if(index2 == -1) {
            index2 = line.find_first_of('=');
            op = '=';
        }
    }
    parser1 = new Parser(line.substr(index1, index2 - index1));
    parser2 = new Parser(line.substr(index2, index3 - index2));
    parser1 -> constructExpressionTree();
    parser2 -> constructExpressionTree();
    std::reverse(line.begin(), line.end());
    std::stringstream ssr(line);
    ssr >> temp;
    std::reverse(temp.begin(), temp.end());
    nextLineNum = std::stoi(temp);
}

std::string IfStmt::syntaxTreeString() {
    string answer = std::to_string(getLineNumber()) + " IF THEN";
    answer += parser1 -> constructExpressionSyntaxTree(parser1->getExpressionTree(),1);
    answer += "\n" + fourSpace + op;
    answer += parser2 -> constructExpressionSyntaxTree(parser2->getExpressionTree(),1);
    answer += "\n" + fourSpace + std::to_string(nextLineNum);
    return answer;
}

int IfStmt::executeStatement(int &nextLineNumber, evalState *evalState, int optionalArgs) {
    int exp1 = parser1->getExpressionTree()->eval(*evalState->getEvaluationContext());
    int exp2 = parser2->getExpressionTree()->eval(*evalState->getEvaluationContext());
    if(op == "<") {
        if(exp1 < exp2) {
            nextLineNumber = nextLineNum;
        }
    }
    else if(op == ">") {
        if(exp1 > exp2) {
            nextLineNumber = nextLineNum;
        }
    }
    else if(op == "=") {
        if(exp1 == exp2) {
            nextLineNumber = nextLineNum;
        }
    }
    return 0;
}




void EndStmt::parseLineString() {
    std::string temp;
    std::stringstream ss(getLineString());
    ss >> temp;
    setLineNumber(std::stoi(temp));
}













