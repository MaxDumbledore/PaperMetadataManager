#ifndef TABLEEDITDIALOG_H
#define TABLEEDITDIALOG_H

#include <QDialog>

namespace Ui {
class TableEditDialog;
}

class TableEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TableEditDialog(QWidget *parent = nullptr);
    ~TableEditDialog();

private:
    Ui::TableEditDialog *ui;
};

#endif // TABLEEDITDIALOG_H
