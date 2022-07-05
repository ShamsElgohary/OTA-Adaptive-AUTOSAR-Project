#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "commands.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void writeCommand(const Command commandToSend);
    void keyPressEvent(QKeyEvent *event);
    void HideStart();
    void HideStop();
    ~MainWindow();

    QWidget *widget =new QWidget ;
    QGridLayout *main_layout = new QGridLayout;
    QTextBrowser *console_text = new QTextBrowser;
    QGroupBox *console = new QGroupBox("Console", this);
    QVBoxLayout *left =new QVBoxLayout ;
    QVBoxLayout *middle=new QVBoxLayout  ;
    QVBoxLayout *right=new QVBoxLayout  ;
    QPushButton * leftBtn =new QPushButton;
    QPushButton * forwarBtn =new QPushButton;
    QPushButton * backwardBtn =new QPushButton;
    QPushButton * rightBtn =new QPushButton;
    QPushButton * startengine =new QPushButton;
    QPushButton * stopengine =new QPushButton;
    QGroupBox *Control = new QGroupBox("Control", this);


signals:

private slots:

    void on_forwardBtn_clicked();

    void on_backwardBtn_clicked();

    void on_leftBtn_clicked();

    void on_rightBtn_clicked();

    void on_stopEngine_clicked();

    void on_startEngine_clicked();

private:
    Ui::MainWindow *ui;
    bool engineStarted = false;

};
#endif // MAINWINDOW_H
