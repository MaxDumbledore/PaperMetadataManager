#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cstbuttongroup.h"
#include "tableform.h"
#include "treeform.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static MainWindow *getInstance(){
        if(instance==nullptr){
            instance=new MainWindow();
        }
        return instance;
    }

    void reload();
    void saveAndQuit();

    const QStringList &getHistoryDirs() const
    {
        return historyDirs;
    }

    void setCurrentDir(const QString &_dir);

private:
    inline static MainWindow *instance=nullptr;

    Ui::MainWindow *ui;
    QStringList historyDirs;
    QString currentDir;
    CstButtonGroup *cstBtnGroup;
    QWidget *currentForm;
    TableForm *tableForm;
    TreeForm *treeForm;

    void loadOpenDirHistory();
    void saveOpenDirHistory();

    void playSwitchAnimation(QWidget *page, bool direction);

    void closeEvent(QCloseEvent *ev) override;
};
#endif // MAINWINDOW_H
