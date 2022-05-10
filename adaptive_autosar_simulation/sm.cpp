#include "sm.h"

sm::sm(QWidget *parent): QWidget{parent}
{

    main_layout->addWidget(state_management, 0, 0, 2, 1);
    main_layout->addWidget(console, 2, 0, 2, 1);



    state_management->setLayout(new QVBoxLayout);

    label->setText("Start Update Session");
    state_management->layout()->addWidget(label);

    state_management->layout()->addWidget(slider);
    slider->setLayout(new QVBoxLayout());

    slider->layout()->addWidget(s_slider);

    p_slider->setStyleSheet("value: 20;");
    slider->layout()->addWidget(p_slider);

    p_slider->setValue(s_slider->value());
    connect(s_slider, SIGNAL(valueChanged(int)), p_slider, SLOT(setValue(int)));

    state_management->layout()->addWidget(function_group_state);
    function_group_state->setLayout(new QVBoxLayout());
    function_group_state->layout()->addWidget(function_group_state_table);
    function_group_state_table->setRowCount(2);
    function_group_state_table->setColumnCount(2);

    //start_simulation->setText("start simulation");
    //state_management->layout()->addWidget(start_simulation);

    console->setLayout(new QVBoxLayout());
    main_layout->addWidget(console, 2, 0, 2, 1);

    console->layout()->addWidget(console_text);

    setLayout(main_layout);

    sim=new simulation(8088);
    connect(this,SIGNAL(sm_signal()),this,SLOT(update_sm()));
    connect(this,SIGNAL(sm_signal()),this,SLOT(update_terminal()));
}
void sm::sm_handler()
{
    emit sm_signal();
}
void sm::update_sm()
{
        std::ifstream file_input("file2.json"); //path to be updated
        Json::Reader reader;
        Json::Value root;
        reader.parse(file_input, root);
        p_slider->reset();
        p_slider->setValue(0);
        label->clear();
        if(root["sm_json"]["UpdateRequest"]["StartUpdateSession"]==true)
        {
        label->setText("StartUpdateSession");
        p_slider->setValue(25);
        }
        if(root["sm_json"]["UpdateRequest"]["StopUpdateSession"]==true)
        {
        label->setText("StopUpdateSession");
        p_slider->setValue(100);
        }
        if(root["sm_json"]["UpdateRequest"]["VerifyUpdate"]==true)
        {
        label->setText("VerifyUpdate");
        p_slider->setValue(75);
        }
        if(root["sm_json"]["UpdateRequest"]["PrepareUpdate"]==true)
        {
        label->setText("PrepareUpdate");
        p_slider->setValue(50);
        }
        std::map<std::string,std::string>FG_states;
        Json::Value states=root["sm_json"]["function_group_states"];
        function_group_state_table->setRowCount(0);
        function_group_state_table->reset();
        int size=states.size();
        function_group_state_table->setRowCount(size);
        function_group_state_table->setColumnCount(2);
        int i=0;
        for(auto fg: states.getMemberNames())
        {
            function_group_state_table->setItem(i,0,new QTableWidgetItem(QString(fg.c_str())));
            function_group_state_table->setItem(i,1,new QTableWidgetItem(QString(states[fg].asCString())));
            i++;
        }
}
void sm::update_terminal()
{
        console_text->clear();
        QFile file("/home/youssef/Documents/OTA-Adaptive-AUTOSAR-Project/sm.txt");
        file.open(QIODevice::ReadOnly);
        QTextStream in(&file);
        console_text->setText(in.readAll());
        //console_text->setTextColor(QColor(Qt::white));
}
