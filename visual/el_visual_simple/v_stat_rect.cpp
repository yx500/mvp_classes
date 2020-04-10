#include "v_stat_rect.h"

#include "qpfunction.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_Stat_Rect,"Прямоугольник","VISUAL Простые элементы")

v_Stat_Rect::v_Stat_Rect(v_Base *parent) : v_Base(parent)
{
    FSIZE=QSizeF(10,10);
    FROUND_SIZE=QSizeF(0,0);
    FPEN_COLOR=Qt::black;
    FPEN_STYLE=Qt::SolidLine;
    FBRUSH_STYLE=Qt::NoBrush;
}

void v_Stat_Rect::d_paint(QPainter *painter, const QStyleOptionGraphicsItem * )
{
    
    QBrush B=QBrush(FBRUSH_COLOR,FBRUSH_STYLE);
    painter->setBrush(B);
    painter->setPen(QPen(QBrush(FPEN_COLOR),0,FPEN_STYLE));
    if ((FROUND_SIZE.width()!=0)&&(FROUND_SIZE.height()!=0)){
        painter->drawRoundedRect(rect,FROUND_SIZE.width(),FROUND_SIZE.height());
    } else {
        painter->drawRect(rect);
    }

}

void v_Stat_Rect::calculateGeometry()
{
    rect=QRectF(0,0,FSIZE.width()*gridDX(),FSIZE.height()*gridDY());
    rect.moveCenter(M[0]);
    {
        _boundingRect=rect.adjusted(-1,-1,+1,+1);
    }
}

