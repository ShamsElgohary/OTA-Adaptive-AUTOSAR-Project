#include "sm_tab.h"
#include"sm.h"
#include"com.h"
sm_tab::sm_tab(QWidget *parent)
    : QWidget{parent}
{

    sm_instance = new sm(nullptr,com_t);
    main_layout->addWidget(sm_instance,3);
    main_layout->addWidget(com_t,1);
    setLayout(main_layout);
}
