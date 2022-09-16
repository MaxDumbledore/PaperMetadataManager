#ifndef TREENODE_H
#define TREENODE_H

#include <QObject>

class TreeNode : public QObject
{
    Q_OBJECT
public:
    explicit TreeNode(QObject *parent = nullptr);

    TreeNode(int _id, int _nodeParent, const QString &_name, const QString &_description, const QList<int> &_nodeChilds, QObject *parent=nullptr);

    void loadFromFile(const QString &nodePath);
    void saveToFile(const QString &nodePath);

    int getId() const
    {
        return id;
    }

    int getNodeParent() const{
        return nodeParent;
    }

    const QList<int> & getNodeChilds() const
    {
        return nodeChilds;
    }

    const QString &getName() const
    {
        return name;
    }

    const QString &getDescription() const
    {
        return description;
    }

    void setId(int newId)
    {
        id = newId;
    }

    void setNodeParent(int newNodeParent)
    {
        nodeParent = newNodeParent;
    }

    void setName(const QString &newName)
    {
        name = newName;
    }

    void setDescription(const QString &newDescription)
    {
        description = newDescription;
    }

    void setNodeChilds(const QList<int> &newNodeChilds)
    {
        nodeChilds = newNodeChilds;
    }

signals:
protected:
    int id,nodeParent;
    QString name,description;
    QList<int> nodeChilds;
};

#endif // TREENODE_H
