//
// Created by 26403 on 25-1-3.
//

#include "program.h"

#include <algorithm>
#include <list>
#include <regex>
#include <QEventLoop>


program::program(QObject *parent) : QObject{parent}
{

}

program::~program() {
    delete evalSt;
    statementMap.clear();
    for(auto element: statements) {
        delete element;
    }
}


Statement *program::creatStatement(string str)
{
    Statement* statement;
    std::regex remRegex(R"(^\s*(\d+)\s+REM\s+(.*)\s*$)");
    std::regex letRegex(R"(^\s*(\d+)\s+LET\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*(.*)\s*$)");
    std::regex printRegex(R"(^\s*(\d+)\s+PRINT\s+(.*)\s*$)");
    std::regex inputRegex(R"(^\s*(\d+)\s+INPUT\s+(\w+)\s*$)");
    std::regex gotoRegex(R"(^\s*(\d+)\s+GOTO\s+(\d+)\s*$)");
    std::regex ifRegex(R"(^\s*(\d+)\s+IF\s+(.+?)\s*(<|>|=)\s*(.+?)\s+THEN\s+(\d+)\s*$)");
    std::regex endRegex(R"(^\s*(\d+)\s+END\s*$)");

    std::smatch match;
    if (std::regex_match(str, match, remRegex))
    {
        statement = new RemStmt(str);
    }
    else if(std::regex_match(str, match, letRegex))
    {
        statement = new LetStmt(str);
    }
    else if(std::regex_match(str, match, printRegex))
    {
        statement = new PrintStmt(str);
    }
    else if(std::regex_match(str, match, inputRegex))
    {
        statement = new InputStmt(str);
    }
    else if(std::regex_match(str, match, gotoRegex))
    {
        statement = new GotoStmt(str);
    }
    else if(std::regex_match(str, match, ifRegex))
    {
        statement = new IfStmt(str);
    }
    else if(std::regex_match(str, match, endRegex))
    {
        statement = new EndStmt(str);
    }
    else statement = nullptr;
    return statement;
}

bool program::checkLineNumber(int lineNumber)
{
    auto it = statementMap.find(lineNumber);
    if(lineNumber > 100000 || lineNumber < 1 || it == statementMap.end())
    {
        return false;
    }
    return true;
}

string program::fetchProgramString()
{
    string ans;
    for(auto statement:statements)
    {
        ans += statement->getLineString() + '\n';
    }
    return ans;
}




void program::loadFromFile(QString filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox mesg;
        mesg.critical(mainWindow, "Error", "Failed to open program file");
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()){
        std::string line = in.readLine().toStdString();
        Statement* sta = creatStatement(line);
        if(sta == nullptr)
        {
            throw MyException("Invalid Statement",INVALIDSTATEMENT);
        }
        addStatement(sta);
    }
}

void program::addStatement(Statement *statement)
{
    statement->parseLineString();
    int num = statement -> getLineNumber();
    if(num > 1000000 || num < 1) {
        throw MyException("Invalid line number", INVALIDLINENUMBER);
    }
    if(statementMap.find(num) != statementMap.end())
    {
        statementMap[num] = statement;
        auto it = std::find(statements.begin(), statements.end(), statement);
        const Statement* temp = *it;
        *it = statement;
        delete temp;
    }
    else {
        auto it = std::lower_bound(statements.begin(), statements.end(), statement, [](const Statement *s, const Statement *t)->bool {
            return s->getLineNumber() < t->getLineNumber();
        });
        statements.insert(it, statement);
        statementMap[num] = statement;
    }
}

void program::deleteStatement(int lineNumber) {
    auto itt = statementMap.find(lineNumber);
    if(itt == statementMap.end())
        throw MyException("Non-existent line number", NONEXISTENTLINENUMBER);
    statementMap.erase(lineNumber);
    auto it = statements.begin();
    for(;it != statements.end(); it++)
    {
        if((*it) ->getLineNumber() == lineNumber)break;
    }
    statements.erase(it);
}

int program::runStatement(int lineNumber, int &resultOrArg) {
    if(lineNumber == -1)return -1;
    auto it = statementMap.find(lineNumber);
    if(it == statementMap.end())
        throw MyException("Non-existent line number", NONEXISTENTLINENUMBER);
    Statement* temp = it->second;
    ++it;
    int nextLineNumber = -1;
    if(it != statementMap.end())
        nextLineNumber = it -> first;
    resultOrArg =  temp -> executeStatement(nextLineNumber, evalSt, resultOrArg);
    return nextLineNumber;
}

void program::runProgram() {
    if(isDebugMode == 1 && !breakPoints.empty())
    {
        currentLine = breakPoints.front();
        runProgramFromTo(statements.front()->getLineNumber(), currentLine);
        return;
    }
    Statement* statement = statements.front();
    int lineNumber = statement->getLineNumber();
    while(lineNumber != -1)
    {
        if(statement->getType() == PRINT) {
            int result;
            lineNumber = runStatement(lineNumber, result);
            results.push_back(result);
        }
        else if(statement->getType() == INPUT) {
            ui->cmdLineEdit->setText("? ");
            askForArg();
            int arg = args.back();
            lineNumber = runStatement(lineNumber, arg);
        }
        else {
            int temp = 0;
            lineNumber = runStatement(lineNumber, temp);
        }
        if(statementMap.find(lineNumber) != statementMap.end())statement = statementMap[lineNumber];
        else if(lineNumber != -1)throw MyException("Non-existent line number", NONEXISTENTLINENUMBER);
    }
}

void program::runProgramFromTo(int fromLine, int toLine)
{
    if(statementMap.find(fromLine)==statementMap.end())return;
    Statement* statement = statementMap[fromLine];
    int lineNumber = statement->getLineNumber();

    if(statement->getType() == PRINT) {
        int result;
        lineNumber = runStatement(lineNumber, result);
        results.push_back(result);
    }
    else if(statement->getType() == INPUT) {
        ui->cmdLineEdit->setText("? ");
        askForArg();
        int arg = args.back();
        lineNumber = runStatement(lineNumber, arg);
    }
    else {
        int temp = 0;
        lineNumber = runStatement(lineNumber, temp);
    }
    if(statementMap.find(lineNumber) != statementMap.end())statement = statementMap[lineNumber];
    else if(lineNumber != -1)throw MyException("Non-existent line number", NONEXISTENTLINENUMBER);
    for(auto num : breakPoints)
    {
        if(num == lineNumber)
        {
            currentLine = num;
            return;
        }
    }
    while(1)
    {
        for(auto num : breakPoints)
        {
            if(num == lineNumber)
            {
                currentLine = num;
                return;
            }
        }
        if(statement->getType() == PRINT) {
            int result;
            lineNumber = runStatement(lineNumber, result);
            results.push_back(result);
        }
        else if(statement->getType() == INPUT) {
            ui->cmdLineEdit->setText("? ");
            askForArg();
            int arg = args.back();
            lineNumber = runStatement(lineNumber, arg);
        }
        else {
            int temp = 0;
            lineNumber = runStatement(lineNumber, temp);
        }
        if(statementMap.find(lineNumber) != statementMap.end())statement = statementMap[lineNumber];
        else if(lineNumber != -1)throw MyException("Non-existent line number", NONEXISTENTLINENUMBER);
        else return;
    }
}

void program::debugProgram() {
}

void program::clearProgramState() {
    evalSt->clear();
}

void program::cleanAll() {
    clearProgramState();
    statementMap.clear();
    for(auto element: statements) {
        delete element;
    }
    currentLine = -1;
    results.clear();
    args.clear();
}

void program::askForArg() {
    askState = 1;
    QEventLoop loop;
    QAction::connect(this, &program::inputCompleted, &loop, &QEventLoop::quit);
    loop.exec();
}

std::string program::programSyntaxTree() {
    std::string ans;
    for(const auto statement : statements) {
        ans += statement->syntaxTreeString();
        ans += "\n";
    }
    return ans;
}






