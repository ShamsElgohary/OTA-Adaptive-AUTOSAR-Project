#ifndef SM_TAB_H
#define SM_TAB_H

#include <QWidget>
#include<QLayout>

class sm_tab : public QWidget
{
    Q_OBJECT
public:
    explicit sm_tab(QWidget *parent = nullptr);
private:
    QHBoxLayout * main_layout = new QHBoxLayout;

signals:

};

#endif // SM_TAB_H