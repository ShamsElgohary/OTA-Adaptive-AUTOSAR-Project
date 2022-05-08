#ifndef EXEC_H
#define EXEC_H

#include <QWidget>
#include <QtWidgets>

class exec : public QWidget
{
    Q_OBJECT
public:
    explicit exec(QWidget *parent = nullptr);

signals:
private:
    QGridLayout *main_layout = new QGridLayout;
    QGroupBox *machine_configGbx = new QGroupBox("Machine Configuration", this);
    QGroupBox *exec_configGbx = new QGroupBox("Executables Configuration", this);
    QGroupBox *sm_requestsGbx = new QGroupBox("Sm Requests", this);

};

#endif // EXEC_H
