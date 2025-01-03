#ifndef EXEC_H
#define EXEC_H

#include <QWidget>
#include <QtWidgets>
#include "jsoncpp/header/json.h"
#include <fstream>
#include <iostream>
using namespace std;
class exec : public QWidget
{
    Q_OBJECT
public:
    explicit exec(QWidget *parent = nullptr);
    void parse_exec_json();
    void update_machine_confg();
    void update_executables_confg();
    void update_running_executables();
    void update_network_consle();
    void update_sm_requests();
    void update_fng_states();
    void em_handler();
    void clear_widget();
    void em_connect();
    int fd;
    bool debugging_mode;
public slots:
    void update_exec();
signals:
    void em_signal();
private:
    struct exe_configuartions
    {
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
    QGroupBox * fng_states_gbx = new QGroupBox("Function group states");


    QTextBrowser *network_text = new QTextBrowser;

    QTreeWidget *machine_tree =new QTreeWidget();
    QTreeWidget *exec_tree =new QTreeWidget();

    QTableWidget *sm1_list =new QTableWidget();
    QTreeWidget *sm2_list =new QTreeWidget();

    QTableWidget *rexe_table =new QTableWidget();
    QTableWidget *fng_states =new QTableWidget();

    map<string , vector<exe_configuartions>> exes_conf;
    map<string,vector<string>> fng ;
    map<string,string> fng_state ;

    vector<running_exec> r_exe;
    vector<string> to_run;
    vector<string> to_term;
    map<string , string> sm_request;
    string msg ;

};

#endif // EXEC_H
