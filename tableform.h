#ifndef TABLEFORM_H
#define TABLEFORM_H

#include <QWidget>

namespace Ui {
class TableForm;
}

class TableForm : public QWidget
{
    Q_OBJECT

public:
    explicit TableForm(QWidget *parent = nullptr);
    ~TableForm();

    void reload();

private:
    Ui::TableForm *ui; 
};

#endif // TABLEFORM_H
