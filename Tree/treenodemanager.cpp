#include "treenodemanager.h"
#include "config.h"
#include "aspectnode.h"
#include "conceptnode.h"
#include <QDebug>

TreeNodeManager::TreeNodeManager(QObject *parent)
    : QObject{parent}
{

}

bool TreeNodeManager::addNode(TreeNode *treeNode)
{
    if(nodes.contains(treeNode->getId()))
        return false;
    nodes[treeNode->getId()]=treeNode;
    treeNode->setParent(this);
    return true;
}

bool TreeNodeManager::removeNode(int nodeId)
{
    if(!nodes.contains(nodeId))
        return false;
    auto node=nodes[nodeId];
    nodes.remove(nodeId);
    node->deleteLater();
    return true;
}

void TreeNodeManager::reload()
{
    nodes.clear();
    QDir dir(Config::getTreePath());
    auto aspectFileList=dir.entryList({"aspect*.txt"});
    for(auto &i:qAsConst(aspectFileList))
    {
        auto node=new AspectNode(Config::getTreePath()+i);
        node->loadFromFile();
        addNode(node);
    }
    auto conceptFileList=dir.entryList({"concept*.txt"});
    for(auto &i:qAsConst(conceptFileList))
    {
        auto node=new ConceptNode(Config::getTreePath()+i);
        node->loadFromFile();
        addNode(node);
    }
}

void TreeNodeManager::saveAndQuit()
{
    auto localValues = nodes.values();
    for(auto &i:qAsConst(localValues))
        i->saveToFile();
}

int TreeNodeManager::getRootId()
{
    auto localKeys = nodes.keys();
    for(auto &i:qAsConst(localKeys))
        if(nodes[i]->getNodeParent()==-1)
            return i;
    return -1;
}

QStandardItem *TreeNodeManager::buildModelItemRecursively(int curId)
{
    auto item=new QStandardItem(nodes[curId]->getName());
    item->setEditable(false);
    auto aspectId=qobject_cast<ConceptNode *>(nodes[curId])->getSelectedAspectId();
    if(aspectId==-1)
        return item;
//    qDebug()<<aspectId;
    auto childList=nodes[aspectId]->getNodeChilds();
    for(int i=0;i<childList.size();i++)
        item->setChild(i,0,buildModelItemRecursively(childList.at(i)));
    return item;
}


