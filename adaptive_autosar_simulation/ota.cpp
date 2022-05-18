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
    meta_data_button->setText("Get Meta Data");
    meta_data->layout()->addWidget(meta_data_button);

    ucm->setLayout(new QVBoxLayout);

    ucm->layout()->addWidget(ucm_package);
    ucm_package->setLayout(new QHBoxLayout);
    ucm_package->layout()->addWidget(ucm_package_tabel);
    ucm_package_tabel->setRowCount(4);
    ucm_package_tabel->setColumnCount(2);
    ucm_package_tabel->setHorizontalHeaderLabels(QStringList() << "Package Name" << "Version");
    ucm_package_tabel->setStyleSheet("QHeaderView::section { background-color:gray }");
    ucm_package_button->setText("Get ucm Package");
    ucm_package->layout()->addWidget(ucm_package_button);

    ucm_console_text->setStyleSheet({"background-color:black"} );
    ucm->layout()->addWidget(ucm_console_text);

    button_box->setLayout(new QHBoxLayout);
    update->setText("Update");
    button_box->layout()->addWidget(update);
    discard_update->setText("Discard");
    button_box->layout()->addWidget(discard_update);

    setLayout(main_layout);
}
