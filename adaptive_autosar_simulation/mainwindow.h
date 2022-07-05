#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "exec.h"
#include "sm.h"
#include "ucm.h"
#include "iam.h"
#include "sd.h"
#include "QThread"
#include "ota.h"
#include "simulation.hpp"
#include "sm_tab.h"
#include"ucm_tab.h"
#include"ota_tab.h"
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
    void choose_handler(simulation::exe_name name);

    void create_server();
    void closeEvent(QCloseEvent *e);
    void open_tab(simulation::exe_name name);

public slots:
    void on_simulation_button_clicked();
    void on_ota_button_clicked();
    void end_simulation_button_clicked();
    void add_sm();
    void add_ucm();
    void add_ota();
    void add_sd();
    void add_iam();
signals:
    void add_sm_s();
    void add_ucm_s();
    void add_ota_s();
    void add_iam_s();
    void add_sd_s();
private:
    Ui::MainWindow *ui;
    QHBoxLayout *main_layout =new QHBoxLayout ;
    QVBoxLayout *vertical_layout_tabs =new QVBoxLayout ;
    QVBoxLayout *vertical_layout_control=new QVBoxLayout  ;
    QHBoxLayout *com = new QHBoxLayout;
    QWidget *widget =new QWidget ;
    exec * exec_tab =new exec ;
    //sm * sm_tab =new sm ;
    sm_tab * SM_tab = new sm_tab;
    IAM*  iam_tab =new IAM ;
    //ucm* ucm_tab=new ucm  ;
    ucm_tab * UCM_tab = new ucm_tab;
    sd* sd_tab=new sd;
    //ota* ota_tab=new ota  ;
    ota_tab * OTA_tab = new ota_tab;
    
    QTabWidget *tabWidget =new QTabWidget ;
    QPushButton * simulation_button =new QPushButton;
    QPushButton * ota_button =new QPushButton;
    QPushButton * end_simulation_button =new QPushButton;
    
    simulation*s ;
    int process_id;
    bool simulation_running=false;
    int flag=0;
};
#endif // MAINWINDOW_H
