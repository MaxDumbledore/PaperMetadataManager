#ifndef METADATAMANAGER_H
#define METADATAMANAGER_H

#include <QObject>
#include <QApplication>
#include <QSqlDatabase>

struct MetaData{
    int id=-1;
    QString title;
    QString chinese_title;
    QString conference;
    int year=-1;
    QString document_link;
    QString code_link;
    QString abstract;
    QString chinese_abstract;
    QString note_link;
    QList<int> concept_node_ids;
    QString remarks;
};

class MetaDataManager : public QObject
{
    Q_OBJECT
public:
    explicit MetaDataManager(QObject *parent = nullptr);

    static MetaDataManager *getInstance(){
        if(instance==nullptr)
        {
            instance=new MetaDataManager(qApp);
        }
        return instance;
    }

    void reload();
    void saveAndQuit();

    void insertRecordIfNotExist(const QString &recordName, const QString &tp);
    QSqlDatabase &getDbConn()
    {
        return dbConn;
    }

    int getNumOfRows();
    int getMaxOfIds();

    bool addMetaData(const MetaData &data);
    void removeMetaData(int id);

    MetaData queryRowAtId(int id);
    bool modifyMetaData(const MetaData &data);

signals:
protected:
    inline static MetaDataManager *instance=nullptr;
    QSqlDatabase dbConn;
};

#endif // METADATAMANAGER_H
