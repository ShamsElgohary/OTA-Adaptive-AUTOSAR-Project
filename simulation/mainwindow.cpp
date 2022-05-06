#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"
#include "jsoncpp/header/json.h"
#include "map"

void MainWindow::handle_sm()
{
    // receive json file
    /*s->creat_socket();
    s->listen_l();
    s->recive_file();*/

    //parse json
    std::ifstream file_input("sm.json"); //path to be updated
    Json::Reader reader;
    Json::Value root;
    reader.parse(file_input, root);
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
MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(handle_sm()));
    s=new simulation(8088);
}
MainWindow::~MainWindow()
{
    delete ui;
}

