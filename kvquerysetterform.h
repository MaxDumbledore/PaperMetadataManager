#ifndef KVQUERYSETTERFORM_H
#define KVQUERYSETTERFORM_H

#include <QWidget>

namespace Ui {
class KVQuerySetterForm;
}

class KVQuerySetterForm : public QWidget
{
    Q_OBJECT

public:
    explicit KVQuerySetterForm(QWidget *parent = nullptr);
    ~KVQuerySetterForm();

    void setKeys(const QStringList &s);
    void clear();

signals:
    void queryUpdated(const QList<QStringList> &data);
private:
    Ui::KVQuerySetterForm *ui;
    bool eventFilter(QObject *object, QEvent *event) override;
    void collectDataAndEmit();

    bool ignoreEmit;
};

#endif // KVQUERYSETTERFORM_H
