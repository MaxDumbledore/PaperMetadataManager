#include "treeform.h"
#include "ui_treeform.h"
#include <QStandardItemModel>
#include <QDebug>
#include "tree.h"

TreeForm::TreeForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TreeForm)
{
    ui->setupUi(this);
    //    ui->treeView->setHeaderHidden(true);
    contextMenu=new QMenu(this);
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, &QTreeView::customContextMenuRequested, this,[this](const QPoint &pos){
        QModelIndex index = ui->treeView->indexAt(pos);
        if (!index.isValid())
            return ;
        auto nodeId=qobject_cast<QStandardItemModel*>(ui->treeView->model())->itemFromIndex(index)->data().toInt();
        const auto &nodes=TreeNodeManager::getInstance()->getNodes();
        contextMenu->clear();
        const auto &aspects=nodes[nodeId]->getNodeChilds();
        for(auto i:aspects)
            contextMenu->addAction(QString("%1").arg(nodes[i]->getName()));
        if(!contextMenu->isEmpty())
            contextMenu->exec(ui->treeView->viewport()->mapToGlobal(pos));

    });
}

TreeForm::~TreeForm()
{
    delete ui;
}

void TreeForm::reload()
{
    auto model = new QStandardItemModel(ui->treeView);
    model->setHorizontalHeaderLabels({"Concept Name","Description"});
    auto rootId=TreeNodeManager::getInstance()->getRootId();
    if(rootId!=-1)
    {
        auto rootItem=buildItemLine(rootId);
        buildModelItemRecursively(rootItem);
        model->setItem(0,0,rootItem);
    }

    replaceModel(model);
    ui->treeView->expandAll();
}

void TreeForm::replaceModel(QStandardItemModel *_model)
{
    auto oriModel=ui->treeView->model();
    ui->treeView->setModel(_model);
    ui->treeView->setSelectionModel(new QItemSelectionModel(_model,_model));
    if(oriModel!=nullptr)
        oriModel->deleteLater();
}

void TreeForm::buildModelItemRecursively(QStandardItem *item)
{
    const auto &nodes=TreeNodeManager::getInstance()->getNodes();
    auto nodeId=item->data().toInt();
    auto aspectId=qobject_cast<ConceptNode *>(nodes[nodeId])->getSelectedAspectId();
    if(aspectId==-1)
        return ;
    //    qDebug()<<aspectId;
    auto childList=nodes[aspectId]->getNodeChilds();
    for(int i=0;i<childList.size();i++)
        buildModelItemRecursively(buildItemLine(childList.at(i), item,i));
}

QStandardItem *TreeForm::buildItemLine(int nodeId, QStandardItem *pa, int row)
{
    const auto &nodes=TreeNodeManager::getInstance()->getNodes();
    auto item=new QStandardItem(nodes[nodeId]->getName());
    item->setData(nodeId);
    item->setEditable(false);
    auto desItem=new QStandardItem(nodes[nodeId]->getDescription());
    desItem->setData(nodeId);
    desItem->setEditable(false);
    if(pa!=nullptr){
        pa->setChild(row,0,item);
        pa->setChild(row,1,desItem);
    }
    return item;
}
