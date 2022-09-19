#include "mainwindow.h"

#include <QApplication>
#include "config.h"
#include "tree.h"
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication::setApplicationName(QObject::tr("Paper Metadata Manager"));
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setStyle(QStyleFactory::create("fusion"));
    QApplication a(argc, argv);
//    testTreeNode();
    auto mw=MainWindow::getInstance();
    mw->show();
    if(!mw->getHistoryDirs().isEmpty())
        mw->setCurrentDir(mw->getHistoryDirs().at(0));
//    test();
    return a.exec();
}
