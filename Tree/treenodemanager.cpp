#include "treenodemanager.h"
#include "config.h"

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

bool TreeNodeManager::addInfo(TreeInfo *treeInfo)
{
    if(infos.contains(treeInfo->getId()))
        return false;
    infos[treeInfo->getId()]=treeInfo;
    treeInfo->setParent(this);
    return true;
}

bool TreeNodeManager::removeInfo(int infoId)
{
    if(!infos.contains(infoId))
        return false;
    auto node=nodes[infoId];
    nodes.remove(infoId);
    node->deleteLater();
    return true;
}

QList<int> TreeNodeManager::getRoots()
{
    QList<int> roots;
    for(auto i:qAsConst(nodes))
        if(i->getNodeParent()==-1)
            roots.append(i->getId());
    return roots;
}

void TreeNodeManager::reload()
{
    nodes.clear();
    infos.clear();
    QDir dir(Config::getTreePath());
    auto nodeFileList=dir.entryList({"node*.txt"});
    auto infoFileList=dir.entryList({"info*.txt"});
    for(auto &i:qAsConst(nodeFileList))
    {
        auto node=new TreeNode();
        node->loadFromFile(Config::getTreePath()+i);
        addNode(node);
    }
    for(auto &i:qAsConst(infoFileList))
    {
        auto info=new TreeInfo();
        info->loadFromFile(Config::getTreePath()+i);
        addInfo(info);
    }
}


