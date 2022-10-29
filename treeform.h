#ifndef TREEFORM_H
#define TREEFORM_H

#include <QWidget>
#include <QStandardItemModel>
#include <QMenu>

namespace Ui {
class TreeForm;
}

class TreeForm : public QWidget
{
    Q_OBJECT

public:
    explicit TreeForm(QWidget *parent = nullptr);
    ~TreeForm();

    void reload();
    void replaceModel(QStandardItemModel *_model);


private:
    Ui::TreeForm *ui;
    QMenu *contextMenu;
    void buildModelItemRecursively(QStandardItem *item);
    QStandardItem * buildItemLine(int nodeId, QStandardItem *pa=nullptr, int row=0);
};

#endif // TREEFORM_H
