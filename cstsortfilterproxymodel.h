#ifndef CSTSORTFILTERPROXYMODEL_H
#define CSTSORTFILTERPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class CstSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit CstSortFilterProxyModel(QObject *parent = nullptr);

signals:

};

#endif // CSTSORTFILTERPROXYMODEL_H
