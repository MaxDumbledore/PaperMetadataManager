#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tabform.h"
#include "treenodemanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tabWidget=new QTabWidget(this);
    ui->gridLayout->addWidget(tabWidget);
    auto roots=TreeNodeManager::getInstance()->getRoots();
    for(int i=0;i<roots.size();i++)
        tabWidget->addTab(new TabForm(),"t");
}

MainWindow::~MainWindow()
{
    delete ui;
}

