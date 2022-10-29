#include "cstsortfilterproxymodel.h"
#include <QDebug>
#include <QTableView>
#include <QHeaderView>

CstSortFilterProxyModel::CstSortFilterProxyModel(QTableView *parent)
    : QSortFilterProxyModel{parent},
      filterMode(FilterMode::SUB_STRING)
{

}

void CstSortFilterProxyModel::sort(int column, Qt::SortOrder order)
{
    auto tableView=qobject_cast<QTableView *>(parent());
    auto header=tableView->horizontalHeader();

    if(header->sortIndicatorOrder()==Qt::AscendingOrder&&header->isSortIndicatorShown()&&prevSort==column)
    {
        header->setSortIndicator(column,Qt::DescendingOrder);
        header->setSortIndicatorShown(false);
        column=-1;
    }
    else
        header->setSortIndicatorShown(true);
    prevSort=column;
    QSortFilterProxyModel::sort(column,order);
}

bool CstSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    int n=sourceModel()->columnCount();
    if(filterMode==FilterMode::SUB_STRING)
    {
        for(int i=0;i<n;i++)
        {
            auto data=sourceModel()->index(source_row,i,source_parent).data(Qt::DisplayRole).toString();
            if(data.contains(filterRegExp()))
                return true;
        }
        return false;
    }
    else
    {
        for(int i=0;i<n;i++)
        {
            auto t=sourceModel()->headerData(i,Qt::Horizontal).toString();
            if(!keyValues.contains(t))
                continue;
            auto data=sourceModel()->index(source_row,i,source_parent).data(Qt::DisplayRole).toString();
            if(keyValues.value(t).first=="contains")
            {
                if(!data.contains(keyValues.value(t).second))
                    return false;
            }
            else if(keyValues.value(t).first=="equals")
            {
                if(data!=keyValues.value(t).second)
                    return false;
            }
        }
        return true;
    }
}

void CstSortFilterProxyModel::setFilterMode(FilterMode newFilterMode)
{
    filterMode = newFilterMode;
}

void CstSortFilterProxyModel::setKeyValues(const QList<QStringList> &newKeyValues)
{
    keyValues.clear();
    for(auto &i:qAsConst(newKeyValues))
        keyValues.insert(i.first(),{i.at(1),i.at(2)});
    setFilterRegExp("temp");
}
