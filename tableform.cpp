#include "tableform.h"
#include "ui_tableform.h"
#include "metadatamanager.h"
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include "cstsqlquerymodel.h"
#include "tableeditdialog.h"

TableForm::TableForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableForm)
{
    ui->setupUi(this);
    ui->tableView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setMaximumSectionSize(500);
    ui->tableView->verticalHeader()->setDefaultSectionSize(20);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(ui->tableView,&QTableView::doubleClicked,this,[this](const QModelIndex &index){
        auto columnTitle=ui->tableView->model()->headerData(index.column(),Qt::Horizontal).toString();
        if(columnTitle.right(4)=="Link"){
            QDesktopServices::openUrl(QUrl::fromEncoded(index.data().toString().toUtf8()));
        }
    });

    connect(ui->addBtn,&QToolButton::clicked,this,[this](){
        TableEditDialog dialog(MetaData{MetaDataManager::getInstance()->getMaxOfIds()+1},this);
        dialog.setWindowTitle(qApp->applicationName()+" - Table Editor");
        while(dialog.exec()==QDialog::Accepted)
        {
            MetaData data=dialog.getCollectedData();
            if(MetaDataManager::getInstance()->addMetaData(data))
            {
                qobject_cast<CstSqlQueryModel *>(ui->tableView->model())->setQueryInDefault("SELECT * from papers ORDER BY id");
                break;
            }
        }
    });

    connect(ui->removeBtn,&QToolButton::clicked, this, [this](){
        auto rowList=ui->tableView->selectionModel()->selectedRows(0);
        if(rowList.isEmpty())
            return ;
        if(QMessageBox::warning(this,qApp->applicationName()+" - Deleting",tr("Are you sure to delete %1 papers?").arg(rowList.size()),QMessageBox::Ok|QMessageBox::Cancel)==QMessageBox::Ok)
        {
            for(auto &i:qAsConst(rowList))
                MetaDataManager::getInstance()->removeMetaData(i.data().toInt());
            qobject_cast<CstSqlQueryModel *>(ui->tableView->model())->setQueryInDefault("SELECT * from papers ORDER BY id");
        }
    });

    connect(ui->editBtn,&QToolButton::clicked,this,[this](){
        auto rowList=ui->tableView->selectionModel()->selectedRows(0);
        if(rowList.isEmpty()||rowList.size()>1)
            return ;
        int rowId=rowList.at(0).data().toInt();
        TableEditDialog dialog(MetaDataManager::getInstance()->queryRowAtId(rowId),this);
        if(dialog.exec()==QDialog::Rejected)
            return ;
        auto data=dialog.getCollectedData();
        MetaDataManager::getInstance()->modifyMetaData(data);
        qobject_cast<CstSqlQueryModel *>(ui->tableView->model())->setQueryInDefault("SELECT * from papers ORDER BY id");
    });
}

TableForm::~TableForm()
{
    delete ui;
}

void TableForm::reload()
{
    auto &conn=MetaDataManager::getInstance()->getDbConn();
    auto records=conn.record("papers");
    QStringList fieldNames;
    for(int i=0;i<records.count();i++)
        fieldNames+=records.fieldName(i);
    auto qryModel=new CstSqlQueryModel("SELECT * from papers ORDER BY id" , ui->tableView);
    int conceptsPos=0;
    for(int i=0;i<fieldNames.size();i++){
        auto title=mapFieldNameToDisplayName(fieldNames.at(i));
        qryModel->setHeaderData(i,Qt::Horizontal,title);
        if(title.right(4)=="Link")
            qryModel->setHeaderData(i,Qt::Horizontal,QColor(Qt::darkBlue),Qt::ForegroundRole);
        if(title=="Concepts")
            conceptsPos=i;
    }
    replaceModel(qryModel);
    ui->tableView->horizontalHeader()->moveSection(conceptsPos,5);
}

QString TableForm::mapFieldNameToDisplayName(const QString &fieldName)
{
    if(fieldName=="concept_node_ids")
        return tr("Concepts");
    auto splitAndToCamelCase=[](const QString &s)
    {
        QStringList parts = s.split('_', Qt::SkipEmptyParts);
        for (int i = 0; i < parts.size(); ++i)
            parts[i].replace(0, 1, parts[i][0].toUpper());
        return parts.join(" ");
    };
    return tr(qPrintable(splitAndToCamelCase(fieldName)));
}

void TableForm::showEvent(QShowEvent *ev)
{
    QWidget::showEvent(ev);
    //maybe can make column width changable here
}

void TableForm::replaceModel(QSqlQueryModel *_model)
{
    auto oriModel=ui->tableView->model();
    auto oriSelectionModel=ui->tableView->selectionModel();
    ui->tableView->setModel(_model);
    ui->tableView->setSelectionModel(new QItemSelectionModel(_model));
    if(oriModel!=nullptr){
        oriModel->deleteLater();
        oriSelectionModel->deleteLater();
    }
}
