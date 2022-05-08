#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
}
MainWindow::~MainWindow()
{
    delete ui;
}
