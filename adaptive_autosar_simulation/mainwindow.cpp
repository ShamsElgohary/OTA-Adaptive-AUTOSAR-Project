#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <filesystem>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

   vertical_layout_tabs->addWidget(tabWidget);

   this->tabWidget->addTab(exec_tab,"Execution Manager");
   simulation_button->setText("start simulation");
   ota_button->setText("Run OTA");
   vertical_layout_control->addWidget(simulation_button);
   vertical_layout_control->addWidget(ota_button);
   end_simulation_button->setText("End simulation");
   vertical_layout_control->addWidget(end_simulation_button);
   ota_button->setVisible(false);

   main_layout->addLayout(vertical_layout_control);
   main_layout->addLayout(vertical_layout_tabs);

   widget->setLayout(main_layout);

   setCentralWidget(widget);

   create_server();
   connect_fun();
}
void MainWindow::create_server()
{
    QThread::create([this]{
        this->s->creat_socket();
        while(1)
        {
            int new_socket =this->s->listen_l();
            cout<<"thread created"<<endl;
            QThread::create([this,new_socket]()
            {
                auto name = this->s->recive_exe_name(new_socket);
                open_tab(name);
                auto handler = [name,this](){choose_handler(name);};
                while(1)
                {
                    if(this->s->recive_file(new_socket,name)){
                        handler();
                        usleep(10);
                    }
                    else break;
                }
            })->start();
        }
    })->start();
}

void MainWindow::connect_fun()
{
    connect(simulation_button, SIGNAL(clicked()), this, SLOT(on_simulation_button_clicked()));
    connect(ota_button, SIGNAL(clicked()), this, SLOT(on_ota_button_clicked()));
    connect(end_simulation_button, SIGNAL(clicked()), this, SLOT(on_ucm_button_clicked()));
}
void MainWindow::on_simulation_button_clicked()
{
//    system("./em");
//    chdir("/home/loay/Documents/GitHub/OTA-Adaptive-AUTOSAR-Project/build-adaptive_autosar_simulation-Desktop_Qt_5_15_2_GCC_64bit-Debug");
    simulation_running=true;
    process_id= fork();
    if (process_id == 0)
    {
        //chdir("/home/shams/GitHub/OTA-Adaptive-AUTOSAR-Project/executables/em/bin/");
        chdir("/home/loay/Documents/GitHub/OTA-Adaptive-AUTOSAR-Project/executables/em/bin/");
        execl("em", nullptr);
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
        ucm_tab->ucm_handler();
        ota_button->setVisible(true);
        break;
    case (simulation::exe_name::ota) :
        ota_tab->ota_handler();
        break;
    case (simulation::exe_name::sd) :
        sd_tab->sd_handler();
        break;
    case (simulation::exe_name::iam) :
        iam_tab->iam_handler();
        break;
    case (simulation::exe_name::sm) :
        sm_tab->sm_handler();
        break;
    }
}

void MainWindow::open_tab(simulation::exe_name name)
{
    switch(name)
    {
    case (simulation::exe_name::sm) :
        this->tabWidget->addTab(sm_tab,"State Manager");
        break;
    case (simulation::exe_name::ucm) :
        this->tabWidget->addTab(ucm_tab,"Update and Configuration Manager");
        break;
    case (simulation::exe_name::ota) :
        this->tabWidget->addTab(ota_tab,"OTA");
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
    //static int count;
    int count=1;
    /*if(count==0)
    {
        QThread*th= QThread::create([this]{
        sm_tab->run_cluster(sm::clusters::UCM);
        sm_tab->sm_handler();
        });
        th->start();
        count++;
        ota_button->setText("Run OTA");
    }*/
    if(count==1)
    {
        QThread*th= QThread::create([this]{
        sm_tab->run_cluster(sm::clusters::OTA);
        sm_tab->sm_handler();
        });
        th->start();
        count=0;
        ota_button->setText("Run OTA");
    }
}
void MainWindow::on_ucm_button_clicked()
{
    system("pkill -x em");
    system("pkill -x sm");
    system("pkill -x iam");
    system("pkill -x ota");
    system("pkill -x someip_sd");
    system("pkill -x ucm");
    system("pkill -x ex1");
    system("pkill -x ex2");
    while(tabWidget->count()!=1)
    {
    tabWidget->removeTab(1);
    }
    if(tabWidget->count()==1)
    {
        exec_tab->clear_widget();
        ota_button->setVisible(false);
        simulation_running=false;
    }
}
void MainWindow::closeEvent(QCloseEvent *e)
{
    s->~simulation();
    if(simulation_running)
    {
        e->ignore();
        QMessageBox msgbox;
        msgbox.setText("Please Stop Simlation Before Exit");
        msgbox.exec();
    }
    else
        e->accept();
}
MainWindow::~MainWindow()
{
    s->~simulation();
    system("pkill -f adaptive_autosar_simulation");
    delete ui;
}
