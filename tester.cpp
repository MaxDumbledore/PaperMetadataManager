#include "tester.h"
#include "tree.h"
#include "config.h"
#include <QDebug>

void testTreeNode()
{
    Config::setDataRoot("D:/PaperMetadata/");
    TreeNode node;
    node.setId(1);
    node.setDescription("Hello");
    node.setName("Fuck");
    node.setNodeParent(-1);
    node.setNodeChilds(QList<int>{2,3,4});
    node.saveToFile(Config::getTreePath()+"node1.txt");
    TreeNode node2;
    node2.loadFromFile(Config::getTreePath()+"node1.txt");
}

void test()
{
    QDir dir("D:/");
    qDebug()<<(dir.entryList({"Qt*"}));
}
