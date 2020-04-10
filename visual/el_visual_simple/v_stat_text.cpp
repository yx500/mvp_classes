#include "v_stat_text.h"
#include "qpfunction.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_StaticText,"Текст","VISUAL Простые элементы")

//const VisualGroupProperty_int v_StaticText::groupProp_TEXTH("TEXTH","ВЫСОТА ТЕКСТА",0,12);


v_StaticText::v_StaticText(v_Base *parent) :
    v_Base(parent)
{
    FSH=12;
    setObjectName("ТЕКСТ");
    FSIZEALLIGNRECT=QSizeF(0,0);
    FISSHOWRECT=false;
    FFONT=QFont("Times", 12-1);
    FCOLOR_RECT=Qt::black;
    FCOLOR_TEXT=Qt::black;
    boundrect_text.setSize(QSizeF(12*4*10,12));
    FFLAGS=MVP_Enums::TTextFlag::TextDontClip;
    FSH_IN_RECT=false;
    FROUND_RECT=false;
    addCOMMON_PROPERTY("SH","TEXTH");
    calculateGeometry();


}




void v_StaticText::d_paint(QPainter *painter, const QStyleOptionGraphicsItem * )
{
    painter->setPen(QPen(QBrush(FCOLOR_RECT),1,Qt::SolidLine));
    painter->setBrush(Qt::NoBrush);
    drawRect(painter);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(FCOLOR_TEXT),1,Qt::SolidLine));
    drawText(painter);
}

void v_StaticText::drawRect(QPainter *painter)
{

        if (!FROUND_RECT) painter->drawRect(allign_rect); else
                          painter->drawRoundedRect(allign_rect, allign_rect.height()/5, allign_rect.height()/5);

}

void v_StaticText::drawText(QPainter *painter)
{
    RTEXTH>0 ? FFONT.setPointSizeF(RTEXTH):FFONT.setPointSizeF(1);
    painter->setFont(FFONT);
    int flags=FFLAGS;
    painter->drawText(allign_rect,flags,objectName(),&boundrect_text);
}




void v_StaticText::calculateGeometry()
{
    allign_rect=QRectF(0,0,FSIZEALLIGNRECT.width()*gridDX(),FSIZEALLIGNRECT.height()*gridDY());
    allign_rect.moveCenter(QPointF(0,0));
    if ((FSH_IN_RECT)&&(!allign_rect.size().isNull())){
        RTEXTH=QPFunction::setFontSizeInRect(FFONT,allign_rect, objectName(), allign_rect.height(),1) ;
    } else {
        RTEXTH=FSH;//viewProperty_int(groupProp_TEXTH,FSH);
    }
    if (RTEXTH<=0) RTEXTH=1;
    FFONT.setPointSizeF(RTEXTH);
    QFontMetrics fm(FFONT);
    int flags=FFLAGS;
    boundrect_text=fm.boundingRect(allign_rect.toRect(),flags,objectName());
    {
        QRectF R=boundrect_text;
        R=R.united(allign_rect);
        _boundingRect=R.adjusted(-1,-1,+1,+1);
    }
}


