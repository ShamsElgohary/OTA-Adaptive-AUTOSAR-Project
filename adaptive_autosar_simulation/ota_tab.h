#ifndef OTA_TAB_H
#define OTA_TAB_H

#include <QWidget>
#include<QLayout>
#include"ota.h"

class ota_tab : public QWidget
{
    Q_OBJECT
public:
    explicit ota_tab(QWidget *parent = nullptr);
    ota * ota_instance;
private:
    QHBoxLayout * main_layout = new QHBoxLayout;

signals:

};

#endif // OTA_TAB_H
