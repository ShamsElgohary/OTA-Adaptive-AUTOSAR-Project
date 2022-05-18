#ifndef SD_H
#define SD_H

#include <QWidget>
#include <QtWidgets>
 #include <QStringList>
#include "simulation.hpp"
class sd :public QWidget
{
     Q_OBJECT
public:
    explicit sd(QWidget *parent = nullptr);
    void sd_handler();
    void sm_handler();
public slots:
    void update_sd();
signals:
    void sd_signal();
private:
    QStringList *columnsheader=new QStringList;
    QTextBrowser *console_text = new QTextBrowser;
    QGroupBox *console = new QGroupBox("Console", this);
    QTableWidget *find=new QTableWidget;
    QTableWidget *offer=new QTableWidget;
     QTableWidget *table=new QTableWidget;
    QGridLayout *main_layout = new QGridLayout;
    QGroupBox *find_RequestsGBX = new QGroupBox("Find Service Requests", this);
    QGroupBox *Offered_ServicesGBX = new QGroupBox("Offered Services", this);
    simulation* sim=nullptr;
};


#endif // SD_H
