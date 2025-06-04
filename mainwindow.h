#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "program.h"
#include <QEventLoop>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_cmdLineEdit_editingFinished();

    void on_btnLoadCode_clicked();

    void on_btnRunCode_clicked();


    void on_btnDebugResume_clicked();

public:
    void waitForInput() {
        QEventLoop loop;
        connect(this, &MainWindow::inputCompleted, &loop, &QEventLoop::quit);
        loop.exec();
    }

signals:
    void inputCompleted();

private:
    Ui::MainWindow *ui;
    program* prg;
    program* prgIm;
    void clearAllWindow();
    void setUIForDebugMode();
    void setUIExitDebugMode();

};
#endif // MAINWINDOW_H
