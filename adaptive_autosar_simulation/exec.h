#ifndef EXEC_H
#define EXEC_H

#include <QWidget>
#include <QtWidgets>

class exec : public QWidget
{
    Q_OBJECT
public:
    explicit exec(QWidget *parent = nullptr);
    void update_exec();
signals:
private:
    QGridLayout *main_layout = new QGridLayout;
    QGroupBox *machine_configGbx = new QGroupBox("Machine Configuration");
    QGroupBox *exec_configGbx = new QGroupBox("Executables Configuration");
    QGroupBox *sm_requestsGbx = new QGroupBox("Sm Requests");
    QGroupBox *newtwork_console_Gbx = new QGroupBox("Network Console");
    QGroupBox * Running_executables = new QGroupBox("Running Executables");


    QTextBrowser *network_text = new QTextBrowser;
    QTreeWidget *machine_tree =new QTreeWidget();
    QListWidget *exec_list =new QListWidget();
    QListWidget *sm1_list =new QListWidget();
    QListWidget *sm2_list =new QListWidget();

    QListWidget *rexe_list =new QListWidget();

};

#endif // EXEC_H
