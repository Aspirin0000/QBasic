//
// Created by 26403 on 25-1-3.
//

#ifndef PROGRAM_H
#define PROGRAM_H
#include "statement.h"
#include <list>
#include <queue>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QMainWindow>
#include <QObject>


#include "evalstate.h"
#include "myexceptions.h"

#include "ui_mainwindow.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class program : public QObject
{
    Q_OBJECT
public:
    explicit program(QObject* parent = nullptr);

    ~program();
    void programInit(QMainWindow* ptr,Ui::MainWindow *u)  {
        mainWindow = ptr;
        ui = u;
        evalSt = new evalState;
        appendIndex = 1;
        askState = 0;
        isDebugMode = 0;
    }
    void loadFromFile(QString filePath);
    void addStatement(Statement* statement);
    void deleteStatement(int lineNumber);
    int runStatement(int lineNumber, int &resultOrArg );
    void runProgram();
    void runProgramFromTo(int fromLine, int toLine);
    void debugProgram();
    void clearProgramState();
    void cleanAll();
    void askForArg();
    std::string programSyntaxTree();
    evalState* getEvalState() { return evalSt; }
    Statement* creatStatement(std::string str);
    std::string fetchProgramString();
    bool checkLineNumber(int lineNumber);

    std::list<int> breakPoints;
    std::vector<int> results;
    std::vector<int> args;
    int appendIndex;
    int askState;
    int isDebugMode;
    int currentLine;
    evalState* evalSt;
private:
    QMainWindow* mainWindow;
    Ui::MainWindow *ui;

    std::list<Statement*> statements;

    map<int, Statement*> statementMap;
signals:
    void inputCompleted();

};



#endif //PROGRAM_H
