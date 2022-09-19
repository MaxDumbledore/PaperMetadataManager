#ifndef TREEFORM_H
#define TREEFORM_H

#include <QWidget>
#include <QStandardItemModel>

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
};

#endif // TREEFORM_H
