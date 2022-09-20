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
    dbConn.exec("create table if not exists papers (id INTEGER PRIMARY KEY, title TEXT NOT NULL CHECK (LENGTH(title) > 0))");
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

bool MetaDataManager::addMetaData(const MetaData &data)
{
    auto s=QString("INSERT INTO papers (id, title, chinese_title, conference, year, document_link, code_link, abstract, chinese_abstract, note_link, concept_node_ids, remarks) VALUES (%1, '%2', '%3', '%4', %5, '%6', '%7', '%8', '%9', '%10', '%11', '%12')")
            .arg(data.id).arg(data.title).arg(data.chinese_title).arg(data.conference).arg(data.year==-1?"null":QString("%1").arg(data.year)).arg(data.document_link).arg(data.code_link).arg(data.abstract).arg(data.chinese_abstract).arg(data.note_link).arg(listIntToString(data.concept_node_ids)).arg(data.remarks);
    qDebug()<<s;
    auto qry=dbConn.exec(s);
    qDebug()<<qry.lastError();
    return qry.lastError().type()==QSqlError::NoError;
}

void MetaDataManager::removeMetaData(int id)
{
    auto qry=dbConn.exec(QString("DELETE FROM papers WHERE id = %1").arg(id));
    qDebug()<<qry.lastError();
}

MetaData MetaDataManager::queryRowAtId(int id)
{
    auto qry=dbConn.exec(QString("SELECT * from papers where id = %1").arg(id));
    if(!qry.next())
        return {};
    MetaData data;
    data.id=qry.value(0).toInt();
    data.title=qry.value(1).toString();
    data.chinese_title=qry.value(2).toString();
    data.conference=qry.value(3).toString();
    data.year=qry.value(4).toInt();
    data.document_link=qry.value(5).toString();
    data.code_link=qry.value(6).toString();
    data.abstract=qry.value(7).toString();
    data.chinese_abstract=qry.value(8).toString();
    data.note_link=qry.value(9).toString();
    data.concept_node_ids=stringToListInt( qry.value(10).toString());
    data.remarks=qry.value(11).toString();
    return data;
}

bool MetaDataManager::modifyMetaData(const MetaData &data)
{
    QSqlQuery qry(dbConn);
    qry.prepare("UPDATE papers SET title=:title,chinese_title=:chinese_title,conference=:conference,"
                "year=:year,document_link=:document_link,code_link=:code_link,abstract=:abstract,chinese_abstract=:chinese_abstract,"
                "note_link=:note_link,concept_node_ids=:concept_node_ids,remarks=:remarks where id=:id");
    qry.bindValue(":title",data.title);
    qry.bindValue(":chinese_title",data.chinese_title);
    qry.bindValue(":conference",data.conference);
    qry.bindValue(":year",data.year);
    qry.bindValue(":document_link",data.document_link);
    qry.bindValue(":code_link",data.code_link);
    qry.bindValue(":abstract",data.abstract);
    qry.bindValue(":chinese_abstract",data.chinese_abstract);
    qry.bindValue(":note_link",data.note_link);
    qry.bindValue(":concept_node_ids",listIntToString(data.concept_node_ids));
    qry.bindValue(":remarks",data.remarks);
    qry.bindValue(":id",data.id);
    qry.exec();
    qDebug()<<qry.lastError();
    return qry.lastError().type()==QSqlError::NoError;
}



//, conference TEXT, year INTEGER, document_link TEXT, code_link TEXT,
