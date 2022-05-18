#include "ota.h"

ota::ota(QWidget *parent)
    : QWidget{parent}
{

    main_layout->addWidget(over_the_air, 0, 0, 3, 1);
    main_layout->addWidget(ucm, 0 ,1, 3, 1);
    main_layout->addWidget(button_box, 3 ,0, 1, 2);



    over_the_air->setLayout(new QVBoxLayout);
    over_the_air->layout()->addWidget(meta_data);
    meta_data->setLayout(new QHBoxLayout);
    meta_data->layout()->addWidget(meta_data_tabel);

    meta_data_tabel->setRowCount(4);
    meta_data_tabel->setColumnCount(2);
    meta_data_tabel->setHorizontalHeaderLabels(QStringList() << "Package Name" << "Version");
    meta_data_tabel->setStyleSheet("QHeaderView::section { background-color:gray }");
    meta_data_tabel->setMinimumHeight(400);


    over_the_air->layout()->addWidget(download_data);
    download_data->setLayout(new QHBoxLayout);
    download_data->layout()->addWidget(download_data_tabel);

    download_data_tabel->setRowCount(4);
    download_data_tabel->setColumnCount(2);
    download_data_tabel->setHorizontalHeaderLabels(QStringList() << "Package Name" << "Version");
    download_data_tabel->setStyleSheet("QHeaderView::section { background-color:gray }");
    download_data_tabel->setMinimumHeight(400);


    ucm->setLayout(new QVBoxLayout);

    ucm->layout()->addWidget(ucm_package);
    ucm_package->setLayout(new QHBoxLayout);
    ucm_package->layout()->addWidget(ucm_package_tabel);
    ucm_package_tabel->setRowCount(4);
    ucm_package_tabel->setColumnCount(2);
    ucm_package_tabel->setHorizontalHeaderLabels(QStringList() << "Package Name" << "Version");
    ucm_package_tabel->setStyleSheet("QHeaderView::section { background-color:gray }");

    ucm_package_tabel->setMinimumHeight(400);

    ucm_console_text->setStyleSheet({"background-color:black"} );
    ucm->layout()->addWidget(ucm_console_text);

    button_box->setLayout(new QHBoxLayout);
    update->setText("Update");
    button_box->layout()->addWidget(update);
    discard_update->setText("Discard");
    button_box->layout()->addWidget(discard_update);

    setLayout(main_layout);

    connect(this,SIGNAL(ota_signal()),this,SLOT(update_ota()));
}


void ota::update_ota()
{
        std::ifstream file_input("file2.json"); //path to be updated
        Json::Reader reader;
        Json::Value root;
        reader.parse(file_input, root);

        Json::Value ecu_package_counter_v= root["ECU_packages_counter"];
        Json::Value server_package_counter_v=root["Server_packages_counter"];
        Json::Value package_2b_downloaded_v=root["Packges_2b_downloader_counter"];
        int ecu_package_counter= ecu_package_counter_v.asInt();
        int server_package_counter= server_package_counter_v.asInt();
        int package_2b_downloaded= package_2b_downloaded_v.asInt();

        ucm_package_tabel->setRowCount(0);
        ucm_package_tabel->reset();
        ucm_package_tabel->setRowCount(ecu_package_counter);
        ucm_package_tabel->setColumnCount(2);

        meta_data_tabel->setRowCount(0);
        meta_data_tabel->reset();
        meta_data_tabel->setRowCount(server_package_counter);
        meta_data_tabel->setColumnCount(2);

        download_data_tabel->setRowCount(0);
        download_data_tabel->reset();
        download_data_tabel->setRowCount(package_2b_downloaded);
        download_data_tabel->setColumnCount(2);


        int i=1;

        for(int pk=0;pk<server_package_counter;pk++)
        {
            string x="Package"+std::to_string(i);
            Json::Value name_r=root["server_Packages"][x]["Name"];
            Json::Value ver_r=root["server_Packages"][x]["Version"];
            string name=name_r.asString();
            string ver=ver_r.asString();
            meta_data_tabel->setItem(pk,0,new QTableWidgetItem(QString(name.c_str())));
            meta_data_tabel->setItem(pk,1,new QTableWidgetItem(QString(ver.c_str())));
            i++;
        }
        i=1;

        for(int pk=0;pk<ecu_package_counter;pk++)
        {
            string x="Package"+std::to_string(i);
            Json::Value name_r=root["ECU_Packages"][x]["Name"];
            Json::Value ver_r=root["ECU_Packages"][x]["Version"];
            string name=name_r.asString();
            string ver=ver_r.asString();
            ucm_package_tabel->setItem(pk,0,new QTableWidgetItem(QString(name.c_str())));
            ucm_package_tabel->setItem(pk,1,new QTableWidgetItem(QString(ver.c_str())));
            i++;
        }





        i=1;



        for(int pk=0;pk<package_2b_downloaded;pk++)
        {

            string x="Package"+std::to_string(i);
            Json::Value name_r=root["download_Packages"][x]["Name"];
            Json::Value ver_r=root["download_Packages"][x]["Version"];
            string name=name_r.asString();
            string ver=ver_r.asString();
            download_data_tabel->setItem(pk,0,new QTableWidgetItem(QString(name.c_str())));
            download_data_tabel->setItem(pk,1,new QTableWidgetItem(QString(ver.c_str())));
            i++;

        }


}




void ota::ota_handler()
{
    emit ota_signal();
}
