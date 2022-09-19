#include "metadatamanager.h"
#include <QSqlQuery>
#include <QDebug>
#include "config.h"
#include <QSqlRecord>

MetaDataManager::MetaDataManager(QObject *parent)
    : QObject{parent}
{

}

void MetaDataManager::reload()
{
    if(dbConn.isOpen())
    {
        dbConn.close();
        dbConn=QSqlDatabase();
        QSqlDatabase::removeDatabase("papers");
    }
    dbConn=QSqlDatabase::addDatabase("QSQLITE","papers");
    dbConn.setDatabaseName(Config::getPaperMetaPath() + "papers.db");
    dbConn.open();
    dbConn.exec("create table if not exists papers (id INTEGER PRIMARY KEY, title TEXT NOT NULL)");
    insertRecordIfNotExist("chinese_title","TEXT");
    insertRecordIfNotExist("conference","TEXT");
    insertRecordIfNotExist("year","INTEGER");
    insertRecordIfNotExist("document_link","TEXT");
    insertRecordIfNotExist("code_link","TEXT");
    insertRecordIfNotExist("abstract","TEXT");
    insertRecordIfNotExist("chinese_abstract","TEXT");
    insertRecordIfNotExist("note_link","TEXT");
    insertRecordIfNotExist("concept_node_ids","TEXT");
}

void MetaDataManager::saveAndQuit()
{
}

void MetaDataManager::insertRecordIfNotExist(const QString &recordName, const QString &tp)
{
    auto records=dbConn.record("papers");
    if(records.contains(recordName))
        return ;
    dbConn.exec(QString("ALTER TABLE papers ADD COLUMN %1 %2").arg(recordName,tp));
}



//, conference TEXT, year INTEGER, document_link TEXT, code_link TEXT,
