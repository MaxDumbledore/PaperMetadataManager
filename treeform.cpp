#include "treeform.h"
#include "ui_treeform.h"
#include <QStandardItemModel>
#include "tree.h"

TreeForm::TreeForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TreeForm)
{
    ui->setupUi(this);
    ui->treeView->setHeaderHidden(true);
}

TreeForm::~TreeForm()
{
    delete ui;
}

void TreeForm::reload()
{
    auto model = new QStandardItemModel(ui->treeView);
    ui->treeView->setModel(model);
    auto rootId=TreeNodeManager::getInstance()->getRootId();
    if(rootId!=-1)
    {
        auto rootItem=TreeNodeManager::getInstance()->buildModelItemRecursively(rootId);
        model->setItem(0,0,rootItem);
    }
    ui->treeView->expandAll();
}

void TreeForm::replaceModel(QStandardItemModel *_model)
{
    auto oriModel=ui->treeView->model();
    auto oriSelectionModel=ui->treeView->selectionModel();
    ui->treeView->setModel(_model);
    ui->treeView->setSelectionModel(new QItemSelectionModel(_model));
    if(oriModel!=nullptr){
        oriModel->deleteLater();
        oriSelectionModel->deleteLater();
    }
}
