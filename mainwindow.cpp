#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QToolButton>
#include "globalsettings.h"
#include "config.h"
#include "tree.h"
#include <QFileDialog>
//#include "tabform.h"
//#include "treenodemanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadOpenDirHistory();

    cstBtnGroup=new CstButtonGroup(CstButtonGroup::South,QColor(21,156,119),this);
    ui->gridLayout->addWidget(cstBtnGroup);
    cstBtnGroup->addButton(new QToolButton());
    cstBtnGroup->addButton(new QToolButton());
    tableForm=new TableForm();
    treeForm=new TreeForm();
    currentForm=tableForm;
    ui->gridLayout->addWidget(currentForm,1,0);

    connect(ui->actionOpen_Dir_O,&QAction::triggered,this,[this](){
        auto dirName=QFileDialog::getExistingDirectory();
        if(!dirName.isEmpty())
        {
            dirName+="/";
            setCurrentDir(dirName);
            QAction *action=nullptr;
            auto menu=ui->actionRecent_Dirs_R->menu();
            if(historyDirs.contains(dirName))
            {
                int index=historyDirs.indexOf(dirName);
                action=menu->actions().at(index);
                historyDirs.removeAt(index);
                menu->removeAction(action);
            }
            else
                action=new QAction(dirName);

            historyDirs.push_front(dirName);
            menu->insertAction(menu->actions().isEmpty()?nullptr:menu->actions().at(0),action);
        }
    });

    connect(cstBtnGroup,&CstButtonGroup::buttonChanged,this,[this](int index){
        QWidget *nextForm=nullptr;
        if(index==0)
            nextForm=tableForm;
        else
            nextForm=treeForm;
        currentForm->hide();
        ui->gridLayout->replaceWidget(currentForm,nextForm);
        nextForm->show();
        playSwitchAnimation(nextForm,index==0);
        currentForm=nextForm;
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reload()
{
    tableForm->reload();
    treeForm->reload();
}

void MainWindow::loadOpenDirHistory()
{
    auto settings=GlobalSettings::getInstance();
    historyDirs=settings->value("OPEN_DIR_HISTORY").toStringList();
    auto menu=new QMenu();
    ui->actionRecent_Dirs_R->setMenu(menu);
    for(auto &d:qAsConst(historyDirs))
    {
        auto *action=new QAction(d);
        connect(action,&QAction::triggered,this,[this,action,menu](){
            setCurrentDir(action->text());
            int index=historyDirs.indexOf(currentDir);
            historyDirs.removeAt(index);
            historyDirs.push_front(currentDir);
            menu->removeAction(action);
            menu->insertAction(menu->actions().isEmpty()?nullptr:menu->actions().at(0),action);
        });
        menu->addAction(action);
    }
}

void MainWindow::saveOpenDirHistory()
{
    auto settings=GlobalSettings::getInstance();
    settings->setValue("OPEN_DIR_HISTORY",historyDirs);
}

void MainWindow::playSwitchAnimation(QWidget *page, bool direction)
{
    auto finalGeometry=page->geometry();
    page->move(finalGeometry.topLeft()+QPoint(direction?-200:200,0));
    auto formerGeometry = page->geometry();
    auto appearAnimation = new QPropertyAnimation(page, "geometry");
    appearAnimation->setDuration(200);
    appearAnimation->setStartValue(formerGeometry);
    appearAnimation->setEndValue(finalGeometry);
    appearAnimation->start();
}

void MainWindow::setCurrentDir(const QString &_dir)
{
    currentDir=_dir;
    Config::setDataRoot(currentDir);
    //    TreeNodeManager::
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
    saveOpenDirHistory();
    QMainWindow::closeEvent(ev);
}



