#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    s = new simulation(8088);

    char path[] = "/home/youssef/Documents/OTA-Adaptive-AUTOSAR-Project/gui_sm";
    mkfifo(path, 0777);

    vertical_layout_tabs->addWidget(tabWidget);

    tabWidget->addTab(exec_tab, "Execution manager");

    simulation_button->setText("start simulation");
    vertical_layout_control->addWidget(simulation_button);

    ota_button->setText("OTA Run");
    vertical_layout_control->addWidget(ota_button);

    ucm_button->setText("UCM Run");
    vertical_layout_control->addWidget(ucm_button);
    //COMM GUI formation
    //    QGroupBox * gbox = new QGroupBox("COMM");
    //    com->addWidget(gbox);

    //    QVBoxLayout *l = new QVBoxLayout;

    //    QTreeWidget * tree = new QTreeWidget;
    //    tree->setColumnCount(1);
    //    l->layout()->addWidget(tree);
    //    gbox->setLayout(l);
    // GUI_COMM c(tree);
    //c.parse(path);

    main_layout->addLayout(vertical_layout_control);
    main_layout->addLayout(vertical_layout_tabs);

    widget->setLayout(main_layout);

    setCentralWidget(widget);

    connect_fun();
}
void MainWindow::connect_fun()
{
    connect(simulation_button, SIGNAL(clicked()), this, SLOT(on_simulation_button_clicked()));
    connect(ota_button, SIGNAL(clicked()), this, SLOT(on_ota_button_clicked()));
    connect(ucm_button, SIGNAL(clicked()), this, SLOT(on_ucm_button_clicked()));
}
void MainWindow::on_simulation_button_clicked()
{
    ucm_tab = new ucm();
    tabWidget->addTab(ucm_tab, "UCM");

    sm_tab = new sm();
    tabWidget->addTab(sm_tab, "state manager");

    sd_tab = new sd();
    tabWidget->addTab(sd_tab, "service_discovery");

    iam_tab = new IAM();
    tabWidget->addTab(iam_tab, "IAM");

    ota_tab = new ota();
    tabWidget->addTab(ota_tab, "OTA");

    //sd_tab->update_sd();
    exec_tab->parse_exec_json();
    /* thread to lister on socket*/

    socket_thread = QThread::create([this]
                                    {
                                        this->s->creat_socket();
                                        std::function<void()> handler = [this]()
                                        { this->choose_handler(); };
                                        this->s->listen_l(handler);
                                    });
    socket_thread->start();
}
void MainWindow::choose_handler()
{
    std::ifstream file_input("file2.json"); //path to be updated
    Json::Reader reader;
    Json::Value root;
    reader.parse(file_input, root);
    string x = root["Cluster_name"].asString();

    if (x == "sm_json")
        this->sm_tab->sm_handler();
    if (x == "ServiceDiscovery")
        this->sd_tab->sd_handler();
    if (x == "iam_json")
        this->iam_tab->iam_handler();
    if (x == "ucm")
        this->ucm_tab->ucm_handler();
    if (x == "ota")
        this->ota_tab->ota_handler();
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
