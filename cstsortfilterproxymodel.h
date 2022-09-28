#ifndef CSTSORTFILTERPROXYMODEL_H
#define CSTSORTFILTERPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>
#include <QTableView>

class CstSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit CstSortFilterProxyModel(QTableView *parent = nullptr);

signals:
protected:
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    int prevSort;
};

#endif // CSTSORTFILTERPROXYMODEL_H
