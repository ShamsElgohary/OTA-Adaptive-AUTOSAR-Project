#ifndef SM_TAB_H
#define SM_TAB_H

#include <QWidget>
#include<QLayout>
#include"sm.h"

class sm_tab : public QWidget
{
    Q_OBJECT
public:
    explicit sm_tab(QWidget *parent = nullptr);
    sm *sm_instance;
    GUI_COMM *com_t = new GUI_COMM;
private:
    QHBoxLayout * main_layout = new QHBoxLayout;

signals:

};

#endif // SM_TAB_H
