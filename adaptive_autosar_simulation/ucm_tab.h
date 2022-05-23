#ifndef UCM_TAB_H
#define UCM_TAB_H

#include <QWidget>
#include<QLayout>

class ucm_tab : public QWidget
{
    Q_OBJECT
public:
    explicit ucm_tab(QWidget *parent = nullptr);
private:
    QHBoxLayout * main_layout = new QHBoxLayout;

signals:

};

#endif // UCM_TAB_H
