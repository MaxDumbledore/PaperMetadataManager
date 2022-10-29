#include "kvquerysetterform.h"
#include "ui_kvquerysetterform.h"

#include <QKeyEvent>
#include <QDebug>

KVQuerySetterForm::KVQuerySetterForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KVQuerySetterForm),
    ignoreEmit()
{
    ui->setupUi(this);
    ui->opCombo->addItems({"contains","equals"});
    ui->filtTable->verticalHeader()->setDefaultSectionSize(20);
    ui->filtTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(ui->valueEdit,&QLineEdit::returnPressed,this,[this](){
        if(ui->valueEdit->text().isEmpty())
            return ;
        int n=ui->filtTable->rowCount();
        ignoreEmit=true;
        ui->filtTable->insertRow(n);
        ui->filtTable->setItem(n,0,new QTableWidgetItem(ui->keyCombo->currentText()));
        ui->filtTable->setItem(n,1,new QTableWidgetItem(ui->opCombo->currentText()));
        ui->filtTable->setItem(n,2,new QTableWidgetItem(ui->valueEdit->text()));
        ignoreEmit=false;
        collectDataAndEmit();
    });

    connect(ui->filtTable,&QTableWidget::itemChanged,this,[this](){
        if(!ignoreEmit)
            collectDataAndEmit();
    });

    ui->filtTable->installEventFilter(this);
    connect(ui->pushButton,&QPushButton::clicked,this,[this](){
        collectDataAndEmit();
    });
}

KVQuerySetterForm::~KVQuerySetterForm()
{
    delete ui;
}

void KVQuerySetterForm::setKeys(const QStringList &s)
{
    ui->keyCombo->clear();
    ui->keyCombo->addItems(s);
}

void KVQuerySetterForm::clear()
{
    ui->filtTable->clearContents();
}

bool KVQuerySetterForm::eventFilter(QObject *object, QEvent *event)
{
    if(object==ui->filtTable&&event->type()==QEvent::KeyRelease&&static_cast<QKeyEvent *>(event)->key()==Qt::Key_Delete)
    {
        ignoreEmit=true;
        ui->filtTable->removeRow(ui->filtTable->currentRow());      
        ignoreEmit=false;
        collectDataAndEmit();
    }
    return QWidget::eventFilter(object,event);
}

void KVQuerySetterForm::collectDataAndEmit()
{
    QList<QStringList> data;
    for(int i=0;i<ui->filtTable->rowCount();i++)
    {
        QStringList dataline;
        for(int j=0;j<3;j++)
            dataline<<ui->filtTable->item(i,j)->data(Qt::DisplayRole).toString();
        data<<dataline;
    }
    emit queryUpdated(data);
}
