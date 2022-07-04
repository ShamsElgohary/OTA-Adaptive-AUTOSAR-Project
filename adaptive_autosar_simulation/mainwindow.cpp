#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <filesystem>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

   vertical_layout_tabs->addWidget(tabWidget);

   this->tabWidget->addTab(exec_tab,"Execution Manager");
   simulation_button->setText("start simulation");
   ota_button->setText("Update System");
   vertical_layout_control->addWidget(simulation_button);
   vertical_layout_control->addWidget(ota_button);
   end_simulation_button->setText("Restart simulation");
   vertical_layout_control->addWidget(end_simulation_button);
   ota_button->setVisible(false);

   main_layout->addLayout(vertical_layout_control);
   main_layout->addLayout(vertical_layout_tabs);

   widget->setLayout(main_layout);

   setCentralWidget(widget);
   s=new simulation(8088);
   create_server();
   connect_fun();
}
void MainWindow::create_server()
{
    s->create_server();
    QThread::create([this]{
        while(1)
        {
            auto new_socket = this->s->listen_l();
            cout<<"thread created"<<endl;
            QThread::create([this,new_socket]()
            {
                auto name = this->s->recive_exe_name(new_socket);
                open_tab(name);
                while(1)
                {
                    if(this->s->recive_file(new_socket,name)){
                        choose_handler(name);
                        usleep(500);
                    }
                    else
                    {
                        cout<<"socket closed"<<endl;
                        new_socket->close();
                        delete new_socket;
                        break;
                    }
                }
            })->start();
        }
    })->start();
}

void MainWindow::connect_fun()
{
    QSignalMapper* signalmapper=new QSignalMapper(this);

    connect(simulation_button, SIGNAL(clicked()), this, SLOT(on_simulation_button_clicked()));
    connect(ota_button, SIGNAL(clicked()), this, SLOT(on_ota_button_clicked()));
    connect(end_simulation_button, SIGNAL(clicked()), this, SLOT(end_simulation_button_clicked()));
    connect(this, SIGNAL(receive_cluster(simulation::exe_name)), this, SLOT(open_tab(simulation::exe_name)));

}
void MainWindow::on_simulation_button_clicked()
{
    simulation_running=true;
    if(flag==0)
    {
        SM_tab=new sm_tab();
        UCM_tab=new ucm_tab();
        OTA_tab=new ota_tab();
        iam_tab=new IAM();
        sd_tab=new sd();
        string path(CUSTOMIZED_PROJECT_PATH+"gui_em");
        mkfifo(path.c_str(),0777);
        process_id= fork();
        if (process_id == 0)
        {
            string path=CUSTOMIZED_PROJECT_PATH+"executables/em/bin/";
            chdir(path.c_str());
            execl("em", nullptr);
        }
        flag=1;
        simulation_button->setText("Load Configurations");
    }
    else if(flag==1)
    {
        QThread*th= QThread::create([this]{
        exec_tab->em_connect();
        });
        th->start();
        simulation_button->setText("Process SM Request");
    }
}
void MainWindow::choose_handler(simulation::exe_name name)
{
    switch(name)
    {
     case (simulation::exe_name::exec) :
        this->exec_tab->em_handler();
        break;
    case (simulation::exe_name::ucm) :
        UCM_tab->ucm_instance->ucm_handler();
        break;
    case (simulation::exe_name::ota) :
        OTA_tab->ota_instance->ota_handler();
        break;
    case (simulation::exe_name::sd) :
        sd_tab->sd_handler();
        break;
    case (simulation::exe_name::iam) :
        iam_tab->iam_handler();
        break;
    case (simulation::exe_name::sm) :
        SM_tab->sm_instance->sm_handler();
        break;
    }
}

void MainWindow::open_tab(simulation::exe_name name)
{
    switch(name)
    {
    case (simulation::exe_name::sm) :
        this->tabWidget->addTab(SM_tab,"State Manager");
        break;
    case (simulation::exe_name::ucm) :
        this->tabWidget->addTab(UCM_tab,"Update and Configuration Manager");
        ota_button->setVisible(true);
        break;
    case (simulation::exe_name::ota) :
        this->tabWidget->addTab(OTA_tab,"OTA");
        break;
    case (simulation::exe_name::sd) :
        this->tabWidget->addTab(sd_tab,"Service Discovery");
        break;
    case (simulation::exe_name::iam) :
        this->tabWidget->addTab(iam_tab,"Identity and Access Manager");
        break;
    }

}
void MainWindow::on_ota_button_clicked()
{
        QThread*th= QThread::create([this]{
        SM_tab->sm_instance->run_cluster(sm::clusters::OTA);
        SM_tab->sm_instance->sm_handler();
        });
        th->start();
        ota_button->setVisible(false);
}
void MainWindow::end_simulation_button_clicked()
{
    system("pkill -x em");
    system("pkill -x sm");
    system("pkill -x iam");
    system("pkill -x ota");
    system("pkill -x someip_sd");
    system("pkill -x ucm");
    system("pkill -x ex1");
    system("pkill -x ex2");
    tabWidget->clear();
    exec_tab->clear_widget();
    this->tabWidget->addTab(exec_tab,"Execution Manager");
    ota_button->setVisible(false);
    simulation_running=false;
    simulation_button->setText("start simulation");
    flag=0;
}
void MainWindow::closeEvent(QCloseEvent *e)
{
    end_simulation_button_clicked();
    e->accept();
}
MainWindow::~MainWindow()
{
    s->~simulation();

    delete ui;
    delete main_layout;
    delete vertical_layout_tabs;
    delete vertical_layout_control;
    delete com;
    delete widget;
    delete exec_tab;
    delete SM_tab;
    delete exec_tab;
    delete iam_tab;
    delete UCM_tab;
    delete sd_tab;
}
