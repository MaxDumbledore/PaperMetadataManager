#ifndef TABLEFORM_H
#define TABLEFORM_H

#include <QSqlQueryModel>
#include <QWidget>
#include "cstsqlquerymodel.h"

namespace Ui {
class TableForm;
}

class TableForm : public QWidget
{
    Q_OBJECT

public:
    explicit TableForm(QWidget *parent = nullptr);
    ~TableForm();

    void reload();
    static QString mapFieldNameToDisplayName(const QString &fieldName);

    void replaceModel(CstSqlQueryModel *_model);

private:
    Ui::TableForm *ui; 

    CstSqlQueryModel *model;
};

#endif // TABLEFORM_H
