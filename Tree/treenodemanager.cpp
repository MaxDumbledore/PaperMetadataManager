#include "treenodemanager.h"
#include "config.h"
#include "aspectnode.h"
#include "conceptnode.h"
#include <QDebug>

TreeNodeManager::TreeNodeManager(QObject *parent)
    : QObject{parent},
      rootId(-1)
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
    rootId=-1;
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

    auto localKeys = nodes.keys();
    for(auto &i:qAsConst(localKeys))
        if(nodes[i]->getNodeParent()==-1){
            rootId= i;
            break;
        }
}

void TreeNodeManager::saveAndQuit()
{
    auto localValues = nodes.values();
    for(auto &i:qAsConst(localValues))
        i->saveToFile();
}

int TreeNodeManager::findFirstContainedByNodeName(int curId, const QString &s)
{
    if(nodes[curId]->getName().contains(s))
        return curId;
    for(auto &i:nodes[curId]->getNodeChilds()){
        auto r=findFirstContainedByNodeName(i,s);
        if(r!=-1)
            return r;
    }
    return -1;
}


