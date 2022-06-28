#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setMaximumHeight(5000);
    w.setMaximumWidth(5000);
    w.showMaximized();
    w.setWindowTitle("Adapative AUTOSAR Simulation");
    QString qss_path=CUSTOMIZED_PROJECT_PATH.c_str()+QString("utility/Theme/Diffnes.qss");
    QFile stylesheetfile(qss_path);
    if (stylesheetfile.open(QFile::ReadOnly))
    {qDebug() << "opened";}
    QString stylesheet=QLatin1String(stylesheetfile.readAll());
    a.setStyleSheet(stylesheet);
    return a.exec();
}
