#include "ota.h"

ota::ota(QWidget *parent,GUI_COMM*ptr) :QWidget{parent}
{
    com_ptr = ptr;

    main_layout->addWidget(over_the_air, 0, 0, 3, 1);
    main_layout->addWidget(ucm, 0 ,1, 3, 1);


    QFont font;
    font.setPointSize(11);
    font.setBold(true);


    over_the_air->setLayout(new QVBoxLayout);
    over_the_air->layout()->addWidget(meta_data);
    meta_data->setLayout(new QHBoxLayout);
    meta_data->layout()->addWidget(meta_data_tabel);
    meta_data_tabel->setColumnCount(2);
    meta_data_tabel->setHorizontalHeaderLabels(QStringList() << "Name" << "Version");
    meta_data_tabel->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    meta_data_tabel->horizontalHeader()->setFont(font);



    over_the_air->layout()->addWidget(download_data);
    download_data->setLayout(new QHBoxLayout);
    download_data->layout()->addWidget(download_data_tabel);
    download_data_tabel->setColumnCount(2);
    download_data_tabel->setHorizontalHeaderLabels(QStringList() << "Name" << "Version");
    download_data_tabel->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    download_data_tabel->horizontalHeader()->setFont(font);


    over_the_air->setLayout(new QHBoxLayout);
    update->setText("Update");
    update->setMinimumHeight(50);
    over_the_air->layout()->addWidget(update);



    ucm->setLayout(new QVBoxLayout);
    ucm->layout()->addWidget(ucm_package);
    ucm_package->setLayout(new QHBoxLayout);
    ucm_package->layout()->addWidget(ucm_package_tabel);
    ucm_package_tabel->setColumnCount(2);
    ucm_package_tabel->setHorizontalHeaderLabels(QStringList() << "Name" << "Version");
    ucm_package_tabel->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ucm_package_tabel->horizontalHeader()->setFont(font);

    update->setStyleSheet(QString::fromUtf8("QPushButton{background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
      "stop: 0 white, stop: 1 grey);"
      "border-style: solid;"
      "border-width: 2px;"
      "font: bold;"
      "border-color: black;"
      "border-radius: 15px;}"));
    setLayout(main_layout);

    connect(this,SIGNAL(ota_signal()),this,SLOT(update_ota()));

    connect(update, SIGNAL(clicked()), this, SLOT(update_thread()));

    com_ptr->parseManifest(serviceInstanceManifestPath);
}

void ota::ota_connect()
{
    string s = update_signal();
    string path(CUSTOMIZED_PROJECT_PATH+"gui_ota");
    int fd=open(path.c_str(), O_WRONLY);
    int size = s.length();
    write(fd,&size,sizeof(int));
    write(fd,s.c_str(),s.length());
    write(fd,"\0",sizeof(char));
    ::close(fd);
}
bool ota_flag = true;
void ota::update_thread(){
    if(ota_flag)
    {
        QThread::create([this]{
            ota_connect();
        })->start();
        ota_flag =false;
    }
}
string ota::update_signal()
{
    auto i = download_data_tabel->selectedItems();
    if(i.length()>0)
    {
        int  s =i[0]->row();
        download_data_tabel->selectRow(s);
        auto row_col = download_data_tabel->selectedItems();
        QString name;
        for(auto &c: row_col)
        {
             name+= c->text();
        }
        return name.toStdString();
    }
    else    {
        cout<<"select a package"<<endl;
    }
}

void ota::update_ota()
{
        ucm_package_tabel->reset();
        meta_data_tabel->reset();
        download_data_tabel->reset();
        ucm_package_tabel->setRowCount(0);
        meta_data_tabel->setRowCount(0);
        download_data_tabel->setRowCount(0);
        com_ptr->parse(reportPath);
        std::ifstream file_input(to_string(simulation::exe_name::ota)); //path to be updated
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
