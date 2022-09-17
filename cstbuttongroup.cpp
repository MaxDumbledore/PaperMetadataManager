#include "cstbuttongroup.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QDebug>

CstButtonGroup::CstButtonGroup(Position p, QColor c, QWidget *parent):
    QWidget(parent),
    btnPosition(p),
    lineColor(c),
    lineHeight(2),
    preIndex(-1),curIndex(-1),
    btnCount(),
    m_offset()
{
    //setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    btnGroup=new QButtonGroup(this);
    connect(btnGroup, &QButtonGroup::idClicked,this,&CstButtonGroup::onButtonClicked);

    animation=new QPropertyAnimation(this,"offset",this);
    animation->setDuration(200);

    createLayout();
}

int CstButtonGroup::getOffset() const
{
    return m_offset;
}

void CstButtonGroup::addButton(QToolButton *btn)
{
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    btn->setCheckable(true);
    //btn->setFixedSize(80,32);
    btnGroup->addButton(btn, btnCount++);
    layout()->addWidget(btn);

    QString style = QString("QToolButton{background-color:transparent;border:none; }"
            "QToolButton:hover{background-color:rgba(%1, %2, %3, 40);}"
            "QToolButton:pressed, QToolButton:checked{background-color:rgba(%1, %2, %3, 80);}")
            .arg(lineColor.red()).arg(lineColor.green()).arg(lineColor.blue());

    btn->setStyleSheet(style);

    if(btnCount==1)
        btn->click();
}

void CstButtonGroup::createLayout()
{
    QLayout *layout=nullptr;
    switch (btnPosition) {
    case North:
    {
        layout = new QHBoxLayout();
        layout->setContentsMargins(0, lineHeight, 0, 0);
        break;
    }
    case South:
    {
        layout = new QHBoxLayout();
        layout->setContentsMargins(0, 0, 0, lineHeight);
        break;
    }
    case East:
    {
        layout = new QVBoxLayout();
        layout->setContentsMargins(0, 0, lineHeight, 0);
        break;
    }
    case West:
    {
        layout = new QVBoxLayout();
        layout->setContentsMargins(lineHeight, 0, 0, 0);
        break;
    }
    default:;
    }
    layout->setSpacing(0);
    setLayout(layout);
}

int CstButtonGroup::getPrefixOffsetSum(int pos)
{
    if(pos==-1)
        return 0;
    int sum=0;
    if(btnPosition==East||btnPosition==West)
    {
        for(int i=0;i<pos;i++)
            sum+=btnGroup->buttons().at(i)->height();
    }
    else
    {
        for(int i=0;i<pos;i++)
            sum+=btnGroup->buttons().at(i)->width();
    }
    return sum;
}

void CstButtonGroup::setOffset(int offset)
{
    m_offset = offset;
    update();
}

void CstButtonGroup::onButtonClicked(int index)
{
    if(index==curIndex)
        return ;

    preIndex = curIndex;
    curIndex = index;

    switch (btnPosition) {
    case North:
    case South:
    {
        animation->setStartValue(getPrefixOffsetSum(preIndex));
        animation->setEndValue(getPrefixOffsetSum(curIndex));
        break;
    }
    case East:
    case West:
    {
        animation->setStartValue(getPrefixOffsetSum(preIndex));
        animation->setEndValue(getPrefixOffsetSum(curIndex));
        break;
    }
    default:;
    }
    animation->start();
    emit buttonChanged(curIndex);
}

void CstButtonGroup::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    if(!btnCount)
        return ;
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(lineColor);

    auto btnSize=btnGroup->button(curIndex)->size();  //For fixed Width

    switch (btnPosition) {
    case North:
        painter.drawRect(m_offset, 0, btnSize.width(), lineHeight);
        break;
    case South:
        painter.drawRect(m_offset, this->height() - lineHeight, btnSize.width(), lineHeight);
        break;
    case East:
        painter.drawRect(this->width() - lineHeight, m_offset, lineHeight, btnSize.height());
        break;
    case West:
        painter.drawRect(0, m_offset, lineHeight, btnSize.height());
        break;
    default: ;
    }
}
