#include "tableeditdialog.h"
#include "ui_tableeditdialog.h"
#include "tree.h"

TableEditDialog::TableEditDialog(const MetaData &oriData, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableEditDialog)
{
    ui->setupUi(this);
    ui->labelId->setText(QString("%1").arg(oriData.id));
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
    ui->conceptRadioManual->setChecked(true);
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
        }
        if(nodeId!=-1)
            result.append(nodeId);
        result.append(TreeNodeManager::getInstance()->findFirstContainedByNodeName(TreeNodeManager::getInstance()->getRootId(),s));
    }
    return result;
}

MetaData TableEditDialog::getCollectedData()
{
    MetaData data;
    data.abstract=ui->abstractTextEdit->toPlainText();
    data.chinese_abstract=ui->cnAbstractTextEdit->toPlainText();
    data.chinese_title=ui->cnTitleLineEdit->text();
    data.concept_node_ids=resolveConceptsFromString(ui->conceptLineEdit->text());
    data.conference=ui->confLineEdit->text();
    data.document_link=ui->docLineEdit->text();
    data.id=ui->labelId->text().toInt();
    data.note_link=ui->noteLineEdit->text();
    data.remarks=ui->remarkLineEdit->text();
    data.title=ui->titleLineEdit->text();
    data.year=ui->confLineEdit->text().toInt();
    return data;
}
