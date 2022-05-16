#ifndef OTA_H
#define OTA_H


#include <QWidget>
#include <QtWidgets>

class ota: public QWidget
{
    Q_OBJECT
public:
    explicit ota(QWidget *parent = nullptr);
signals:
private:

    QGridLayout *main_layout = new QGridLayout;
    QGroupBox *over_the_air = new QGroupBox("OTA", this);
    QGroupBox *ucm = new QGroupBox("UCM", this);
    QGroupBox *button_box = new QGroupBox;

    QGroupBox *meta_data = new QGroupBox;
    QPushButton * meta_data_button =new QPushButton;
    QTableWidget *meta_data_tabel = new QTableWidget;

    QGroupBox *ucm_package = new QGroupBox;
    QPushButton * ucm_package_button =new QPushButton;
    QTableWidget *ucm_package_tabel = new QTableWidget;

    QPushButton * update =new QPushButton;
    QPushButton * discard_update =new QPushButton;

    QTextBrowser *ucm_console_text = new QTextBrowser;


};
#endif // OTA_H
