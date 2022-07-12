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
   debug_button->setText("Start Debugging");
   vertical_layout_control->addWidget(simulation_button);
   vertical_layout_control->addWidget(debug_button);
   vertical_layout_control->addWidget(ota_button);
   end_simulation_button->setText("Restart simulation");
   vertical_layout_control->addWidget(end_simulation_button);
   ota_button->setVisible(false);

   main_layout->addLayout(vertical_layout_control);
   main_layout->addLayout(vertical_layout_tabs);

   widget->setLayout(main_layout);

   setCentralWidget(widget);
   s=new simulation(8088);
   connect_fun();
   create_server();
   string path(CUSTOMIZED_PROJECT_PATH+"gui_em");
   mkfifo(path.c_str(),0777);
   string path1(CUSTOMIZED_PROJECT_PATH+"gui_ota");
   mkfifo(path1.c_str(),0777);
   flag=0;
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
                        usleep(100);
                    }
                    else
                    {
                        cout<<"socket closed"<<endl;
                        cout<<name<<endl;
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
    connect(simulation_button, SIGNAL(clicked()), this, SLOT(on_simulation_button_clicked()));
    connect(ota_button, SIGNAL(clicked()), this, SLOT(on_ota_button_clicked()));
    connect(end_simulation_button, SIGNAL(clicked()), this, SLOT(end_simulation_button_clicked()));
    connect(debug_button, SIGNAL(clicked()), this, SLOT(on_debug_button_clicked()));
    connect(this, SIGNAL(add_sm_s()), this, SLOT(add_sm()));
    connect(this, SIGNAL(add_ucm_s()), this, SLOT(add_ucm()));
    connect(this, SIGNAL(add_ota_s()), this, SLOT(add_ota()));
    connect(this, SIGNAL(add_iam_s()), this, SLOT(add_iam()));
    connect(this, SIGNAL(add_sd_s()), this, SLOT(add_sd()));

}
void MainWindow::on_simulation_button_clicked()
{
    string command="cp "+CUSTOMIZED_PROJECT_PATH+"executables/etc/system/Process_List.json "+CUSTOMIZED_PROJECT_PATH+"executables/Process_List.json";
    system(command.c_str());

    simulation_button->setEnabled(false);
    debug_button->setVisible(false);
    simulation_running=true;
        SM_tab=new sm_tab();
        UCM_tab=new ucm_tab();
        OTA_tab=new ota_tab();
        iam_tab=new IAM();
        sd_tab=new sd();
        process_id= fork();
        if (process_id == 0)
        {
            string path=CUSTOMIZED_PROJECT_PATH+"executables/em/bin/";
            chdir(path.c_str());
            execl("em", nullptr);
        }
        debugging_mode=false;
        exec_tab->debugging_mode=this->debugging_mode;
        exec_tab->em_connect();

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
void MainWindow::add_sm()
{
    this->tabWidget->addTab(SM_tab,"State Manager");
}
void MainWindow::add_ucm()
{
    this->tabWidget->addTab(UCM_tab,"Update and Configuration Manager");
    ota_button->setVisible(true);
}

void MainWindow::add_ota()
{
    this->tabWidget->addTab(OTA_tab,"OTA");
}

void MainWindow::add_sd()
{
    this->tabWidget->addTab(sd_tab,"Service Discovery");
}

void MainWindow::add_iam()
{
    this->tabWidget->addTab(iam_tab,"Identity and Access Manager");
}

void MainWindow::open_tab(simulation::exe_name name)
{
    switch(name)
    {
        case (simulation::exe_name::sm) :
            emit add_sm_s();
            break;
        case (simulation::exe_name::ucm) :
             emit add_ucm_s();
            break;
        case (simulation::exe_name::ota) :
            emit add_ota_s();
            break;
        case (simulation::exe_name::sd) :
            emit add_sd_s();
            break;
        case (simulation::exe_name::iam) :
            emit add_iam_s();
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
extern bool ota_flag;
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
    system("pkill -x Car_GUI");

    //return original process list
    string command="mv "+CUSTOMIZED_PROJECT_PATH+"executables/Process_List.json "+CUSTOMIZED_PROJECT_PATH+"executables/etc/system";
    system(command.c_str());
    string backup_path=CUSTOMIZED_PROJECT_PATH+"executables/Backup";
    if(DirectoryExists(backup_path.c_str()))
    //if(true)
    {
    //remove updated gui
    command = "rm -r "+CUSTOMIZED_PROJECT_PATH+"executables/Car_GUI";
    system(command.c_str());

    //rename Backup
    command="mv "+CUSTOMIZED_PROJECT_PATH+"executables/Backup "+CUSTOMIZED_PROJECT_PATH+"executables/Car_GUI";
    system(command.c_str());
    }
    tabWidget->clear();
    exec_tab->clear_widget();
    this->tabWidget->addTab(exec_tab,"Execution Manager");
    ota_button->setVisible(false);
    simulation_running=false;
    simulation_button->setText("start simulation");
    flag=0;
    simulation_button->setEnabled(true);
    simulation_button->setVisible(true);
    debug_button->setVisible(true);
    debug_button->setText("Start Debugging");
    string path(CUSTOMIZED_PROJECT_PATH+"gui_em");
    remove(path.c_str());
    flag=0;
    ota_flag=true;
    debugging_mode=false;

}
void MainWindow::on_debug_button_clicked()
{
        string command="cp "+CUSTOMIZED_PROJECT_PATH+"executables/etc/system/Process_List.json "+CUSTOMIZED_PROJECT_PATH+"executables/Process_List.json";
        system(command.c_str());
        simulation_button->setVisible(false);
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
            this->debugging_mode=true;
            exec_tab->debugging_mode=this->debugging_mode;
            exec_tab->em_connect();
            debug_button->setText("Load Configurations");
        }
        else if(flag==1)
        {
            QThread*th= QThread::create([this]{
            exec_tab->em_connect();
            });
            th->start();
            debug_button->setText("Process SM Request");
        }


}
bool MainWindow::DirectoryExists( const char* pzPath )
{
    if ( pzPath == NULL) return false;

    DIR *pDir;
    bool bExists = false;

    pDir = opendir (pzPath);

    if (pDir != NULL)
    {
        bExists = true;
        (void) closedir (pDir);
    }

    return bExists;
}
void MainWindow::closeEvent(QCloseEvent *e)
{
    end_simulation_button_clicked();
    e->accept();
}

MainWindow::~MainWindow()
{
    delete ui;
}
