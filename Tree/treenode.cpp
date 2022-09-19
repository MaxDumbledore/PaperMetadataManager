#include "treenode.h"
#include <QFile>
#include <QDebug>

TreeNode::TreeNode(QString _nodePath, QObject *parent)
    : QObject{parent},
      id(-1),
      nodeParent(-1),
      nodePath(std::move(_nodePath))
{
}

QString listIntToString(const QList<int> &l){
    QString s;
    for(int i:qAsConst(l))
        s+=QString("%1,").arg(i);
    s.chop(1);
    return s;
}

QList<int> stringToListInt(const QString &s){
    auto c=s.split(',');
    QList<int> l;
    for(auto &i:qAsConst(c))
        if(!i.isEmpty())
            l.append(i.toInt());
    return l;
}

void TreeNode::loadFromFile()
{
    QFile file(nodePath);
    file.open(QFile::ReadOnly);
    QMap<QString,QString> dataMap;
    while(!file.atEnd()){
        auto item=readNextItem(file);
        if(item.first.isEmpty())
            continue;
        dataMap.insert(item.first,item.second);
    }
    load(dataMap);
    file.close();
}

void TreeNode::saveToFile()
{
    QFile file(nodePath);
    file.open(QFile::WriteOnly);
    save(file);
    file.close();
}

void TreeNode::load(QMap<QString,QString> &dataMap)
{
    id=dataMap["ID"].toInt();
    nodeParent=dataMap["PARENT_ID"].toInt();
    name=dataMap["NAME"];
    description=dataMap["DESCRIPTION"];
    nodeChilds=stringToListInt(dataMap["CHILD_IDS"]);
//    qDebug()<<nodeChilds;
}

void TreeNode::save(QFile &file)
{
    QString dataStr;
    dataStr.append(QString("ID=%1\n").arg(id));
    dataStr.append(QString("PARENT_ID=%1\n").arg(nodeParent));
    dataStr.append(QString("NAME=%1\n").arg(name));
    dataStr.append(QString("DESCRIPTION=%1\n").arg(description));
    dataStr.append(QString("CHILD_IDS=%1\n").arg(listIntToString(nodeChilds)));
    file.write(dataStr.toUtf8());
}

QPair<QString, QString> TreeNode::readNextItem(QFile &file){
    auto l=QString(file.readLine()).split('=');
    if(l.size()!=2){
        qDebug()<<l<<"Wrong in parse '=' in string!";
        return {};
    }
    l[1].chop(1); //pop \n
    return {std::move(l.at(0)),std::move(l.at(1))};
}
















