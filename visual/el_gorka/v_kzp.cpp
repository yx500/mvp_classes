#include "v_kzp.h"
#include "v_rc_gor_park.h"
#include "qpfunction.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_KZP,"Контроллер КЗП","VISUAL ГОРКА")


v_KZP::v_KZP(v_Base *parent) : v_Model(parent)
{
    FSIZE=QSize(2,2);
    FTEXT_HEIGHT=4;
    resetStates();
}

void v_KZP::resetStates()
{
    v_Model::resetStates();
    FSTATE_D=0;
    FSTATE_E=0;
    FSTATE_V=0;
    FSTATE_P=0;

}


void v_KZP::calculateGeometry()
{
    qreal w=FSIZE.width()*gridDX();
    qreal h=FSIZE.height()*gridDY();
    R=QRectF(0,0,w,h);
    R.moveCenter(QPointF(0,0));
    rect_text=QRectF(0,0,w,FTEXT_HEIGHT);
    rect_text.moveCenter(QPointF(0,-h/2.-rect_text.height()/2.));
    rect_D=QRectF(0,0,w*0.9,h/2.1);
    rect_V=rect_D;
    rect_D.moveCenter(QPointF(0.,-h/4));
    rect_V.moveCenter(QPointF(0.,+h/4));

    {
        QPolygonF P=QPolygonF(R);
        P=P.united(rect_text);
        _boundingRect=P.boundingRect();
    }


}


void v_KZP::d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    QColor clrP=Qt::gray;
    QColor clrB=Qt::green;
    if (FSTATE_33){
        clrB=getColor(Color_state_33);
        clrP=Qt::gray;
    } else {
        if (FSTATE_E) {
            clrP=Qt::black;
            clrB=Qt::red;
        }
    }

    QFont font("Times",1);
    int flags=Qt::AlignCenter;

    if (FTEXT_HEIGHT>0){
        font.setPointSizeF(FTEXT_HEIGHT);
        painter->setFont(font);
        painter->setPen(QPen(QBrush(Qt::black),1,Qt::SolidLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawText(rect_text,flags,objectName());
    }

    if (option->levelOfDetailFromTransform(painter->worldTransform())>FLEVELDETAIL){
        painter->setPen(QPen(QBrush(clrP),0,Qt::SolidLine));
        painter->setBrush(clrB);
        painter->drawRect(R);
        painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));

        if (rect_D.height()>0){
            font.setPointSizeF(rect_D.height()*0.7);
            painter->setFont(font);

            painter->setBrush(Qt::NoBrush);
            painter->drawRect(rect_D);
            painter->drawText(rect_D,flags,QString::number(FSTATE_D));
        }
        painter->drawRect(rect_V);
        if (FSTATE_E==0)
            painter->drawText(rect_V,flags,QString::number(FSTATE_V,'f',2)); else
            painter->drawText(rect_V,flags,QString("E%1").arg(FSTATE_E));

    } else {
        painter->setPen(QPen(QBrush(clrP),0,Qt::SolidLine));
        painter->setBrush(clrB);
        painter->drawRect(R);
    }
}

void v_KZP::updateStatesFromModel()
{
    v_Model::updateStatesFromModel();
    SET_STATE_MODEL_INT(STATE_D);
    SET_STATE_MODEL_INT(STATE_E);
    SET_STATE_MODEL_QREAL(STATE_V);
    SET_STATE_MODEL_INT(STATE_P);
}
