#include "iam.h"

void IAM::iam_handler()
{
    emit iam_signal();
}

IAM::IAM(QWidget *parent): QWidget{parent}
{
    AccesscontrollistGBX->setLayout(new QHBoxLayout());

    main_layout->addWidget(AccesscontrollistGBX, 0, 2, 3, 1);
    main_layout->addWidget(GantrequestsGBX, 0 ,1, 3, 1);


    AccesscontrollistGBX ->setLayout(new QHBoxLayout);
    AccesscontrollistGBX->layout()->addWidget(access);

    access->setRowCount(0);
    access->setColumnCount(5);
    QStringList columnsheader = {"Exe Name","Grant Type","PR Type","Service ID","Instance ID"};
    access->setHorizontalHeaderLabels(columnsheader);
    access->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    GantrequestsGBX->setLayout(new QHBoxLayout);
    GantrequestsGBX->layout()->addWidget(request);

    request->setRowCount(0);
    request->setColumnCount(7);
    QStringList columnsheader2 = {"Exe ID","Exe Name","Grant Type","PR Type","Service ID","Instance ID", "Grant Result"};
    request->setHorizontalHeaderLabels(columnsheader2);
    request->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    console->setLayout(new QVBoxLayout());
    main_layout->addWidget(console, 3, 0, 2, 3);

    console->layout()->addWidget(console_text);

    setLayout(main_layout);
    sim=new simulation(8088);
    connect(this,SIGNAL(iam_signal()),this,SLOT(PARSEIAM()));
}
void IAM::PARSEIAM()
{

    std::ifstream file_input("file2.json"); //path to be updated
    Json::Reader reader;
    Json::Value root;
    reader.parse(file_input, root);
    Json::Value processes=root["Processes"];
    Json::Value requests=root["Requests"];

    for(int i=access->rowCount()-1; i>=0; i--)
        access->removeRow(i);
    for(int i=request->rowCount()-1; i>=0; i--)
        request->removeRow(i);

    int pr=0;
    int rq=0;
    for(Json::Value::ArrayIndex i=0;i!=processes.size();i++)
    {
        access->insertRow(pr);
        std::string process_name = processes[i]["Process"].asString();
        std::string GrantType = processes[i]["info"]["GrantType"].asString();
        std::string Instance_ID = processes[i]["info"]["Instance_ID"].asString();
        std::string PR_Type = processes[i]["info"]["PR_Type"].asString();
        std::string Service_ID = processes[i]["info"]["Service_ID"].asString();
        //access->setItem(i,0,new QTableWidgetItem(QString(std::to_string(i).c_str())));
        access->setItem(i,0,new QTableWidgetItem(QString(process_name.c_str())));
        access->setItem(i,1,new QTableWidgetItem(QString(GrantType.c_str())));
        access->setItem(i,2,new QTableWidgetItem(QString(PR_Type.c_str())));
        access->setItem(i,3,new QTableWidgetItem(QString(Service_ID.c_str())));
        access->setItem(i,4,new QTableWidgetItem(QString(Instance_ID.c_str())));
        //offer->removeRow();
        pr++;
    }

    for(Json::Value::ArrayIndex i=0;i!=requests.size();i++)
    {
        request->insertRow(rq);
        std::string Grant_Result = requests[i]["Grant_Result"].asString();
        std::string Process_id = requests[i]["Process_id"].asString();
        std::string Process_name = requests[i]["Process_name"].asString();
        std::string GrantType = requests[i]["info"]["GrantType"].asString();
        std::string Instance_ID = requests[i]["info"]["Instance_ID"].asString();
        std::string PR_Type = requests[i]["info"]["PR_Type"].asString();
        std::string Service_ID = requests[i]["info"]["Service_ID"].asString();
        //request->setItem(i,0,new QTableWidgetItem(QString(std::to_string(i).c_str())));
        request->setItem(i,0,new QTableWidgetItem(QString(Process_id.c_str())));
        request->setItem(i,1,new QTableWidgetItem(QString(Process_name.c_str())));
        request->setItem(i,2,new QTableWidgetItem(QString(GrantType.c_str())));
        request->setItem(i,3,new QTableWidgetItem(QString(PR_Type.c_str())));
        request->setItem(i,4,new QTableWidgetItem(QString(Service_ID.c_str())));
        request->setItem(i,5,new QTableWidgetItem(QString(Instance_ID.c_str())));
        request->setItem(i,6,new QTableWidgetItem(QString(Grant_Result.c_str())));
        //offer->removeRow();
        rq++;
    }
}
