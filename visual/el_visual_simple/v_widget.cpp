#include "v_widget.h"

#include <QGraphicsProxyWidget>

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_Widget,"Виджет","VISUAL Простые элементы")


v_Widget::v_Widget(v_Base *parent) : v_Base(parent)
{
    FSIZE=QSizeF(10,10);
    FPROPERTYES="";
    FTYPE="";
    FPROPERTYES="";
    FSCALETOSIZE=true;
    PW=new QGraphicsProxyWidget(proxyGraphicsItem);

}

void v_Widget::setTYPE(QString p)
{
    if (FTYPE!=p){
        FTYPE=p;
    }
}

void v_Widget::calculateGeometry()
{
    rect=QRectF(0,0,FSIZE.width()*gridDX(),FSIZE.height()*gridDY());
    rect.moveCenter(QPointF(0,0));
    _boundingRect=rect;
}

void v_Widget::d_paint(QPainter *painter, const QStyleOptionGraphicsItem * )
{
    if (ISEDITORMODE){
        painter->setBrush(QBrush(Qt::NoBrush));
        painter->setPen(QPen(Qt::black));
        painter->drawRect(rect);
        painter->drawText(rect,Qt::AlignCenter,FTYPE+"\r"+idstr());
    }

}
