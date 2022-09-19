#include "config.h"
#include "tree.h"
#include "metadatamanager.h"
#include "mainwindow.h"


void Config::setDataRoot(QString _dataRoot){
    dataRoot=std::move(_dataRoot);
    treePath=dataRoot+"trees/";
    paperMetaPath=dataRoot+"meta_db/";
    notePath=dataRoot+"notes/";
    create_dir_if_not_exist(dataRoot);
    create_dir_if_not_exist(treePath);
    create_dir_if_not_exist(paperMetaPath);
    create_dir_if_not_exist(notePath);
    TreeNodeManager::getInstance()->reload();
    MetaDataManager::getInstance()->reload();
    MainWindow::getInstance()->reload();
}

void Config::saveAndQuit()
{
    if(dataRoot.isEmpty())
        return ;
    TreeNodeManager::getInstance()->saveAndQuit();
    MetaDataManager::getInstance()->saveAndQuit();
    MainWindow::getInstance()->saveAndQuit();
}
