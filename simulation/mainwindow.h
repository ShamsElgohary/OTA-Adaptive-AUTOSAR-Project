#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include "simulation.hpp"
#include "QThread"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void handle_sm();
    void choose_handler();
public slots:
    void sock_listen();


private:
    Ui::MainWindow *ui;
    simulation* s=nullptr;
    QThread* sm_thread=nullptr;
};
#endif // MAINWINDOW_H