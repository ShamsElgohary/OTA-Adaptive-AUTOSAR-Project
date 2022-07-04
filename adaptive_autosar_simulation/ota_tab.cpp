#include "ota_tab.h"
#include"ota.h"
#include"com.h"
ota_tab::ota_tab(QWidget *parent)
    : QWidget{parent}
{
    GUI_COMM *com_t = new GUI_COMM();
    ota_instance = new ota(nullptr,com_t);
    main_layout->addWidget(ota_instance,3);
    main_layout->addWidget(com_t,1);
    setLayout(main_layout);
}
