#ifndef CSTBUTTONGROUP_H
#define CSTBUTTONGROUP_H

#include <QObject>
#include <QWidget>
#include <QButtonGroup>
#include <QPropertyAnimation>
#include <QToolButton>

class CstButtonGroup:public QWidget
{
    Q_OBJECT

    Q_PROPERTY(int offset READ getOffset WRITE setOffset USER false)
public:
    enum Position
    {
        North,South,East,West
    };

    explicit CstButtonGroup(Position p, QColor c, QWidget *parent=nullptr);

    int getOffset() const;

    void addButton(QToolButton *btn);

signals:
    void buttonChanged(int id);

public slots:
    void setOffset(int offset);

private slots:
    void onButtonClicked(int index);

private:
    QButtonGroup *btnGroup;
    QPropertyAnimation *animation;
    Position btnPosition;
    QColor lineColor;
    int lineHeight;
    int preIndex,curIndex;
    int btnCount;
    int m_offset;

    void paintEvent(QPaintEvent *event) override;

    void createLayout();

    int getPrefixOffsetSum(int pos);
};

#endif // CSTBUTTONGROUP_H
