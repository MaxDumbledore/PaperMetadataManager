#ifndef TREEINFO_H
#define TREEINFO_H

#include <QObject>

class TreeInfo : public QObject
{
    Q_OBJECT
public:
    explicit TreeInfo(QObject *parent = nullptr);

    void loadFromFile(const QString &infoPath);
    void saveToFile(const QString &infoPath);

    int getId() const
    {
        return id;
    }

    void setId(int newId)
    {
        id = newId;
    }

    const QString &getName() const
    {
        return name;
    }

    void setName(const QString &newName)
    {
        name = newName;
    }

    const QString &getDescription() const
    {
        return description;
    }

    void setDescription(const QString &newDescription)
    {
        description = newDescription;
    }

    int getRootNodeId() const
    {
        return rootNodeId;
    }

    void setRootNodeId(int newRootNodeId)
    {
        rootNodeId = newRootNodeId;
    }

signals:
protected:
    int id,rootNodeId;
    QString name,description;
};

#endif // TREEINFO_H
