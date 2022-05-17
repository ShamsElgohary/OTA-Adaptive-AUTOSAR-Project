#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "exec.h"
#include "sm.h"
#include "ucm.h"
#include "ota.h"
#include "iam.h"
#include "sd.h"
#include "QThread"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void connect_fun();
    void choose_handler();

private slots:
    void on_simulation_button_clicked();

private:
    Ui::MainWindow *ui;
    QHBoxLayout *main_layout =new QHBoxLayout ;
    QVBoxLayout *vertical_layout_tabs =new QVBoxLayout ;
    QVBoxLayout *vertical_layout_control=new QVBoxLayout  ;
    QWidget *widget =new QWidget ;
    exec * exec_tab =new exec ;
    sm * sm_tab =new sm ;
    iam*  iam_tab =new iam ;
    ucm* ucm_tab=new ucm  ;
    ota* ota_tab=new ota  ;
    sd* sd_tab=new sd;

    QTabWidget *tabWidget =new QTabWidget ;
    QPushButton * simulation_button =new QPushButton;

    simulation* s=nullptr;
    QThread* socket_thread=nullptr;

};
#endif // MAINWINDOW_H
