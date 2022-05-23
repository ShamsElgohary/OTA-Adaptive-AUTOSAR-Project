#include "sm_tab.h"
#include"sm.h"
#include"com.h"
sm_tab::sm_tab(QWidget *parent)
    : QWidget{parent}
{
    sm *sm_t = new sm();
    main_layout->addWidget(sm_t,3);

    GUI_COMM *com_t = new GUI_COMM();
    main_layout->addWidget(com_t,1);
    //com_t->parse("/home/maher/Development/Qt/second rty/untitled/GUI_Report.json");
    //com_t->parseManifest("/home/maher/Documents/service_manifest.json");
    setLayout(main_layout);
}
