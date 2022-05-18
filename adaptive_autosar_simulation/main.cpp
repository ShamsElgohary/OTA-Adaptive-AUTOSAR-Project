#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile stylesheetfile("/home/mariam/Desktop/OTA-Adaptive-AUTOSAR-Project/utility/Theme/Diffnes.qss");
    if (stylesheetfile.open(QFile::ReadOnly))
    {qDebug() << "opened";}
    QString stylesheet=QLatin1String(stylesheetfile.readAll());
    a.setStyleSheet(stylesheet);
    MainWindow w;
    w.showMaximized();
    w.setWindowTitle("Adapative AUTOSAR Simulation");
    return a.exec();
}
