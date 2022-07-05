#include "mainwindow.h"

#include <QApplication>
#include "../../utility/general.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
       MainWindow w;
    w.setWindowTitle("Car Engine");
    QString qss_path=CUSTOMIZED_PROJECT_PATH.c_str()+QString("utility/Theme/SpyBot2.qss");
        QFile stylesheetfile(qss_path);
    if (stylesheetfile.open(QFile::ReadOnly))
    {qDebug() << "opened";}
    QString stylesheet=QLatin1String(stylesheetfile.readAll());
    a.setStyleSheet(stylesheet);
    QString icons_path=CUSTOMIZED_PROJECT_PATH.c_str()+QString("utility/Theme/car.png");
    a.setWindowIcon(QIcon(icons_path));
    w.show();
    return a.exec();
}
