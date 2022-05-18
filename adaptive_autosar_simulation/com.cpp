#include"com.h"

void GUI_COMM::parse(QString path)
{
    QFile file(path);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "can't open error!";
        return;
    }

    // Read the entire contents of the file
    QTextStream stream(&file);
    //stream.setCodec("UTF-8");		// Set the read code to UTF8
    QString str = stream.readAll();

    file.close();

    // The QJsonParseError class is used to report errors during JSON parsing.
    QJsonParseError jsonError;
    // Parse json into UTF-8 encoded json document, and create a QJsonDocument from it.
    // If the parsing is successful, the QJsonDocument object is returned; otherwise, null is returned
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8(), &jsonError);
    // Judge whether parsing failed
    if (jsonError.error != QJsonParseError::NoError && !doc.isNull()) {
        qDebug() << "Json Format error!" << jsonError.error;
        return;
    }

    QJsonObject rootObj = doc.object();
    QJsonValue Value = rootObj.value("com_json");
    QJsonObject comjson = Value.toObject();
    QStringList list = comjson.keys();
    for(int i = 0 ; i<list.size() ; i++)
        this->addTreeRoot(list[i],this->tree,path);
}

void GUI_COMM::addTreeRoot(QString name,QTreeWidget* x,QString path)
{
    QTreeWidgetItem * itm = new QTreeWidgetItem(x);
    itm->setText(0,name);
    //ui->treeWidget->addTopLevelItem(itm);
    //addTreeChild(itm,"Method_ID : 1");

    QFile file(path);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "can't open error!";
        return;
    }

    // Read the entire contents of the file
    QTextStream stream(&file);
   // stream.setCodec("UTF-8");		// Set the read code to UTF8
    QString str = stream.readAll();

    file.close();

    // The QJsonParseError class is used to report errors during JSON parsing.
    QJsonParseError jsonError;
    // Parse json into UTF-8 encoded json document, and create a QJsonDocument from it.
    // If the parsing is successful, the QJsonDocument object is returned; otherwise, null is returned
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8(), &jsonError);
    // Judge whether parsing failed
    if (jsonError.error != QJsonParseError::NoError && !doc.isNull()) {
        qDebug() << "Json Format error!" << jsonError.error;
        return;
    }
    QJsonObject comjson = doc["com_json"].toObject();
    QJsonArray objectsArray = comjson[name].toArray();
    for(int i = 0 ; i<objectsArray.size() ; i++)
    {
        QJsonObject obj = objectsArray[i].toObject();

        QStringList list = obj.keys();
        for(int x=0 ; x<list.size() ; x++)
        {
            addTreeChild(itm,list[x],obj.value(list[x]).toVariant());
        }
        addTreeChild(itm,"","");
    }
}

void GUI_COMM::addTreeChild(QTreeWidgetItem *parent,QString first ,QVariant name)
{
    QTreeWidgetItem * itm = new QTreeWidgetItem();
    QString str = first+" : "+name.toString();
    if(first != "")
    {
        itm->setText(0,str);
    }
    else
    {
        itm->setText(0,"--------------");
    }
    parent->addChild(itm);
}

GUI_COMM::GUI_COMM(QTreeWidget* ptrTree)
{
    this->tree = ptrTree;
}


//void GUI_COMM::on_pushButton_clicked()
//{
//    QFile file("C:/Users/Mohamed/Desktop/qt/ucmcom.json");
//    if (!file.open(QFile::ReadOnly | QFile::Text)) {
//        qDebug() << "can't open error!";
//        return;
//    }

//    // Read the entire contents of the file
//    QTextStream stream(&file);
//    stream.setCodec("UTF-8");		// Set the read code to UTF8
//    QString str = stream.readAll();

//    file.close();

//    // The QJsonParseError class is used to report errors during JSON parsing.
//    QJsonParseError jsonError;
//    // Parse json into UTF-8 encoded json document, and create a QJsonDocument from it.
//    // If the parsing is successful, the QJsonDocument object is returned; otherwise, null is returned
//    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8(), &jsonError);
//    // Judge whether parsing failed
//    if (jsonError.error != QJsonParseError::NoError && !doc.isNull()) {
//        qDebug() << "Json Format error!" << jsonError.error;
//        return;
//    }

//    QJsonObject rootObj = doc.object();
//    QJsonValue Value = rootObj.value("com_json");
//    QJsonObject comjson = Value.toObject();
//    QStringList list = comjson.keys();
//    for(int i = 0 ; i<list.size() ; i++)
//        addTreeRoot(list[i]);
//}
