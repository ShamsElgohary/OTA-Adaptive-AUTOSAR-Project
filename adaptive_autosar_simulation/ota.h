#ifndef OTA_H
#define OTA_H


#include <QWidget>
#include <QtWidgets>
#include "simulation.hpp"
#include "./jsoncpp/header/json.h"
#include"com.h"
#include "../utility/general.hpp"

class ota: public QWidget
{
    Q_OBJECT
public:
    explicit ota(QWidget *parent = nullptr,GUI_COMM*ptr = nullptr);
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

    GUI_COMM * com_ptr;

    QString reportPath = QString::fromStdString(CUSTOMIZED_PROJECT_PATH) +"executables/em/bin/ota_Reporting.json";
    QString serviceInstanceManifestPath = QString::fromStdString(CUSTOMIZED_PROJECT_PATH) +"executables/ota/0.1/etc/service_manifest.json";


};
#endif // OTA_H
