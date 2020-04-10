#include "v_ris.h"

#include "m_ris.h"

#include "qpfunction.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_RIS,"РИС","VISUAL ГОРКА")

v_RIS::v_RIS(v_Base *parent) : v_Model(parent)
{
    ris=nullptr;
    calculateGeometry();

    FSIZE=QSize(1,2);
    FSIZE_TEXT=QSize(1,4);
    FTEXT_XY=QPointF(0,-FSIZE.height()/2);

    resetStates();
}

void v_RIS::resetStates()
{
    FSTATE_V=0;
}

void v_RIS::calculateGeometry()
{
    qreal w=FSIZE.width()*gridDX();
    qreal h=FSIZE.height()*gridDY();
    R=QRectF(0,0,w,h);
    R.moveCenter(QPointF(0,0));

    T=QPointF(R.width()/2,0);
    if (FINVERTDRAWX) T=QPointF(-R.width()/2,0); else
        T=QPointF( R.width()/2,0);

    qreal wt=FSIZE_TEXT.width()*gridDX();
    qreal ht=FSIZE_TEXT.height()*gridDY();

    rect_text=QRect(0,0,wt,ht);
    rect_text.moveCenter(QPointF(FTEXT_XY.x()*gridDX(), FTEXT_XY.y()*gridDY()));
    _boundingRect=R;
    _boundingRect=_boundingRect.united(rect_text);

}

void v_RIS::d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    QColor clrP=Qt::black;
    QColor clrB=getColor(Color_FON);

    if (ISEDITORMODE){
        clrB=Qt::gray;
        clrP=Qt::black;
    } else {
        if (FSTATE_33) {
            clrB=getColor(Color_state_33);
        } else {
            if (FSTATE_ERRCS){
                clrB=Qt::red;
            }else
                if (FSTATE_V>0) {
                    clrB=Qt::yellow;
                }else {

                }
        }
    }
    painter->setPen(QPen(QBrush(clrP),1,Qt::SolidLine));
    painter->setBrush(clrB);
    painter->drawRect(R);
    painter->drawEllipse(T,2,2);
    if (option->levelOfDetailFromTransform(painter->worldTransform())>FLEVELDETAIL){
        QFont font("Times",1);
        int flags=Qt::AlignCenter;
        (rect_text.height()*0.7>0) ? font.setPointSizeF(rect_text.height()*0.7):font.setPointSizeF(1);
        painter->setFont(font);
        painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));
        painter->drawText(rect_text,flags,objectName(),&bound_rect_text);

        if (FSTATE_V>0){
            QString S;
            S.sprintf("%2.1f",FSTATE_V);
            (R.height()*0.6>0) ? font.setPointSizeF(R.height()*0.6):font.setPointSizeF(1);
            painter->drawText(R,flags,S);
        }
    }

}

void v_RIS::updateStatesFromModel()
{
    v_Model::updateStatesFromModel();
    m_RIS *ris=qobject_cast<m_RIS *>(modelObject());
    if (ris){
        setSTATE_V(ris->STATE_V());
        if (ris->controllerARS()){
            setSTATE_ERRCS(ris->controllerARS()->STATE_ERRCS());
        }
    }
    SET_STATE_MODEL_QREAL(STATE_V);
}
