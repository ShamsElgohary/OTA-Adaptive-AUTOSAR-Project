#ifndef SM_H
#define SM_H

#include <QWidget>
#include <QtWidgets>
#include "simulation.hpp"
#include "./jsoncpp/header/json.h"

class sm: public QWidget
{
    Q_OBJECT
public:
    explicit sm(QWidget *parent = nullptr);
    void sm_handler();
public slots:
    void update_sm();
    void update_terminal();
signals:
    void sm_signal();
private:

    QGridLayout *main_layout = new QGridLayout;
    QGroupBox *state_management = new QGroupBox("State Management", this);
    QGroupBox *function_group_state = new QGroupBox("Function Group State", this);
    QGroupBox *console = new QGroupBox("Console", this);
    QGroupBox *slider = new QGroupBox("slider", this);
    QTextBrowser *console_text = new QTextBrowser;
    QLabel *label = new QLabel;
    QSlider *s_slider = new QSlider(Qt::Horizontal, this);
    QProgressBar *p_slider = new QProgressBar;
    QTableWidget *function_group_state_table = new QTableWidget;

    simulation* sim=nullptr;






};

#endif // SM_H
