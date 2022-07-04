#ifndef SM_H
#define SM_H

#include <QWidget>
#include <QtWidgets>
#include "simulation.hpp"
#include "./jsoncpp/header/json.h"
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <bits/stdc++.h>
#include "../utility/general.hpp"
#include"com.h"

class sm: public QWidget
{
    Q_OBJECT
public:
    explicit sm(QWidget *parent = nullptr,GUI_COMM*ptr = nullptr);
    //explicit sm(QWidget *parent = nullptr,GUI_COMM * ptr = nullptr);
    void sm_handler();
    enum clusters{OTA=1,UCM};
public slots:
    void update_sm();
    void update_terminal();
    void run_cluster(clusters cluster);
signals:
    void sm_signal();
private:

    QGridLayout *main_layout = new QGridLayout;
    QGroupBox *state_management = new QGroupBox("State Management", this);
    //QGroupBox *com = new QGroupBox("COM", this);

    QGroupBox *function_group_state = new QGroupBox("Function Group State", this);
    QGroupBox *console = new QGroupBox("Console", this);
    QGroupBox *slider = new QGroupBox("slider", this);
    QTextBrowser *console_text = new QTextBrowser;
    QLabel *label = new QLabel;
    QSlider *s_slider = new QSlider(Qt::Horizontal, this);
    QProgressBar *p_slider = new QProgressBar;
    QTableWidget *function_group_state_table = new QTableWidget;

    simulation* sim=nullptr;

    GUI_COMM * com_ptr;

    QString reportPath = QString::fromStdString(CUSTOMIZED_PROJECT_PATH) +"executables/em/bin/sm_Reporting.json";
    QString serviceInstanceManifestPath = QString::fromStdString(CUSTOMIZED_PROJECT_PATH) +"executables/sm/0.1/etc/service_manifest.json";






};

#endif // SM_H
