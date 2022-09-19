#ifndef CSTSQLQUERYMODEL_H
#define CSTSQLQUERYMODEL_H

#include <QObject>
#include <QSqlQueryModel>

class CstSqlQueryModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit CstSqlQueryModel(QObject *parent = nullptr);

signals:
protected:
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
};

#endif // CSTSQLQUERYMODEL_H
