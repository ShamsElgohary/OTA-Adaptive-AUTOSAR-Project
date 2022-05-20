#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <filesystem>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

   vertical_layout_tabs->addWidget(tabWidget);


   simulation_button->setText("start simulation");
   vertical_layout_control->addWidget(simulation_button);
   ucm_button->setText("UCM Run");
   vertical_layout_control->addWidget(ucm_button);

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
    connect(ucm_button, SIGNAL(clicked()), this, SLOT(on_ucm_button_clicked()));
}
void MainWindow::on_simulation_button_clicked()
{
//    system("./em");
//    chdir("/home/loay/Documents/GitHub/OTA-Adaptive-AUTOSAR-Project/build-adaptive_autosar_simulation-Desktop_Qt_5_15_2_GCC_64bit-Debug");
    int id = fork();
    if (id == 0)
    {
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
     case (simulation::exe_name::exec) :
        this->tabWidget->addTab(exec_tab,"Execution Manager");
        break;
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
    QThread *th = QThread::create([this]
                                  {
                                      sm_tab->run_cluster(sm::clusters::OTA);
                                      sm_tab->sm_handler();
                                  });
    th->start();
    ota_tab = new ota();
    tabWidget->addTab(ota_tab, "OTA");
}
void MainWindow::on_ucm_button_clicked()
{
    QThread *th = QThread::create([this]
          {
              sm_tab->run_cluster(sm::clusters::UCM);
              sm_tab->sm_handler();
          });
    th->start();
    ucm_tab = new ucm();
    tabWidget->addTab(ucm_tab, "UCM");
}
MainWindow::~MainWindow()
{
    delete ui;
}
