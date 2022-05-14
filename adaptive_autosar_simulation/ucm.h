#ifndef UCM_H
#define UCM_H

#include <QWidget>
#include <QtWidgets>

class ucm: public QWidget
{
    Q_OBJECT
public:
    explicit ucm(QWidget *parent = nullptr);



private:
     QGridLayout *main_layout = new QGridLayout;

     //UCMmain GridBox setup
     QGroupBox *ucmBox = new QGroupBox("UCM");
     QGroupBox *log = new QGroupBox("LOG");
     QGroupBox *mid = new QGroupBox("MID");
     QGroupBox *clusters = new QGroupBox("CLUSTERS");

     //MID Colume setup
     QGroupBox *status = new QGroupBox("STATUS");
     QGroupBox *newSwPkg = new QGroupBox("New Software Package Details");
     QGroupBox *activate = new QGroupBox("Activate Details");


     //Add lists to mid colume



    //COM GridBox setup
     QGroupBox *comBox = new QGroupBox("COM");
};

#endif // UCM_H
