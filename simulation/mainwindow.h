#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include "simulation.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void handle_sm();

private:
    Ui::MainWindow *ui;
    simulation* s=nullptr;
    bool sim=1;
};
#endif // MAINWINDOW_H
