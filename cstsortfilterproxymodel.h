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
    enum FilterMode{
        SUB_STRING,
        KEY_VALUE
    };

    void setFilterMode(FilterMode newFilterMode);
    void setKeyValues(const QList<QStringList> &newKeyValues);

signals:
protected:
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    int prevSort;
    FilterMode filterMode;
    QMap<QString,QPair<QString,QString>> keyValues;
};

#endif // CSTSORTFILTERPROXYMODEL_H
