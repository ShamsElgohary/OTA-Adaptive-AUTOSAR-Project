#ifndef SM_H
#define SM_H

#include <QWidget>
#include <QtWidgets>

class sm: public QWidget
{
    Q_OBJECT
public:
    explicit sm(QWidget *parent = nullptr);
signals:
private:

    QGridLayout *main_layout = new QGridLayout;
    QGroupBox *state_management = new QGroupBox("State Management", this);
    QGroupBox *function_group_state = new QGroupBox("Function Group State", this);
    QGroupBox *console = new QGroupBox("Console", this);
    QGroupBox *slider = new QGroupBox("slider", this);
    QPushButton * start_simulation =new QPushButton;
    QTextBrowser *console_text = new QTextBrowser;
    QLabel *label = new QLabel;
    QSlider *s_slider = new QSlider(Qt::Horizontal, this);
    QProgressBar *p_slider = new QProgressBar;
    QTableWidget *function_group_state_table = new QTableWidget;





};

#endif // SM_H
