#ifndef CONCEPTNODE_H
#define CONCEPTNODE_H

#include <QObject>
#include "treenode.h"

class ConceptNode : public TreeNode
{
    Q_OBJECT
public:
    explicit ConceptNode(QString _nodePath, QObject *parent = nullptr);

    void load(QMap<QString, QString> &dataMap) override;
    void save(QFile &file) override;

    int getSelectedAspectId() const
    {
        return selectedAspectId;
    }

    void setSelectedAspectId(int newSelectedAspectId)
    {
        selectedAspectId = newSelectedAspectId;
    }

signals:
protected:
    int selectedAspectId;
};

#endif // CONCEPTNODE_H
