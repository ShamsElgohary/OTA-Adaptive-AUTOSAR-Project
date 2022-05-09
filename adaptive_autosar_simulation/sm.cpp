#include "sm.h"

sm::sm(QWidget *parent): QWidget{parent}
{

    main_layout->addWidget(state_management, 0, 0, 2, 1);
    main_layout->addWidget(console, 2, 0, 2, 1);



    state_management->setLayout(new QVBoxLayout);

    label->setText("Start Upadte Session");
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

    start_simulation->setText("start simulation");
    state_management->layout()->addWidget(start_simulation);

    console->setLayout(new QVBoxLayout());
    main_layout->addWidget(console, 2, 0, 2, 1);

    console->layout()->addWidget(console_text);

    setLayout(main_layout);
}
