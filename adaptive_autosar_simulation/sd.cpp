#include "sd.h"
#include <QTableWidget>

sd::sd(QWidget *parent): QWidget{parent}
{
    find_RequestsGBX->setLayout(new QHBoxLayout());

    main_layout->addWidget(find_RequestsGBX, 0, 2, 3, 1);
    main_layout->addWidget(Offered_ServicesGBX, 0 ,1, 3, 1);


    find_RequestsGBX ->setLayout(new QHBoxLayout);
    find_RequestsGBX->layout()->addWidget(find);

    find->setRowCount(5);
    find->setColumnCount(2);
    QStringList columnsheader = { "Service ID", "Instance ID"};
    find->setHorizontalHeaderLabels(columnsheader);
    find->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    Offered_ServicesGBX->setLayout(new QHBoxLayout);
    Offered_ServicesGBX->layout()->addWidget(new QListWidget);

    console->setLayout(new QVBoxLayout());
    main_layout->addWidget(console, 3, 0, 2, 3);

    console->layout()->addWidget(console_text);

    setLayout(main_layout);
}
