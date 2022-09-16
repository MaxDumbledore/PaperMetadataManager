#include "treenode.h"
#include <QFile>
#include <QDebug>

TreeNode::TreeNode(QObject *parent)
    : QObject{parent},
      id(-1),
      nodeParent(-1)
{

}

TreeNode::TreeNode(int _id, int _nodeParent, const QString &_name, const QString &_description, const QList<int> &_nodeChilds, QObject *parent):
    QObject{parent},
    id(_id),
    nodeParent(_nodeParent),
    name(_name),
    description(_description),
    nodeChilds(_nodeChilds)
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
        l.append(i.toInt());
    return l;
}

void TreeNode::loadFromFile(const QString &nodePath)
{
    QFile file(nodePath);
    file.open(QFile::ReadOnly);

    auto nextItem=[&]() -> QString{
        auto l=QString(file.readLine()).split('=');
        if(l.size()!=2){
            qDebug()<<"Wrong in parse '=' in string!";
        }
        return l.back();
    };

    id=nextItem().toInt();
    nodeParent=nextItem().toInt();
    name=nextItem();
    description=nextItem();
    nodeChilds=stringToListInt(nextItem());
}

void TreeNode::saveToFile(const QString &nodePath)
{
    QFile file(nodePath);
    file.open(QFile::WriteOnly);

    QString dataStr;
    dataStr.append(QString("ID=%1\n").arg(id));
    dataStr.append(QString("PARENT_ID=%1\n").arg(nodeParent));
    dataStr.append(QString("NAME=%1\n").arg(name));
    dataStr.append(QString("DESCRIPTION=%1\n").arg(description));
    dataStr.append(QString("CHILD_IDS=%1\n").arg(listIntToString(nodeChilds)));
    file.write(dataStr.toUtf8());
    file.close();
}
















