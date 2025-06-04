#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <regex>


std::regex inputRegex(R"(^\s*INPUT\s+(\w+)\s*$)");
std::regex letRegex(R"(^\s*LET\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*(.*)\s*$)");
std::regex printRegex(R"(^\s*PRINT\s+(.*)\s*$)");
std::regex runRegex(R"(^\s*RUN\s*$)");
std::regex loadRegex(R"(^\s*LOAD\s*$)");
std::regex listRegex(R"(^\s*LIST\s*$)");
std::regex clearRegex(R"(^\s*CLEAR\s*$)");
std::regex clearoRegex(R"(^\s*CLEAROUTPUT\s*$)");
std::regex helpRegex(R"(^\s*HELP\s*$)");
std::regex quitRegex(R"(^\s*QUIT\s*$)");
std::regex deleteRegex(R"(^\s*(\d+)\s*$)");

std::regex addBreakRegex(R"(^\s*ADD\s+(\d+)\s*$)");
std::regex delBreakRegex(R"(^\s*DELETE\s+(\d+)\s*$)");
std::smatch match;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    prg = nullptr;
    prgIm = nullptr;
    ui->setupUi(this);
    setUIExitDebugMode();
    connect(ui->btnClearCode, &QPushButton::clicked, this, &MainWindow::clearAllWindow);
    connect(ui->btnDebugMode, &QPushButton::clicked, this, &MainWindow::setUIForDebugMode);
    connect(ui->btnExitDebugMode, &QPushButton::clicked, this, &MainWindow::setUIExitDebugMode);

    connect(ui->cmdLineEdit, SIGNAL(returnPressed()), this, SLOT(on_cmdLineEdit_editingFinished()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete prg;
    delete prgIm;
}

void MainWindow::on_cmdLineEdit_editingFinished()
{
    string cmd = ui->cmdLineEdit->text().toStdString();
    if(cmd == "")return;
    ui->cmdLineEdit->setText("");
    if(cmd[0] == '?')
    {
        int arg = stoi(cmd.substr(2));
        if(prgIm != nullptr && prgIm->askState == 1)
        {
            prgIm->args.push_back(arg);
            prgIm->askState = 0;
            emit prgIm->inputCompleted();
        }
        else if(prg != nullptr && prg->askState == 1)
        {
            prg->args.push_back(arg);
            prg->askState = 0;
            emit prg->inputCompleted();
        }
        return;
    }
    if(prg != nullptr && prg->isDebugMode == 1)
    {
        if(std::regex_match(cmd, match, addBreakRegex))
        {
            int lineNumber = stoi(match[1].str());
            if(prg->checkLineNumber(lineNumber) == false)
            {
                QMessageBox::critical(this, "Error",
                                     "Invalid Line Number",
                                      QMessageBox::Ok);
                return;
            }
            int flag = 0;
            for (auto it = prg->breakPoints.begin(); it != prg->breakPoints.end(); ++it) {
                if (*it > lineNumber) {
                    prg->breakPoints.insert(it, lineNumber);
                    flag = 1;
                    break;
                }
            }
            if(flag == 0)prg->breakPoints.push_back(lineNumber);
            ui->breakPointsDisplay->setText("");
            for(auto num : prg->breakPoints)
            {
                ui->breakPointsDisplay->append(QString::fromStdString(std::to_string(num)));
            }
        }
        else if(std::regex_match(cmd, match, delBreakRegex))
        {
            int lineNumber = stoi(match[1].str());
            if(prg->checkLineNumber(lineNumber) == false)
            {
                QMessageBox::critical(this, "Error",
                                      "Invalid Line Number",
                                      QMessageBox::Ok);
                return;
            }
            int flag = 0;
            for (auto it = prg->breakPoints.begin(); it != prg->breakPoints.end(); ++it) {
                if (*it == lineNumber) {
                    prg->breakPoints.erase(it);
                    flag = 1;
                    break;
                }
            }
            if(flag == 0)
            {
                QMessageBox::critical(this, "Error",
                                      "Invalid Line Number",
                                      QMessageBox::Ok);
                return;
            }
            ui->breakPointsDisplay->setText("");
            for(auto num : prg->breakPoints)
            {
                ui->breakPointsDisplay->append(QString::fromStdString(std::to_string(num)));
            }
        }
    }
    if(std::regex_match(cmd, match, inputRegex) && prg->isDebugMode == 0)
    {
        if(prgIm == nullptr)
        {
            prgIm = new program;
            prgIm->programInit(this,ui);
        }
        cmd = std::to_string(prgIm->appendIndex) + " " + cmd;
        prgIm->addStatement(prgIm->creatStatement(cmd));
        ui->cmdLineEdit->setText("? ");
        prgIm->askForArg();
        prgIm->runStatement(prgIm->appendIndex, prgIm->args.back());
        prgIm->appendIndex++;
    }
    else if(std::regex_match(cmd, match, letRegex) && prg->isDebugMode == 0)
    {
        if(prgIm == nullptr)
        {
            prgIm = new program;
            prgIm->programInit(this,ui);
        }
        cmd = std::to_string(prgIm->appendIndex) + " " + cmd;
        prgIm->addStatement(prgIm->creatStatement(cmd));
        int temp = 0;
        prgIm->runStatement(prgIm->appendIndex, temp);
        prgIm->appendIndex++;
    }
    else if(std::regex_match(cmd, match, printRegex) && prg->isDebugMode == 0)
    {
        if(prgIm == nullptr)
        {
            prgIm = new program;
            prgIm->programInit(this,ui);
        }
        cmd = std::to_string(prgIm->appendIndex) + " " + cmd;
        prgIm->addStatement(prgIm->creatStatement(cmd));
        int temp = 0;
        prgIm->runStatement(prgIm->appendIndex, temp);
        prgIm->results.push_back(temp);
        ui -> OutputDisplay->append(QString::fromStdString(std::to_string(prgIm->results.back())));
        prgIm->appendIndex++;
    }
    else if(std::regex_match(cmd, match, runRegex))
    {
        on_btnRunCode_clicked();
    }
    else if(std::regex_match(cmd, match, loadRegex) && prg->isDebugMode == 0)
    {
        on_btnLoadCode_clicked();
    }
    else if(std::regex_match(cmd, match, listRegex))
    {

    }
    else if(std::regex_match(cmd, match, clearRegex) && prg->isDebugMode == 0)
    {
        clearAllWindow();
    }
    else if(std::regex_match(cmd, match, clearoRegex))
    {
        ui -> OutputDisplay->clear();
    }
    else if(std::regex_match(cmd, match, helpRegex))
    {
        QMessageBox mesg;
        mesg.information(this, "Help",
        "RUN    This command starts program execution beginning at the lowest-numbered line. Unless the flow is changed by GOTO and IF commands, statements are executed in line-number order. Execution ends when the program hits the END statement or continues past the last statement in the program. \nLOAD   This command loads a file containing statements and commands. Statements and commands should be stored (also displayed in GUI) and executed respectively, as if they were entered into input box in order. A prompt window should be displayed when this command is entered. The window asks users to choose the file to load.\nCLEAR   This command deletes the program so the user can start entering a new one.\n"
                         , QMessageBox::Ok);

        mesg.information(this, "Help",
"REM This statement is used for comments. Any text on the line after the keyword REM is ignored. \nLET	This statement is BASIC’s assignment statement. The LET keyword is followed by a variable name, an equal sign, and an expression. As in C++, the effect of this statement is to assign the value of the expression to the variable, replacing any previous value. In BASIC, assignment is not an operator and may not be nested inside other expressions. \nPRINT In minimal BASIC, the PRINT statement has the form: \n	PRINT exp\nwhere exp is an expression. The effect of this statement is to print the value of the expression on the console and then print a newline character so that the output from the next PRINT statement begins on a new line.\nINPUT In the minimal version of the BASIC interpreter, the INPUT statement has the form: \n	INPUT var\nwhere var is a variable read in from the user. The effect of this statement is to print a prompt consisting of the string \" ? \" and then to read in a value to be stored in the variable. (The string \" ? \" should display in the command input edit box in GUI.)\nGOTO This statement has the syntax\n	GOTO n\nand forces an unconditional change in the control flow of the program. When the program hits this statement, the program continues from line n instead of continuing with the next statement. Your program should report an error if line n does not exist.\nIF	This statement provides conditional control. The syntax for this statement is:\n	IF exp1 op exp2 THEN n \nwhere exp1 and exp2 are expressions and op is one of the conditional operators=, <, or >. If the condition holds, the program should continue from line n just as in the GOTO statement. If not, the program continues on to the next line.Note that the conditional operators (=, <, >) are not parts of expressions.\nEND	Marks the end of the program. Execution halts when this line is reached. This statement is usually optional in BASIC programs because execution also stops if the program continues past the last numbered line."
                         , QMessageBox::Ok);
    }
    else if(std::regex_match(cmd, match, quitRegex))
    {
        this->close();
    }
    else if(std::regex_match(cmd, match, deleteRegex) && prg->isDebugMode == 0)
    {
        if(prg == nullptr)
        {
            QMessageBox::critical(this, "Error",
                                  "No Program NOW! ",
                                  QMessageBox::Ok);
        }
        try {
             if(prg != nullptr)prg->deleteStatement(stoi(match[1].str()));
        } catch (MyException& e) {
            QMessageBox::critical(this, "Error",
                                  (std::string(e.what()) +  "\nCode :" +std::to_string(e.code())).c_str(),
                                  QMessageBox::Ok);
        }
        if(prg != nullptr)ui -> CodeDisplay ->setText(QString::fromStdString(prg->fetchProgramString()));
    }
    else
    {
        if(prg == nullptr)
        {
            prg = new program;
            prg->programInit(this,ui);
        }
        if(prg->isDebugMode != 0)return;
        Statement* statement = prg -> creatStatement(cmd);
        if(statement == nullptr)
        {
            QMessageBox::critical(this, "Error",
                                 "Invalid Command, Failed to Parse",
                                  QMessageBox::Ok);
        }
        else
        {
            if(prg == nullptr)
            {
                prg = new program;
                prg->programInit(this,ui);
            }
            prg -> addStatement(statement);
        }
        if(prg != nullptr)ui -> CodeDisplay ->setText(QString::fromStdString(prg->fetchProgramString()));
    }
}

void MainWindow::clearAllWindow()
{
    ui->CodeDisplay->clear();
    ui->monitorDisplay->clear();
    ui->treeDisplay->clear();
    delete prg;
    prg = nullptr;
}

void MainWindow::setUIForDebugMode(){
    ui->btnClearCode->setVisible(false);
    ui->btnLoadCode->setVisible(false);
    ui->btnDebugMode->setVisible(false);

    ui->btnExitDebugMode->setVisible(true);
    ui->btnDebugResume->setVisible(true);

    ui->labelSyntaxTree->setVisible(false);
    ui->treeDisplay->setVisible(false);

    ui->labelMonitor->setVisible(true);
    ui->monitorDisplay->setVisible(true);
    ui->labelBreakPoints->setVisible(true);
    ui->breakPointsDisplay->setVisible(true);
    ui -> OutputDisplay->clear();
    if(prg==nullptr)return;
    prg->isDebugMode = 1;
    prg->clearProgramState();
}
void MainWindow::setUIExitDebugMode(){
    ui->btnClearCode->setVisible(true);
    ui->btnLoadCode->setVisible(true);
    ui->btnDebugMode->setVisible(true);

    ui->btnExitDebugMode->setVisible(false);
    ui->btnDebugResume->setVisible(false);

    ui->labelSyntaxTree->setVisible(true);
    ui->treeDisplay->setVisible(true);

    ui->labelMonitor->setVisible(false);
    ui->monitorDisplay->setVisible(false);
    ui->labelBreakPoints->setVisible(false);
    ui->breakPointsDisplay->setVisible(false);
    ui -> OutputDisplay->clear();
    if(prg == nullptr)return;
    prg->isDebugMode = 0;
    prg->breakPoints.clear();
    prg->clearProgramState();
}

void MainWindow::on_btnLoadCode_clicked()
{
    QString file = QFileDialog::getOpenFileName(
             this, tr("open program file"),
             "./", tr("All files (*.*)"));

    if(file.isEmpty())
    {
         QMessageBox mesg;
         mesg.warning(this,"Warning","Failed to load program");
         return;
    }
    delete prg;
    prg = new program;
    prg->programInit(this,ui);
    try{
        prg -> loadFromFile(file);
    }
    catch(MyException &e)
    {
        QMessageBox::critical(this, "Error",
                              (std::string(e.what()) +  "\nCode :" +std::to_string(e.code())).c_str(),
                              QMessageBox::Ok);
    }
    ui -> CodeDisplay ->setText(QString::fromStdString(prg->fetchProgramString()));
}


void MainWindow::on_btnRunCode_clicked()
{
    if(prg == nullptr)return;
    ui -> treeDisplay -> setText(QString::fromStdString(prg->programSyntaxTree()));
    try {
        prg->runProgram();
    } catch (MyException& e) {
        QMessageBox::critical(this, "Error",
                              (std::string(e.what()) +  "\nCode :" +std::to_string(e.code())).c_str(),
                              QMessageBox::Ok);
    }
    string result;
    for(size_t i = 0; i < prg->results.size();i ++)
    {
        if(i != prg->results.size() - 1)result += std::to_string(prg->results[i]) + '\n';
        else result += std::to_string(prg->results[i]);
    }
    ui -> OutputDisplay->append(QString::fromStdString(result));
    if(prg->isDebugMode == 0)prg->clearProgramState();
    else
    {
        ui->monitorDisplay->setText("");
        for(auto it : prg->evalSt->getEvaluationContext()->symbolTable)
        {
            ui->monitorDisplay->append(QString::fromStdString(it.first + " = " + std::to_string(it.second)));
        }
    }
}

void MainWindow::on_btnDebugResume_clicked()
{
    int curLine = prg ->currentLine;
    int nextLine = -1;
    for(auto it = prg->breakPoints.begin();it != prg->breakPoints.end();it++)
    {
        if(*it > curLine)
        {
            nextLine = curLine;
            break;
        }
    }
    prg->currentLine = nextLine;
    prg->runProgramFromTo(curLine,prg->currentLine);
    string result;
    for(size_t i = 0; i < prg->results.size();i ++)
    {
        if(i != prg->results.size() - 1)result += std::to_string(prg->results[i]) + '\n';
        else result += std::to_string(prg->results[i]);
    }
    ui -> OutputDisplay->setText(QString::fromStdString(result));
    ui->monitorDisplay->setText("");
    for(auto it : prg->evalSt->getEvaluationContext()->symbolTable)
    {
        ui->monitorDisplay->append(QString::fromStdString(it.first + " = " + std::to_string(it.second)));
    }
}


