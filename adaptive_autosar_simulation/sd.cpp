#include "sd.h"
#include "./jsoncpp/header/json.h"
#include "fstream"
#include <QTableWidget>
#include "iostream"

sd::sd(QWidget *parent): QWidget{parent}
{
    find_RequestsGBX->setLayout(new QHBoxLayout());

    main_layout->addWidget(find_RequestsGBX, 0, 2, 3, 1);
    main_layout->addWidget(Offered_ServicesGBX, 0 ,1, 3, 1);


    find_RequestsGBX ->setLayout(new QHBoxLayout);
    find_RequestsGBX->layout()->addWidget(find);

    find->setRowCount(0);
    find->setColumnCount(2);
    QStringList columnsheader = { "Service ID", "Instance ID"};
    find->setHorizontalHeaderLabels(columnsheader);
    find->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    Offered_ServicesGBX->setLayout(new QHBoxLayout);
    Offered_ServicesGBX->layout()->addWidget(offer);

    offer->setRowCount(0);
    offer->setColumnCount(2);
    offer->setHorizontalHeaderLabels(columnsheader);
    offer->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    console->setLayout(new QVBoxLayout());
    main_layout->addWidget(console, 3, 0, 2, 3);

    console->layout()->addWidget(console_text);

    setLayout(main_layout);
}
void sd::update_sd()
{
    std::ifstream file_input("sd.json"); //path to be updated
    Json::Reader reader;
    Json::Value root;
    reader.parse(file_input, root);
    Json::Value Received_SD_messages=root["SD"]["Received SD messages"];
    Json::Value find_requests=root["SD"]["Find Requests"];
    Json::Value ServiceInfoMap=root["SD"]["ServiceInfoMap"];
    int offered_services=0;
    int find_services=0;
    for(Json::Value::ArrayIndex i=0;i!=Received_SD_messages.size();i++)
    {
        if(Received_SD_messages[i]["Type"]=="offer service")
        {
            offer->insertRow(offered_services);
            std::string service_id = Received_SD_messages[i]["Service ID"].asString();
            std::string instance_id = Received_SD_messages[i]["Instance ID"].asString();
            offer->setItem(i,0,new QTableWidgetItem(QString(service_id.c_str())));
            offer->setItem(i,1,new QTableWidgetItem(QString(instance_id.c_str())));
            offered_services++;
        }
    }
    for(Json::Value::ArrayIndex j=0;j!=find_requests.size();j++)
    {
            find->insertRow(find_services);
            std::string service_id = find_requests[j]["Service ID"].asString();
            std::string instance_id = find_requests[j]["Instance ID"].asString();
            find->setItem(j,0,new QTableWidgetItem(QString(service_id.c_str())));
            find->setItem(j,1,new QTableWidgetItem(QString(instance_id.c_str())));
            find_services++;
    }
    for(Json::Value::ArrayIndex i=0;i!=ServiceInfoMap.size();i++)
    {
            std::string service_id = ServiceInfoMap[i]["Service ID"].asString();
            std::string instance_id = ServiceInfoMap[i]["Instance ID"].asString();
            std::string ip = ServiceInfoMap[i]["IP"].asString();
            std::string port = ServiceInfoMap[i]["Port Number"].asString();

            std::string result= " Service id : "+service_id
                               +" Instance id : "+instance_id
                               +" IP : "+ip
                               +" Port : "+port+"\n";
            console_text->insertPlainText(result.c_str());
            console_text->setTextColor(QColor(Qt::black));
    }

}
