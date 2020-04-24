#include "v_stat_text.h"
#include "qpfunction.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_StaticText,"Текст","VISUAL Простые элементы")

REGISTERPROPERTY(v_StaticText,SH, "Текст размер","В пикселях","")
REGISTERPROPERTY(v_StaticText,SIZEALLIGNRECT, "Рамка размер","В сетке","")
REGISTERPROPERTY(v_StaticText,ISSHOWRECT, "Показывать рамку","Показывать рамку","")
REGISTERPROPERTY(v_StaticText,SH_IN_RECT, "Текст размер под рамку","Подогнать размер текста под высоту рамки","")



v_StaticText::v_StaticText(v_Base *parent) :
    v_Model(parent)
{
    FSH=12;
    setObjectName("ТЕКСТ");
    FSIZEALLIGNRECT=QSizeF(6,2);
    FISSHOWRECT=false;
    FFONT=QFont("Times", 12-1);
    FCOLOR_RECT=Qt::black;
    FCOLOR_TEXT=Qt::black;
    boundrect_text.setSize(QSizeF(12*4*10,12));
    FFLAGS=MVP_Enums::TTextFlag::AlignCenter;
    FSH_IN_RECT=false;
    FROUND_RECT=false;
    addCOMMON_PROPERTY("SH","TEXTH");
    calculateGeometry();


}




void v_StaticText::d_paint(QPainter *painter, const QStyleOptionGraphicsItem * )
{
    if (FISSHOWRECT) drawRect(painter);
    drawText(painter);
}

void v_StaticText::drawRect(QPainter *painter)
{
        painter->setPen(QPen(QBrush(getRectColor()),1,Qt::SolidLine));
        painter->setBrush(QBrush(getBrusColor()));
        if (!FROUND_RECT) painter->drawRect(allign_rect); else
                          painter->drawRoundedRect(allign_rect, allign_rect.height()/5, allign_rect.height()/5);

}

void v_StaticText::drawText(QPainter *painter)
{
    RTEXTH>0 ? FFONT.setPointSizeF(RTEXTH):FFONT.setPointSizeF(1);
    painter->setPen(QPen(QBrush(getTextColor()),1,Qt::SolidLine));
    painter->setFont(FFONT);
    int flags=FFLAGS;
    painter->drawText(allign_rect,flags,getText(),&boundrect_text);
}




void v_StaticText::calculateGeometry()
{
    allign_rect=QRectF(0,0,FSIZEALLIGNRECT.width()*gridDX(),FSIZEALLIGNRECT.height()*gridDY());
    if (allign_rect.size().isNull()) allign_rect=QRectF(0,0,1,1);
    allign_rect.moveCenter(QPointF(0,0));
    if ((FSH_IN_RECT)){
//        RTEXTH=QPFunction::setFontSizeInRect(FFONT,allign_rect, objectName(), allign_rect.height(),1) ;
        RTEXTH=allign_rect.height()-2;
    } else {
        RTEXTH=FSH;//viewProperty_int(groupProp_TEXTH,FSH);
    }
    if (RTEXTH<=0) RTEXTH=1;
    FFONT.setPointSizeF(RTEXTH);
    _boundingRect=allign_rect;
//    QFontMetrics fm(FFONT);
//    int flags=FFLAGS;
//    boundrect_text=fm.boundingRect(allign_rect.toRect(),flags,objectName());
//    {
//        QRectF R=boundrect_text;
//        R=R.united(allign_rect);
//        _boundingRect=R.adjusted(-1,-1,+1,+1);
//    }
}


