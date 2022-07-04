#include "ucm_tab.h"
#include"ucm.h"
#include"com.h"
ucm_tab::ucm_tab(QWidget *parent)
    : QWidget{parent}
{
    GUI_COMM *com_t = new GUI_COMM();
    ucm_instance = new ucm(nullptr,com_t);
    main_layout->addWidget(ucm_instance,3);
    main_layout->addWidget(com_t,1);
    setLayout(main_layout);
}
