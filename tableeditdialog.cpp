#include "tableeditdialog.h"
#include "ui_tableeditdialog.h"

TableEditDialog::TableEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableEditDialog)
{
    ui->setupUi(this);
}

TableEditDialog::~TableEditDialog()
{
    delete ui;
}
