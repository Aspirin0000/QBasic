//
// Created by 26403 on 25-1-2.
//

#ifndef STATEMENT_H
#define STATEMENT_H



#include "expression.h"
#include "myexceptions.h"
#include "parser.h"
#include "evalstate.h"

enum StatementType  {
    REM, LET, PRINT, INPUT, GOTO, IF, END
};

class Statement {
public:
    Statement(std::string line = "") : lineString(line) {}
    virtual ~Statement() {}

    void setLineNumber(int line) { lineNumber = line; }
    int getLineNumber() const { return lineNumber; }
    void setLineString(std::string line) { lineString = line; }
    std::string getLineString() { return lineString; }
    bool operator==(const Statement& other) const {
        return this->getLineNumber() == other.getLineNumber();
    }

    virtual StatementType getType() = 0;
    virtual void parseLineString() = 0;
    virtual std::string syntaxTreeString() = 0;
    virtual int executeStatement(int& nextLineNumber, evalState* evalState, int optionalArgs) = 0;
private:
    int lineNumber{};
    std::string lineString;
};


class RemStmt: public Statement {
public:
    explicit RemStmt(std::string line = "") : Statement(line) {}
    ~RemStmt() override = default;
    StatementType getType() override {
        return StatementType::REM;
    }
    void parseLineString() override;
    std::string syntaxTreeString() override;
    int executeStatement(int& nextLineNumber, evalState* evalState, int optionalArgs) override;
private:
    std::string remString;
};


class LetStmt: public Statement {
public:
    explicit LetStmt(std::string line = "") : Statement(line) {}
    ~LetStmt() override {
        delete parser;
    }
    StatementType getType() override {
        return StatementType::LET;
    }
    void parseLineString() override;
    std::string syntaxTreeString() override;
    int executeStatement(int& nextLineNumber, evalState* evalState, int optionalArgs) override;

private:
    std::string leftVariant;
    Parser* parser;
};
class PrintStmt: public Statement {
public:
    explicit PrintStmt(std::string line = "") : Statement(line) {}
    ~PrintStmt() override {
        delete parser;
    }
    StatementType getType() override {
        return StatementType::PRINT;
    }
    void parseLineString() override;
    std::string syntaxTreeString() override;
    int executeStatement(int& nextLineNumber, evalState* evalState, int optionalArgs) override;
private:
    Parser* parser;
};
class InputStmt: public Statement {
public:
    explicit InputStmt(std::string line = "") : Statement(line) {}
    ~InputStmt() override = default;
    StatementType getType() override {
        return StatementType::INPUT;
    }
    void parseLineString() override;
    std::string syntaxTreeString() override {return "";}
    int executeStatement(int& nextLineNumber, evalState* evalState, int optionalArgs) override;
private:
    std::string leftVariant;
};
class GotoStmt: public Statement {
public:
    explicit GotoStmt(std::string line = "") : Statement(line) {}
    ~GotoStmt() override = default;
    StatementType getType() override {
        return StatementType::GOTO;
    }
    void parseLineString() override;
    std::string syntaxTreeString() override;
    int executeStatement(int& nextLineNumber, evalState* evalState, int optionalArgs) override;
private:
    int nextLineNum;
};



class IfStmt: public Statement {
public:
    explicit IfStmt(std::string line = "") : Statement(line) {}
    ~IfStmt() override {
        delete parser1;
        delete parser2;
    }
    StatementType getType() override {
        return StatementType::IF;
    }
    void parseLineString() override;
    std::string syntaxTreeString() override;
    int executeStatement(int& nextLineNumber, evalState* evalState, int optionalArgs) override;

private:
    string op;
    Parser* parser1, *parser2;
    int nextLineNum;
};



class EndStmt: public Statement {
public:
    explicit EndStmt(std::string line = "") : Statement(line) {}
    ~EndStmt() override = default;
    StatementType getType() override {
        return StatementType::END;
    }
    void parseLineString() override;
    std::string syntaxTreeString() override {return "";}
    int executeStatement(int& nextLineNumber, evalState* evalState, int optionalArgs) override {return 0;}
};



#endif //STATEMENT_H
