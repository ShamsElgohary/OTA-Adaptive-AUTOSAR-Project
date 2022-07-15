#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../../utility/general.hpp"
#include "iostream"
#include <QKeyEvent>
#include "CarControlImpl.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QThread::create([this]()
    {
        CarControlImpl service(1);
        service.OfferService();
    })->start();
    QString icons_path=CUSTOMIZED_PROJECT_PATH.c_str()+QString("utility/Theme/forward.png");
    QPixmap pixmap(icons_path);
    QIcon ButtonIcon(pixmap);
    rightBtn->setIcon(ButtonIcon);
    QSize iconSize(pixmap.width()/2,pixmap.height()/2);
    rightBtn->setIconSize(iconSize);

    icons_path=CUSTOMIZED_PROJECT_PATH.c_str()+QString("utility/Theme/arrow_left.png");
    QPixmap pixmap4(icons_path);
    QIcon ButtonIcon4(pixmap4);
    leftBtn->setIcon(ButtonIcon4);
    QSize iconSize4(pixmap.width()/2,pixmap.height()/2);
    leftBtn->setIconSize(iconSize4);

    icons_path=CUSTOMIZED_PROJECT_PATH.c_str()+QString("utility/Theme/arrow_up.png");
    QPixmap pixmap3(icons_path);
    QIcon ButtonIcon3(pixmap3);
    forwarBtn->setIcon(ButtonIcon3);
    QSize iconSize3(pixmap.width()/2,pixmap.height()/2);
    forwarBtn->setIconSize(iconSize3);
    forwarBtn->show();

    icons_path=CUSTOMIZED_PROJECT_PATH.c_str()+QString("utility/Theme/arrow_down.png");
    QPixmap pixmap2(icons_path);
    QIcon ButtonIcon2(pixmap2);
    backwardBtn->setIcon(ButtonIcon2);
    QSize iconSize2(pixmap.width()/2,pixmap.height()/2);
    backwardBtn->setIconSize(iconSize2);

    icons_path=CUSTOMIZED_PROJECT_PATH.c_str()+QString("utility/Theme/start.png");
    QPixmap pixmap5(icons_path);
    QIcon ButtonIcon5(pixmap5);
    startengine->setIcon(ButtonIcon5);
    QSize iconSize5(pixmap.width()/2,pixmap.height()/2);
    startengine->setIconSize(iconSize5);

    icons_path=CUSTOMIZED_PROJECT_PATH.c_str()+QString("utility/Theme/stop.png");
    QPixmap pixmap6(icons_path);
    QIcon ButtonIcon6(pixmap6);
    stopengine->setIcon(ButtonIcon6);
    QSize iconSize6(pixmap.width()/2,pixmap.height()/2);
    stopengine->setIconSize(iconSize6);

    ui->setupUi(this);

    leftBtn->show();

    forwarBtn->show();

    backwardBtn->show();

    rightBtn->show();
    left->addWidget(leftBtn);
    middle->addWidget(forwarBtn);
    middle->addWidget(backwardBtn);
    right->addWidget(rightBtn);
    Control->setLayout(new QVBoxLayout);
    Control->layout()->addWidget(startengine);
    Control->layout()->addWidget(stopengine);

    main_layout->addLayout(left, 0, 0, 3, 1);
    main_layout->addLayout(middle, 0 ,1, 3, 1);
    main_layout->addLayout(right, 0, 2, 3, 1);
    console->setLayout(new QVBoxLayout());
    main_layout->addWidget(console, 3, 0, 2, 4);
    main_layout->addWidget(Control,0,3,3,1);
    console_text->setTextColor(QColor(Qt::cyan));
    console->layout()->addWidget(console_text);
    widget->setLayout(main_layout);
    setCentralWidget(widget);

    connect(leftBtn,SIGNAL(clicked()),this,SLOT(on_leftBtn_clicked()));
    connect(rightBtn,SIGNAL(clicked()),this,SLOT(on_rightBtn_clicked()));
    connect(forwarBtn,SIGNAL(clicked()),this,SLOT(on_forwardBtn_clicked()));
    connect(backwardBtn,SIGNAL(clicked()),this,SLOT(on_backwardBtn_clicked()));
    connect(stopengine,SIGNAL(clicked()),this,SLOT(on_stopEngine_clicked()));
    connect(startengine,SIGNAL(clicked()),this,SLOT(on_startEngine_clicked()));

    HideStop();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::writeCommand(const Command commandToSend)
{
    switch(commandToSend)
    {
    case Command::START:
        console_text->insertPlainText("[Command] Start Engine\n");
        std::cout << "[Command] Start Engine" << std::endl;
        break;
    case Command::STOP:
        console_text->insertPlainText("[Command] Stop Engine\n");
        std::cout << "[Command] Stop Engine" << std::endl;
        break;
    case Command::MOVE_FORWARD:
        console_text->insertPlainText("[Command] Move Forward\n");
        std::cout << "[Command] Move Forward" << std::endl;
        break;
    case Command::MOVE_BACKWARD:
        console_text->insertPlainText("[Command] Move Backward\n");
        std::cout << "[Command] Move Backward" << std::endl;
        break;
    case Command::MOVE_LEFT:
        console_text->insertPlainText("[Command] Turn Left\n");
        std::cout << "[Command] Turn Left" << std::endl;
        break;
    case Command::MOVE_RIGHT:
        console_text->insertPlainText("[Command] Turn Right\n");
        std::cout << "[Command] Turn Right" << std::endl;
        break;
    default:
        console_text->insertPlainText("[Command Erro] Unknown Command\n");
        std::cout << "[Command Erro] Unknown Command" << std::endl;
        break;
    }
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if(!engineStarted){
        return;
    }

    if(event->key() == Qt::Key_W)
    {
        on_forwardBtn_clicked();
    }

    if(event->key() == Qt::Key_S)
    {
        on_backwardBtn_clicked();
    }

    if(event->key() == Qt::Key_A)
    {
        on_leftBtn_clicked();
    }

    if(event->key() == Qt::Key_D)
    {
        on_rightBtn_clicked();
    }
}

void MainWindow::on_forwardBtn_clicked()
{
    writeCommand(Command::MOVE_FORWARD);
}

void MainWindow::on_backwardBtn_clicked()
{
    writeCommand(Command::MOVE_BACKWARD);
}

void MainWindow::on_leftBtn_clicked()
{
    writeCommand(Command::MOVE_LEFT);
}

void MainWindow::on_rightBtn_clicked()
{
    writeCommand(Command::MOVE_RIGHT);
}

void MainWindow::on_stopEngine_clicked()
{
    engineStarted = false;
    writeCommand(Command::STOP);
    HideStop();
}

void MainWindow::on_startEngine_clicked()
{
    engineStarted = true;
    writeCommand(Command::START);
    HideStart();
}

void MainWindow::HideStart()
{
    startengine->setVisible(false);
    stopengine->setVisible(true);
    forwarBtn->setVisible(true);
    backwardBtn->setVisible(true);
    rightBtn->setVisible(true);
    leftBtn->setVisible(true);
}

void MainWindow::HideStop()
{
    startengine->setVisible(true);
    stopengine->setVisible(false);
    forwarBtn->setVisible(false);
    backwardBtn->setVisible(false);
    rightBtn->setVisible(false);
    leftBtn->setVisible(false);
    console_text->clear();
}


void MainWindow::closeEvent(QCloseEvent *e)
{
    ara::exec::ExecutionClient exec;
    exec.ReportExecutionStaste(ara::exec::ExecutionState::Kterminate);
    e->accept();
}







