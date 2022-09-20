#include "metadatamanager.h"
#include <QSqlQuery>
#include <QDebug>
#include "config.h"
#include <QSqlRecord>
#include <QSqlError>
#include "utils.h"

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
    insertRecordIfNotExist("remarks","TEXT");
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

int MetaDataManager::getNumOfRows()
{
    auto qry=dbConn.exec("SELECT COUNT(1) from papers");
    qry.next();
    return qry.value(0).toInt();
}

int MetaDataManager::getMaxOfIds()
{
    auto qry=dbConn.exec("SELECT MAX(id) FROM papers");
    if(qry.next())
        return qry.value(0).toInt();
    return 0;
}

void MetaDataManager::addMetaData(const MetaData &data)
{
    qDebug("fw");
    auto s=QString("INSERT INTO papers (id, title, chinese_title, conference, year, document_link, code_link, abstract, chinese_abstract, note_link, concept_node_ids, remarks) VALUES (%1, '%2', '%3', '%4', %5, '%6', '%7', '%8', '%9', '%10', '%11', '%12')")
            .arg(data.id).arg(data.title).arg(data.chinese_title).arg(data.conference).arg(data.year==-1?"null":QString("%1").arg(data.year)).arg(data.document_link).arg(data.code_link).arg(data.abstract).arg(data.chinese_abstract).arg(data.note_link).arg(listIntToString(data.concept_node_ids)).arg(data.remarks);
    qDebug()<<s;
    auto qry=dbConn.exec(s);
    qDebug()<<qry.lastError();
}

void MetaDataManager::removeMetaData(int id)
{
    dbConn.exec(QString("DELETE FROM papers WHERE id = %1").arg(id));
}



//, conference TEXT, year INTEGER, document_link TEXT, code_link TEXT,
