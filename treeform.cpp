#include "treeform.h"
#include "ui_treeform.h"

TreeForm::TreeForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TreeForm)
{
    ui->setupUi(this);
}

TreeForm::~TreeForm()
{
    delete ui;
}

void TreeForm::reload()
{

}
