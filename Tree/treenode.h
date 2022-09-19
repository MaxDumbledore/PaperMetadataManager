#ifndef TREENODE_H
#define TREENODE_H

#include <QObject>
#include <QFile>

class TreeNode : public QObject
{
    Q_OBJECT
public:
    explicit TreeNode(QString _nodePath, QObject *parent = nullptr);

    void loadFromFile();
    void saveToFile();

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

    QString nodePath;
    virtual void load(QMap<QString,QString> &dataMap);
    virtual void save(QFile &file);

    static QPair<QString,QString> readNextItem(QFile &file);;
};

#endif // TREENODE_H
