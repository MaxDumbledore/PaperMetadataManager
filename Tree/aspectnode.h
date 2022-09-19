#ifndef ASPECTNODE_H
#define ASPECTNODE_H

#include <QObject>
#include "treenode.h"

class AspectNode : public TreeNode
{
    Q_OBJECT
public:
    explicit AspectNode( QString _nodePath,QObject *parent = nullptr);
signals:
};

#endif // ASPECTNODE_H
