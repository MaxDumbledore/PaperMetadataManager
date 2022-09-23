#include "tableform.h"
#include "ui_tableform.h"
#include "metadatamanager.h"
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QProcess>
#include <QSortFilterProxyModel>
#include "cstsqlquerymodel.h"
#include "tableeditdialog.h"
#include "config.h"

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
    ui->tableView->setSortingEnabled(true);
    connect(ui->tableView->horizontalHeader(),&QHeaderView::sortIndicatorChanged,this,[this](int logicalIndex, Qt::SortOrder order){
        if(logicalIndex==0)
        ui->tableView->horizontalHeader()->setSortIndicator(logicalIndex,order);
    });

    connect(ui->tableView,&QTableView::doubleClicked,this,[this](const QModelIndex &index){
        auto columnTitle=ui->tableView->model()->headerData(index.column(),Qt::Horizontal).toString();
        if(columnTitle=="Note Link")
        {
            QDesktopServices::openUrl(QUrl::fromLocalFile(index.data().toString().toUtf8()));
        }
        else if(columnTitle.right(4)=="Link"){
            QDesktopServices::openUrl(QUrl::fromEncoded(index.data().toString().toUtf8()));
        }
    });

    connect(ui->addBtn,&QToolButton::clicked,this,[this](){
        MetaData data;
        data.id=MetaDataManager::getInstance()->getMaxOfIds()+1;
        TableEditDialog dialog(data,this);
        dialog.setWindowTitle(qApp->applicationName()+" - Table Editor");
        while(dialog.exec()==QDialog::Accepted)
        {
            data=dialog.getCollectedData();

            auto fileName=data.title;
            fileName.replace('\\',' ');
            fileName.replace('/',' ');
            fileName.replace('<',' ');
            fileName.replace('>',' ');
            fileName.replace('?',' ');
            fileName.replace(':',' ');
            fileName.replace('|',' ');
            fileName.replace('*',' ');
            fileName.replace('\"',' ');
            auto notePath=Config::getNotePath()+fileName+".md";
            data.note_link=notePath;
            if(MetaDataManager::getInstance()->addMetaData(data))
            {
                //make note


                QFile file(notePath);
                file.open(QFile::WriteOnly);
                file.write(("# "+data.title+"\n").toUtf8());
                file.write(data.chinese_title.toUtf8()+"\n");
                file.write(QString("## 摘要\n").toUtf8());
                file.write(data.chinese_abstract.toUtf8());
                file.close();
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
    auto model=new QSortFilterProxyModel();
    model->setSourceModel(_model);
    _model->setParent(model);
    ui->tableView->setModel(model);
    ui->tableView->setSelectionModel(new QItemSelectionModel(model));
    if(oriModel!=nullptr){
        oriModel->deleteLater();
        oriSelectionModel->deleteLater();
    }
}
