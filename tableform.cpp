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
#include "cstsortfilterproxymodel.h"
#include "cstsqlquerymodel.h"
#include "tableeditdialog.h"
#include "config.h"
#include <QButtonGroup>

TableForm::TableForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableForm)
{
    ui->setupUi(this);

    auto bg=QButtonGroup(this);
    bg.addButton(ui->kvQueryRadioBtn);
    bg.addButton(ui->ssQueryRadioBtn);

    connect(ui->ssQueryRadioBtn,&QRadioButton::toggled,this,[this](){
        ui->kvQuerySetterForm->hide();
        ui->filterLineEdit->show();
    });

    connect(ui->kvQueryRadioBtn,&QRadioButton::toggled,this,[this](){
        ui->filterLineEdit->hide();
        ui->kvQuerySetterForm->show();
    });

    ui->ssQueryRadioBtn->setChecked(true);
    ui->kvQuerySetterForm->hide();

    ui->tableView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    //    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //    ui->tableView->horizontalHeader()->moveSection(conceptsPos,5);

    ui->tableView->horizontalHeader()->setMaximumSectionSize(500);
    ui->tableView->verticalHeader()->setDefaultSectionSize(20);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader() ->setSortIndicatorShown(false);

    connect(ui->tableView,&QTableView::doubleClicked,this,[this](const QModelIndex &index){
        auto columnTitle=ui->tableView->model()->headerData(index.column(),Qt::Horizontal).toString();
        if(columnTitle=="Note Link")
        {
            QDesktopServices::openUrl(QUrl::fromLocalFile(index.data().toString().toUtf8()));
        }
        else if(columnTitle.right(4)=="Link")
        {
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
            bool newFile=false;
            if(data.note_link.isEmpty())
            {
                auto notePath=Config::getNotePath()+fileName+".md";
                data.note_link=notePath;
                newFile=true;
            }
            if(MetaDataManager::getInstance()->addMetaData(data))
            {
                //make note
//                qDebug()<<data.note_link;
                if(newFile)
                {
//                    qDebug()<<data.note_link;

                    auto notePath=Config::getNotePath()+fileName+".md";
                    data.note_link=notePath;
                    QFile file(notePath);
                    if(!file.exists())
                    {
                        file.open(QFile::WriteOnly);
                        file.write(("# "+data.title+"\n").toUtf8());
                        file.write(data.chinese_title.toUtf8()+"\n");
                        file.write(QString("## 摘要\n").toUtf8());
                        file.write(data.chinese_abstract.toUtf8());
                        file.close();
                    }
                }
                model->setQueryInDefault("SELECT * from papers ORDER BY id");
                break;
            }
        }
    });

    connect(ui->removeBtn,&QToolButton::clicked, this, [this](){
        auto rowList=ui->tableView->selectionModel()->selectedRows(0);
        if(rowList.isEmpty())
            return ;
        if(QMessageBox::warning(this,qApp->applicationName()+" - Deleting",QString("Are you sure to delete %1 papers?").arg(rowList.size()),QMessageBox::Ok|QMessageBox::Cancel)==QMessageBox::Ok)
        {
            for(auto &i:qAsConst(rowList))
                MetaDataManager::getInstance()->removeMetaData(i.data().toInt());
            model->setQueryInDefault("SELECT * from papers ORDER BY id");
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
        model->setQueryInDefault("SELECT * from papers ORDER BY id");
    });

    connect(ui->filterLineEdit,&QLineEdit::returnPressed,this,[this](){
        auto model=qobject_cast<CstSortFilterProxyModel *>(ui->tableView->model());
        model->setFilterMode(CstSortFilterProxyModel::SUB_STRING);
        model->setFilterRegExp(ui->filterLineEdit->text());
    });

    connect(ui->kvQuerySetterForm,&KVQuerySetterForm::queryUpdated,this,[this](const QList<QStringList> &data){
        auto model=qobject_cast<CstSortFilterProxyModel *>(ui->tableView->model());
        model->setFilterMode(CstSortFilterProxyModel::KEY_VALUE);
        model->setKeyValues(data);
    });

    connect(ui->resetBtn,&QToolButton::clicked,this,[this](){
        ui->filterLineEdit->clear();
        ui->kvQuerySetterForm->clear();
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
    QStringList displayNames;
    for(int i=0;i<fieldNames.size();i++){
        auto title=mapFieldNameToDisplayName(fieldNames.at(i));
        displayNames.append(title);
        qryModel->setHeaderData(i,Qt::Horizontal,title);
        if(title.right(4)=="Link")
            qryModel->setHeaderData(i,Qt::Horizontal,QColor(Qt::darkBlue),Qt::ForegroundRole);
    }
    replaceModel(qryModel);

    ui->kvQuerySetterForm->setKeys(displayNames);
    //    ui->tableView->setColumnWidth(0,10);
    //    ui->tableView->setColumnWidth(1,500);
    //    ui->tableView->setColumnWidth(2,300);
}

QString TableForm::mapFieldNameToDisplayName(const QString &fieldName)
{
    if(fieldName=="concept_node_ids")
        return "Concepts";
    auto splitAndToCamelCase=[](const QString &s)
    {
        QStringList parts = s.split('_', Qt::SkipEmptyParts);
        for (int i = 0; i < parts.size(); ++i)
            parts[i].replace(0, 1, parts[i][0].toUpper());
        return parts.join(" ");
    };
    return splitAndToCamelCase(fieldName);
}

void TableForm::replaceModel(CstSqlQueryModel *_model)
{
    model=_model;
    auto oriProxyModel=ui->tableView->model();
    auto proxyModel=new CstSortFilterProxyModel(ui->tableView);
    proxyModel->setSourceModel(model);
    model->setParent(proxyModel);
    ui->tableView->setModel(proxyModel);
    ui->tableView->setSelectionModel(new QItemSelectionModel(proxyModel,proxyModel));
    if(oriProxyModel!=nullptr)
        oriProxyModel->deleteLater();
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->columnAt(0);
    //Very Strange, just call columnAt() then setSectionResizeMode will keep the width of ResizeToContents
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    //I initially want to write setColumnWidth(columnWidth) here, but needn't
}
