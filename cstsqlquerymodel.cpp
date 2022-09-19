#include "cstsqlquerymodel.h"
#include <QDebug>

CstSqlQueryModel::CstSqlQueryModel(QObject *parent)
    : QSqlQueryModel{parent}
{

}

QVariant CstSqlQueryModel::data(const QModelIndex &item, int role) const
{
    if(headerData(item.column(),Qt::Horizontal).toString()==tr("Concepts")&&role == Qt::DisplayRole)
        return "fw";
    return  QSqlQueryModel::data(item,role);
}

