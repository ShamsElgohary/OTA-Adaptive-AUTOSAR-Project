#include "mainwindow.h"
#include "../../utility/general.hpp"
#include "execution_client.hpp"

#include <QApplication>

using namespace ara::exec;

void handle_sigTerm(int sig)
{
    ExecutionClient exec;
    exec.ReportExecutionStaste(ExecutionState::Kterminate);
    exit(1);
}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handle_sigTerm;
    sigaction(SIGTERM, &sa, NULL);

    ara::exec::ExecutionClient exec;
    exec.ReportExecutionStaste(ara::exec::ExecutionState::Krunning);

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