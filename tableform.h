#ifndef TABLEFORM_H
#define TABLEFORM_H

#include <QSqlQueryModel>
#include <QWidget>

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

    void showEvent(QShowEvent *ev) override;

    void replaceModel(QSqlQueryModel *_model);

private:
    Ui::TableForm *ui; 
};

#endif // TABLEFORM_H
