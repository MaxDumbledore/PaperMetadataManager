#ifndef TABLEEDITDIALOG_H
#define TABLEEDITDIALOG_H

#include <QDialog>
#include "metadatamanager.h"

namespace Ui {
class TableEditDialog;
}

class TableEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TableEditDialog(const MetaData &data,QWidget *parent = nullptr);
    ~TableEditDialog();

    static QList<int> resolveConceptsFromString(QString s);
    static QString resolveStringFromConcepts(const QList<int> &l);

    MetaData getCollectedData();

private:
    Ui::TableEditDialog *ui;
};

#endif // TABLEEDITDIALOG_H
