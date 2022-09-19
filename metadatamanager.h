#ifndef METADATAMANAGER_H
#define METADATAMANAGER_H

#include <QObject>
#include <QApplication>
#include <QSqlDatabase>

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


signals:
protected:
    inline static MetaDataManager *instance=nullptr;
    QSqlDatabase dbConn;
};

#endif // METADATAMANAGER_H
