#include "cstsortfilterproxymodel.h"
#include <QDebug>
#include <QTableView>
#include <QHeaderView>

CstSortFilterProxyModel::CstSortFilterProxyModel(QTableView *parent)
    : QSortFilterProxyModel{parent}
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
    for(int i=0;i<n;i++)
    {
        auto data=sourceModel()->index(source_row,i,source_parent).data(Qt::DisplayRole).toString();
        if(data.contains(filterRegExp()))
            return true;
    }
    return false;
}
