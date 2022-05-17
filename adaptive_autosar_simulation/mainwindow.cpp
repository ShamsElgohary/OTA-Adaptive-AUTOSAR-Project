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

    sm_tab = new sm();
    tabWidget ->addTab(sm_tab,"state manager");

    sd_tab= new sd();
    tabWidget ->addTab(sd_tab,"service_discovery");

    ota_tab = new ota();
    tabWidget ->addTab(ota_tab,"OTA");

    //sd_tab->update_sd();
    exec_tab->parse_exec_json();
    ucm_tab = new ucm();
    tabWidget ->addTab(ucm_tab,"UCM");


    /* thread to lister on socket*/

//    socket_thread=QThread::create([this]{
//        this->s->creat_socket();
//        std::function<void()>handler = [this](){this->choose_handler();};
//        this->s->listen_l(handler);
//        });
//    socket_thread->start();


}
void MainWindow::choose_handler()
{
    std::ifstream file_input("file2.json"); //path to be updated
        Json::Reader reader;
        Json::Value root;
        reader.parse(file_input, root);
        string x=root["Cluster_name"].asString();

        if(x=="sm_json")
            this->sm_tab->sm_handler();
}
MainWindow::~MainWindow()
{
    delete ui;
}
