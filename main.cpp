#include "mainwindow.h"

#include <QApplication>
#include "config.h"
#include "treenode.h"
#include "tester.h"
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setStyle(QStyleFactory::create("fusion"));
    QApplication a(argc, argv);
//    testTreeNode();
//    MainWindow w;
//    w.show();
    test();
    return a.exec();
}
