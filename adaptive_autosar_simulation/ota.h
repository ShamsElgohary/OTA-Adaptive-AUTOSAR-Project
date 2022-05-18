#ifndef OTA_H
#define OTA_H


#include <QWidget>
#include <QtWidgets>
#include "simulation.hpp"
#include "./jsoncpp/header/json.h"

class ota: public QWidget
{
    Q_OBJECT
public:
    explicit ota(QWidget *parent = nullptr);
public slots:
    void update_ota();
    void ota_handler();

signals: void ota_signal();
private:

    QGridLayout *main_layout = new QGridLayout;
    QGroupBox *over_the_air = new QGroupBox("OTA", this);
    QGroupBox *ucm = new QGroupBox("UCM", this);
    QGroupBox *button_box = new QGroupBox;

    QGroupBox *meta_data = new QGroupBox("Meta Data", this);
    QTableWidget *meta_data_tabel = new QTableWidget;

    QGroupBox *download_data = new QGroupBox("Download Data", this);
    QTableWidget *download_data_tabel = new QTableWidget;

    QGroupBox *ucm_package = new QGroupBox;
    QTableWidget *ucm_package_tabel = new QTableWidget;

    QPushButton * update =new QPushButton;
    QPushButton * discard_update =new QPushButton;

    QTextBrowser *ucm_console_text = new QTextBrowser;


};
#endif // OTA_H
