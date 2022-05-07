#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"
#include "jsoncpp/header/json.h"
#include "map"
#include "QThread"
#include "functional"
void MainWindow::handle_sm()
{
    std::ifstream file_input("file2.json"); //path to be updated
    Json::Reader reader;
    Json::Value root;
    reader.parse(file_input, root);
    ui->progressBar->setValue(0);
    if(root["sm_json"]["UpdateRequest"]["StartUpdateSession"]==true)
    {
    ui->label_17->setText("StartUpdateSession");
    ui->progressBar->setValue(25);
    }
    if(root["sm_json"]["UpdateRequest"]["StopUpdateSession"]==true)
    {
    ui->label_17->setText("StopUpdateSession");
    ui->progressBar->setValue(100);
    }
    if(root["sm_json"]["UpdateRequest"]["VerifyUpdate"]==true)
    {
    ui->label_17->setText("VerifyUpdate");
    ui->progressBar->setValue(75);
    }
    if(root["sm_json"]["UpdateRequest"]["PrepareUpdate"]==true)
    {
    ui->label_17->setText("PrepareUpdate");
    ui->progressBar->setValue(50);
    }
    std::map<std::string,std::string>FG_states;
    Json::Value states=root["sm_json"]["function_group_states"];
    ui->tableWidget_4->setRowCount(0);
    int size=states.size();
    ui->tableWidget_4->setRowCount(size);
    ui->tableWidget_4->setColumnCount(2);
    int i=0;
    for(auto fg: states.getMemberNames())
    {
        ui->tableWidget_4->setItem(i,0,new QTableWidgetItem(QString(fg.c_str())));
        ui->tableWidget_4->setItem(i,1,new QTableWidgetItem(QString(states[fg].asCString())));
        i++;
    }
}
void MainWindow::choose_handler()
{
    std::ifstream file_input("file2.json"); //path to be updated
    Json::Reader reader;
    Json::Value root;
    reader.parse(file_input, root);
    string x=root["Cluster_name"].asString();
    if(x=="sm_json")
        this->handle_sm();
}
void MainWindow::sock_listen()
{
    sm_thread=QThread::create([this]{
    this->s->creat_socket();
    std::function<void()>handler = [this](){this->choose_handler();};
    this->s->listen_l(handler);
    });
    sm_thread->start();
}
MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(sock_listen()));
    s=new simulation(8088);
    //sm_thread=QThread::create([this]{handle_sm();});
}
MainWindow::~MainWindow()
{
    delete ui;
}

