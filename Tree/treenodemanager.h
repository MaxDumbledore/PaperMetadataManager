#ifndef TREENODEMANAGER_H
#define TREENODEMANAGER_H

#include <QObject>
#include <QMap>
#include <QApplication>
#include "treenode.h"
#include "treeinfo.h"

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

    bool addInfo(TreeInfo *treeInfo);
    bool removeInfo(int infoId);

    QList<int> getRoots();

    const QMap<int, TreeNode *> &getNodes() const
    {
        return nodes;
    }

    void reload();

signals:
protected:
    QMap<int,TreeNode *> nodes;
    QMap<int,TreeInfo *> infos;
    inline static TreeNodeManager *instance=nullptr;
};

#endif // TREENODEMANAGER_H
