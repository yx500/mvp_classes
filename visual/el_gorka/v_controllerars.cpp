#include "v_controllerars.h"
#include "qpfunction.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_ControllerARS,"Контроллер АРС","VISUAL ГОРКА")

v_ControllerARS::v_ControllerARS(v_Base *parent) : v_Model(parent)
{
    FSIZE=QSize(2,2);
    FTEXT_HEIGHT=4;
    resetStates();
}

void v_ControllerARS::resetStates()
{
    v_Model::resetStates();
    FSTATE_ADDR=false;
    FSTATE_V=0;
    FSTATE_VOLEG=0;
    FSTATE_T1=0;
    FSTATE_T2=0;
    FSTATE_T3=0;
    FSTATE_T4=0;
    FSTATE_R=0;
    FSTATE_RC=0;
    FSTATE_RRC=0;
    FSTATE_ERRCS=0;
    FSTATE_RC_PRED=0;
}



void v_ControllerARS::calculateGeometry()
{
    qreal w=FSIZE.width()*gridDX();
    qreal h=FSIZE.height()*gridDY();
    R=QRectF(0,0,w,h);
    R.moveCenter(QPointF(0,0));
    rect_text=QRectF(0,0,w,FTEXT_HEIGHT);
    rect_text.moveCenter(QPointF(0,-h/2.-rect_text.height()/2.));
    qreal dy=h/8.;
    rect_RRC=QRectF(0,0,w/2.1,dy*1.9);
    rect_ERRCS=rect_RRC;
    rect_RRC.moveCenter(QPointF(-w/4.,-dy*3.));
    rect_ERRCS.moveCenter(QPointF(+w/4.,-dy*3.));
    rect_RRC_B=QRectF(0,0,w/2.5,h/2.5);
    rect_ERRCS_B=rect_RRC_B;
    rect_RRC_B.moveCenter(QPointF(-w/4.,-h/4.));
    rect_ERRCS_B.moveCenter(QPointF(+w/4.,-h/4.));

    rect_RC=QRectF(0,0,w/3.,dy*1.9);
    rect_RC_PRED=rect_RC;
    rect_RC.moveCenter(QPointF(-w/4.,-dy));
    rect_RC_PRED.moveCenter(QPointF(+w/4.,-dy));

    qreal wt=w/6.;
    qreal dt=w/6.;
    for(int i=0;i<5;i++){
        rect_T[i]=QRectF(0,0,wt,dy*1.9);
        rect_T[i].moveCenter(QPointF(-w/2.+1.*(i+1)*dt,dy));
    }

    rect_V=QRectF(0,0,w/2.1,dy*1.9);
    rect_VOLEG=rect_RRC;
    rect_V.moveCenter(QPointF(-w/4.,+dy*3.));
    rect_VOLEG.moveCenter(QPointF(+w/4.,+dy*3.));
    {
        QPolygonF P=QPolygonF(R);
        P=P.united(rect_text);
        _boundingRect=P.boundingRect();
    }

}



void v_ControllerARS::d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    QColor clrP_R=Qt::gray;
    QColor clrB_R=Qt::black;
    QColor clrP_R_B=Qt::gray;
    QColor clrB_R_B=Qt::black;
    if (FSTATE_33){
        clrB_R=getColor(Color_state_33);
        clrB_R_B=clrB_R;
        clrP_R=Qt::gray;
        clrP_R_B=clrP_R;
    } else {
        if (FSTATE_ADDR) {
            clrP_R=Qt::green;
            clrB_R=Qt::white;
            clrP_R_B=Qt::black;
            clrB_R_B=Qt::green;
        } else {
            clrP_R=Qt::black;
            clrB_R=Qt::gray;
            clrP_R_B=Qt::black;
            clrB_R_B=Qt::gray;
        }
    }
    QFont font("Times",1);
    int flags=Qt::AlignCenter;

    (FTEXT_HEIGHT>0) ? font.setPointSizeF(FTEXT_HEIGHT):font.setPointSizeF(1) ;
    painter->setFont(font);
    painter->setPen(QPen(QBrush(Qt::black),1,Qt::SolidLine));
    painter->setBrush(Qt::NoBrush);
    painter->drawText(rect_text,flags,objectName());



    if (option->levelOfDetailFromTransform(painter->worldTransform())>FLEVELDETAIL){
        painter->setPen(QPen(QBrush(clrP_R),0,Qt::SolidLine));
        painter->setBrush(clrB_R);
        painter->drawRect(R);
        painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));


        (R.height()/8.1>0) ? font.setPointSizeF(R.height()/8.1):font.setPointSizeF(1);
        if (FSTATE_RRC) painter->setBrush(Qt::yellow); else
                        painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect_RRC);
        painter->drawText(rect_RRC,flags,"РРС");

        if (FSTATE_ERRCS) painter->setBrush(Qt::red); else
                          painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect_ERRCS);
        painter->drawText(rect_ERRCS,flags,"ЧСТ");

        if (FSTATE_RC) painter->setBrush(Qt::yellow); else
                       painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect_RC);
        painter->drawText(rect_RC,flags,"РЦ");

        if (FSTATE_RC_PRED) painter->setBrush(Qt::yellow); else
                            painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect_RC_PRED);
        painter->drawText(rect_RC_PRED,flags,"ПРЦ");

        painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect_V);
        painter->drawText(rect_V,flags,QString::number((double)FSTATE_V,'f',1));
        painter->drawRect(rect_VOLEG);
        painter->drawText(rect_VOLEG,flags,QString::number((double)FSTATE_VOLEG,'f',1));


        if (FSTATE_T1) painter->setBrush(Qt::yellow); else painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect_T[0]);painter->drawText(rect_T[0],flags,"1");
        if (FSTATE_T2) painter->setBrush(Qt::yellow); else painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect_T[1]);painter->drawText(rect_T[1],flags,"2");
        if (FSTATE_T3) painter->setBrush(Qt::yellow); else painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect_T[2]);painter->drawText(rect_T[2],flags,"3");
        if (FSTATE_T4) painter->setBrush(Qt::yellow); else painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect_T[3]);painter->drawText(rect_T[3],flags,"4");
        if (FSTATE_R) painter->setBrush(Qt::white); else painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect_T[4]);painter->drawText(rect_T[4],flags,"Р");

    } else {
        painter->setPen(QPen(QBrush(clrP_R_B),0,Qt::SolidLine));
        painter->setBrush(clrB_R_B);
        painter->drawRect(R);
        painter->setPen(Qt::NoPen);
        if (FSTATE_RRC) {
            painter->setBrush(Qt::yellow);
            painter->drawRect(rect_RRC_B);
        }
        if (FSTATE_ERRCS) {
            painter->setBrush(Qt::red);
            painter->drawRect(rect_ERRCS_B);
        }
    }
}

void v_ControllerARS::updateStatesFromModel()
{
    v_Model::updateStatesFromModel();
    SET_STATE_MODEL_BOOL(STATE_ADDR);
    SET_STATE_MODEL_BOOL(STATE_T1);
    SET_STATE_MODEL_BOOL(STATE_T2);
    SET_STATE_MODEL_BOOL(STATE_T3);
    SET_STATE_MODEL_BOOL(STATE_T4);
    SET_STATE_MODEL_BOOL(STATE_R);
    SET_STATE_MODEL_BOOL(STATE_RC);
    SET_STATE_MODEL_BOOL(STATE_RRC);
    SET_STATE_MODEL_BOOL(STATE_ERRCS);
    SET_STATE_MODEL_BOOL(STATE_RC_PRED);
    SET_STATE_MODEL_QREAL(STATE_V);
    SET_STATE_MODEL_QREAL(STATE_VOLEG);


}
