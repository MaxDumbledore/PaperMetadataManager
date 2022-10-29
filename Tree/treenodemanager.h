#ifndef TREENODEMANAGER_H
#define TREENODEMANAGER_H

#include <QObject>
#include <QMap>
#include <QApplication>
#include <QStandardItem>
#include "treenode.h"

class TreeNodeManager : public QObject
{
    Q_OBJECT
public:
    explicit TreeNodeManager(QObject *parent = nullptr);

    static TreeNodeManager *getInstance()
    {
        if(instance==nullptr)
            instance=new TreeNodeManager(qApp);
        return instance;
    }

    bool addNode(TreeNode *treeNode);
    bool removeNode(int nodeId);

    const QMap<int, TreeNode *> &getNodes() const
    {
        return nodes;
    }

    void reload();
    void saveAndQuit();

    int getRootId() const
    {
        return rootId;
    }

    int findFirstContainedByNodeName(int curId, const QString &s);

signals:
protected:
    QMap<int,TreeNode *> nodes;
    int rootId;
    inline static TreeNodeManager *instance=nullptr;
};

#endif // TREENODEMANAGER_H
