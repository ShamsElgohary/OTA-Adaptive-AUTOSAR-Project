#ifndef COM_H
#define COM_H

#include <QWidget>
#include <QtWidgets>
#include <QMainWindow>
#include<QtCore>
#include<QTreeWidget>
#include<QJsonDocument>
#include <type_traits>
#include<QString>

class GUI_COMM : public QWidget
{
    Q_OBJECT
public:
    //GUI_COMM(QTreeWidget*);
    explicit GUI_COMM(QWidget *parent = nullptr);
    void parse(QString);
    void parseManifest(QString);//path of service Instance manifest file
private:
    //QTreeWidget* tree;
    //QString path;
    QVBoxLayout * very_main = new QVBoxLayout;
    QGroupBox * main_box = new QGroupBox("Communication Management");
    QVBoxLayout * main_layout = new QVBoxLayout;
    QGroupBox * commBox = new QGroupBox("Comm Info");
    QGroupBox * srvcInstManifest = new QGroupBox("Service Instance Manifest");
    QVBoxLayout * commLayout = new QVBoxLayout;
    QVBoxLayout * manifestLayout = new QVBoxLayout;
    QTreeWidget * commTree = new QTreeWidget;
    QTreeWidget * ManifestTree = new QTreeWidget;


    void addTreeRoot(QString,QTreeWidget*,QString);
    void addTreeRootManifest(QString,QTreeWidget*,QString);
    void addTreeChild(QTreeWidgetItem *parent,QString first, QVariant name);
};

#endif // COM_H
