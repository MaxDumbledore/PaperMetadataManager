#include "tableeditdialog.h"
#include "ui_tableeditdialog.h"
#include "tree.h"
#include <QDebug>

TableEditDialog::TableEditDialog(const MetaData &data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableEditDialog)
{
    //    qDebug()<<oriData.year;
    ui->setupUi(this);
    connect(ui->conceptRadioCard,&QRadioButton::toggled,this,[this](bool checked){
        if(!checked)
            return ;
        ui->conceptLineEdit->setEnabled(false);
        ui->conceptCardBox->setEnabled(true);
        ui->conceptRadioManual->setChecked(false);
    });
    connect(ui->conceptRadioManual,&QRadioButton::toggled,this,[this](bool checked){
        if(!checked)
            return ;
        ui->conceptLineEdit->setEnabled(true);
        ui->conceptCardBox->setEnabled(false);
        ui->conceptRadioCard->setChecked(false);
    });
    connect(ui->filterBtn,&QPushButton::clicked, this, [this](){
        auto text=ui->abstractTextEdit->toPlainText();
        text.replace('\n',' ');
        text.remove('\r');
        text.remove((char)2);
        ui->abstractTextEdit->setText(text);
    });

    connect(ui->titleLineEdit,&QLineEdit::editingFinished,this,[this](){
        auto text=ui->titleLineEdit->text();
        text.remove((char)2);
        ui->titleLineEdit->setText(text);
    });

    ui->conceptRadioManual->setChecked(true);

    ui->abstractTextEdit->setText(data.abstract);
    ui->cnAbstractTextEdit->setText(data.chinese_abstract);
    ui->cnTitleLineEdit->setText(data.chinese_title);
    ui->codeLineEdit->setText(data.code_link);
    ui->conceptLineEdit->setText(resolveStringFromConcepts(data.concept_node_ids));
    ui->confLineEdit->setText(data.conference);
    ui->docLineEdit->setText(data.document_link);
    ui->idLineEdit->setText(QString("%1").arg(data.id));
    ui->noteLineEdit->setText(data.note_link);
    ui->remarkLineEdit->setText(data.remarks);
    ui->titleLineEdit->setText(data.title);
    if(data.year!=-1)
        ui->yearLineEdit->setText(QString("%1").arg(data.year));
}

TableEditDialog::~TableEditDialog()
{
    delete ui;
}

QList<int> TableEditDialog::resolveConceptsFromString(QString s)
{
    auto sl=s.split(',');
    QList<int> result;
    for(int i=0;i<sl.size();i++)
    {
        int nodeId=-1;
        if(sl.at(i).right(1)==')')
        {
            int pos=sl.at(i).indexOf('(');
            nodeId=sl.at(i).midRef(pos+1,sl.at(i).size()-pos-2).toInt();
            if(nodeId!=-1)
                result.append(nodeId);
            continue;
        }
        if(s.isEmpty())
            continue;
        //        qDebug(/)<<s;
        result.append(TreeNodeManager::getInstance()->findFirstContainedByNodeName(TreeNodeManager::getInstance()->getRootId(),s));
    }
    return result;
}

QString TableEditDialog::resolveStringFromConcepts(const QList<int> &l)
{
    QStringList s;
    for(auto i:qAsConst(l))
    {
        auto name=TreeNodeManager::getInstance()->getNodes()[i]->getName();
        s+=name+QString("(%1)").arg(i);
    }
    return s.join(',');
}

MetaData TableEditDialog::getCollectedData()
{
    MetaData data;
    data.abstract=ui->abstractTextEdit->toPlainText();
    data.chinese_abstract=ui->cnAbstractTextEdit->toPlainText();
    data.chinese_title=ui->cnTitleLineEdit->text();
    data.code_link=ui->codeLineEdit->text();
    data.concept_node_ids=resolveConceptsFromString(ui->conceptLineEdit->text());
    data.conference=ui->confLineEdit->text();
    data.document_link=ui->docLineEdit->text();
    data.id=ui->idLineEdit->text().toInt();
    data.note_link=ui->noteLineEdit->text();
    data.remarks=ui->remarkLineEdit->text();
    data.title=ui->titleLineEdit->text();
    if(!ui->yearLineEdit->text().isEmpty())
        data.year=ui->yearLineEdit->text().toInt();
    return data;
}
