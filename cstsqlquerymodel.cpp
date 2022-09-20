#include "cstsqlquerymodel.h"
#include <QDebug>
#include "metadatamanager.h"
#include "utils.h"
#include "tree.h"

CstSqlQueryModel::CstSqlQueryModel(const QString &sqlQuery, QObject *parent)
    : QSqlQueryModel{parent}
{
    setQueryInDefault(sqlQuery);
}

QVariant CstSqlQueryModel::data(const QModelIndex &item, int role) const
{
    auto temp=QSqlQueryModel::data(item,role);
    if(headerData(item.column(),Qt::Horizontal).toString()==tr("Concepts")&&role == Qt::DisplayRole)
    {
        auto l=stringToListInt(temp.toString());
        QStringList result;
        for(auto i:qAsConst(l))
            result.append(TreeNodeManager::getInstance()->getNodes()[i]->getName());
        return result.join(',');
    }
    return temp;
}

void CstSqlQueryModel::setQueryInDefault(const QString &qry)
{
    setQuery(qry,MetaDataManager::getInstance()->getDbConn());
}

