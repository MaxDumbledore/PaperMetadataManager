#ifndef TREEFORM_H
#define TREEFORM_H

#include <QWidget>

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

private:
    Ui::TreeForm *ui;
};

#endif // TREEFORM_H
