#ifndef EXEC_H
#define EXEC_H

#include <QWidget>
#include <QtWidgets>
#include "jsoncpp/header/json.h"
#include <fstream>
using namespace std;
class exec : public QWidget
{
    Q_OBJECT
public:
    explicit exec(QWidget *parent = nullptr);
    void parse_exec_json();
    void update_exec();

signals:
private:
    struct exe_configuartions
    {
        string name ;
        string funtion_group ;
        vector<string> states ;
        map<string ,string>dependancy ;
    };
    struct running_exec
    {
        string name ;
        string current_state ;
        int pid ;
    };

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


    vector<exe_configuartions> exes_conf;
    map<string,vector<string>> fng ;
    vector<running_exec> r_exe;
    vector<string> to_run;
    vector<string> to_term;

};

#endif // EXEC_H
