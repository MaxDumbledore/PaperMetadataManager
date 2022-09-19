#include "conceptnode.h"
#include <QMap>

ConceptNode::ConceptNode(QString _nodePath, QObject *parent)
    : TreeNode{_nodePath, parent},
      selectedAspectId(-1)
{

}

void ConceptNode::load(QMap<QString, QString> &dataMap)
{
    TreeNode::load(dataMap);
    if(!dataMap.contains("SELECTED_ASPECT_ID")||selectedAspectId==-1)
    {
        if(!nodeChilds.empty())
            selectedAspectId=nodeChilds.at(0);
        return ;
    }
    selectedAspectId=dataMap["SELECTED_ASPECT_ID"].toInt();
}

void ConceptNode::save(QFile &file)
{
    TreeNode::save(file);
    file.write(QString("SELECTED_ASPECT_ID=%1\n").arg(selectedAspectId).toUtf8());
}




