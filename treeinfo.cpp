#include "treeinfo.h"
#include <QFile>
#include <QDebug>

TreeInfo::TreeInfo(QObject *parent)
    : QObject{parent}
{

}

void TreeInfo::loadFromFile(const QString &infoPath)
{
    QFile file(infoPath);
    file.open(QFile::ReadOnly);

    auto nextItem=[&]() -> QString{
        auto l=QString(file.readLine()).split('=');
        if(l.size()!=2){
            qDebug()<<"Wrong in parse '=' in string!";
        }
        return l.back();
    };

    id=nextItem().toInt();
    rootNodeId=nextItem().toInt();
    name=nextItem();
    description=nextItem();
}

void TreeInfo::saveToFile(const QString &infoPath)
{
    QFile file(infoPath);
    file.open(QFile::WriteOnly);

    QString dataStr;
    dataStr.append(QString("ID=%1\n").arg(id));
    dataStr.append(QString("ROOT_NODE_ID=%1\n").arg(rootNodeId));
    dataStr.append(QString("NAME=%1\n").arg(name));
    dataStr.append(QString("DESCRIPTION=%1\n").arg(description));
    file.write(dataStr.toUtf8());
    file.close();
}















