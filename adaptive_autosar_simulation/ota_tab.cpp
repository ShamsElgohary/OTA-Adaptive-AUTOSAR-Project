#include "ota_tab.h"
#include"ota.h"
#include"com.h"
ota_tab::ota_tab(QWidget *parent)
    : QWidget{parent}
{
    ota *ota_t = new ota();
    main_layout->addWidget(ota_t,3);

    GUI_COMM *com_t = new GUI_COMM();
    main_layout->addWidget(com_t,1);
    //com_t->parse("/home/maher/Development/Qt/second rty/untitled/GUI_Report.json");
    setLayout(main_layout);
}
