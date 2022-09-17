#include "tableform.h"
#include "ui_tableform.h"
#include "metadatamanager.h"
#include <QSqlRecord>
#include <QSqlQueryModel>


TableForm::TableForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableForm)
{
    ui->setupUi(this);

}

TableForm::~TableForm()
{
    delete ui;
}

void TableForm::reload()
{
    auto &conn=MetaDataManager::getInstance()->getDbConn();
    auto qryModel=new QSqlQueryModel(this);
    qryModel->setQuery("SELECT * from papers ORDER BY id",conn);
    auto records=conn.record("papers");
    for(int i=0;i<records.count();i++){
        qryModel->setHeaderData(i,Qt::Horizontal,records.fieldName(i));
    }

    auto theSelection=new QItemSelectionModel(qryModel);
    ui->tableView->setModel(qryModel);
    ui->tableView->setSelectionModel(theSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}
