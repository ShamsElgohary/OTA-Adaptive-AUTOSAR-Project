#ifndef COM_H
#define COM_H

#include <QMainWindow>
#include<QtCore>
#include<QTreeWidget>
#include<QJsonDocument>
#include <type_traits>
#include<QString>

class GUI_COMM
{

public:
    GUI_COMM(QTreeWidget*);
    void parse(QString);
private:
    QTreeWidget* tree;
    //QString path;
    void addTreeRoot(QString,QTreeWidget*,QString);
    void addTreeChild(QTreeWidgetItem *parent,QString first, QVariant name);
};

#endif // COM_H
