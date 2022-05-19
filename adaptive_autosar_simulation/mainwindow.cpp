#include "mainwindow.h"
#include "./ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    s=new simulation(8088);

    vertical_layout_tabs->addWidget(tabWidget);

    tabWidget ->addTab(exec_tab,"Execution manager");

    simulation_button->setText("start simulation");
    vertical_layout_control->addWidget(simulation_button);

    main_layout->addLayout(vertical_layout_control);
    main_layout->addLayout(vertical_layout_tabs);

    widget->setLayout(main_layout);

    setCentralWidget(widget);

    connect_fun();


}
void MainWindow::connect_fun()
{
    connect(simulation_button,SIGNAL(clicked()),this,SLOT(on_simulation_button_clicked()));
}
void MainWindow::on_simulation_button_clicked()
{
    ucm_tab = new ucm();
    tabWidget ->addTab(ucm_tab,"Update and Configuration Manager");

    sm_tab = new sm();
    tabWidget ->addTab(sm_tab,"state manager");


    ota_tab = new ota();
    tabWidget ->addTab(ota_tab,"OTA");

    QThread::create([this]{
        this->s->creat_socket();

        while(1)
        {
            int new_socket =this->s->listen_l();
            cout<<"thread created"<<endl;
            QThread::create([this,new_socket]()
            {
                auto name = this->s->recive_exe_name(new_socket);
                while(1)
                {
                    bool file = this->s->recive_file(new_socket,name);
                    cout<<file<<endl;
                    if(file){
                        callHandler(name);
                        usleep(10);
                    }
                    else
                    {
                        break;
                    }

                }
            })->start();

        }
    })->start();
}
void MainWindow::callHandler(simulation::exe_name name)
{
    switch(name)
    {
        case (simulation::exe_name::exec) : exec_tab->em_handler();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
