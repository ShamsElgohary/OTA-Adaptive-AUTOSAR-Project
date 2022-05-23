#include "ucm_tab.h"
#include"ucm.h"
#include"com.h"
ucm_tab::ucm_tab(QWidget *parent)
    : QWidget{parent}
{
    ucm *ucm_t = new ucm();
    main_layout->addWidget(ucm_t,3);

    GUI_COMM *com_t = new GUI_COMM();
    main_layout->addWidget(com_t,1);
    //com_t->parse("/home/maher/Development/Qt/second rty/untitled/GUI_Report.json");
    setLayout(main_layout);
}
