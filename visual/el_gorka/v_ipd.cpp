#include "v_ipd.h"

#include "qpfunction.h"
#include "v_rc.h"


#include "mvp_objectfactory.h"
REGISTERELEMENT(v_IPD,"ИПД","VISUAL ГОРКА")


v_IPD::v_IPD(v_Base *parent) : v_RTDS(parent)
{
}

void v_IPD::d_paint(QPainter *painter, const QStyleOptionGraphicsItem * )
{
    
    QColor clrB=QColor();

    if (FSTATE_33) {
        clrB=getColor(Color_state_33);
    } else {
        if (FSTATE_SRAB) clrB=getColor(v_RC::Color_rc_busy);   else
            clrB=Qt::gray;
    }
    painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));
    QBrush B=QBrush(clrB);
    if (clrB.isValid()) painter->setBrush(clrB); else
        painter->setBrush(Qt::NoBrush);
    painter->drawRect(RC);
}

void v_IPD::calculateGeometry()
{
    S=FSHRC*2;//viewProperty_int(v_RC::groupProp_SHRC,FSHRC)*2;
    M[0]=QPointF(0,0);
    RC=QRectF(0,0,S,S);
    RC.moveCenter(M[0]);
    _boundingRect=RC.adjusted(-4,-4,4,4);
}


