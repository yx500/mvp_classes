#include "v_rtds.h"
#include "qpfunction.h"
#include "v_rc.h"


#include "mvp_objectfactory.h"
REGISTERELEMENT(v_RTDS,"РТДС","VISUAL ГОРКА")

REGISTERPROPERTY(v_RTDS,M1,"CLASSIC_VIEW","КЛАССИЧ.ВИД","")

v_RTDS::v_RTDS(v_Base *parent) : v_Model(parent)
{
    FSHRC=4;
    FCLASSIC_VIEW=false;
    calculateGeometry();

    FSIZE=QSize(1,2);
    FLENGTH=4;
    FTEXT_XY=QPointF(0,-FLENGTH);
    addCOMMON_PROPERTY("SHRC","SH_RC");

    resetStates();
}

void v_RTDS::resetStates()
{
    v_Model::resetStates();
    FSTATE_SRAB=false;
}

void v_RTDS::setSHRC(int p)
{
    if (p>=0){
        if (FSHRC!=p){
            FSHRC=p;
            doPropertyChanged();
        }
    }
}

void v_RTDS::calculateGeometry()
{
    S=gridDY();
    M[0]=QPointF(0,0);
    RC=QRectF(0,0,S,S);
    RC.moveCenter(M[0]);

    qreal w=FSIZE.width()*gridDX();
    qreal h=FSIZE.height()*gridDY();
    qreal l=FLENGTH*gridDY()/2.;

    T[0]=QPointF(0,-l);
    T[1]=QPointF(0,+l);

    RR[0]=QRectF(0,0,w,h);
    RR[1]=RR[0];
    RR[0].moveCenter(QPointF(0,-l));
    RR[1].moveCenter(QPointF(0,+l));
    TR[0].clear();TR[1].clear();
    qreal wt=w/2;
    qreal ht=h*0.8;

    TR[0] << QPointF(-wt/2.,-ht/2.) << QPointF(+wt/2.,-ht/2.) << QPointF(0,+ht/2.);
    TR[1]=TR[0];
    TR[0].translate(w/2.,-l);
    TR[1].translate(w/2.,+l);
    rect_text=QRect(0,0,w*4,h/2);
    rect_text.moveCenter(QPointF(0,-l-h/2.-rect_text.height()/2.));

    {
        if (!FCLASSIC_VIEW) {
            _boundingRect=RC.adjusted(-4,-4,4,4);
        } else{
            QPolygonF P=QPolygonF(RR[0]);
            P=P.united(RR[1]);
            P=P.united(TR[0]);
            P=P.united(TR[1]);
            if (!bound_rect_text.isEmpty()) P=P.united(bound_rect_text);
            _boundingRect=P.boundingRect();
        }
    }


}

void v_RTDS::d_paint(QPainter *painter, const QStyleOptionGraphicsItem * option)
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
            if (FSTATE_SRAB) {
                clrB=getColor(v_RC::Color_rc_busy);
            }else {
            }
        }
    }
    painter->setPen(QPen(QBrush(clrP),1,Qt::SolidLine));
    painter->setBrush(clrB);
    if (!FCLASSIC_VIEW){
        painter->drawEllipse(RC);
    } else {
        //        painter->setPen(QPen(B.color(),1,Qt::SolidLine));
        //        painter->drawLine(T[0],T[1]);
        //        painter->drawEllipse(T[0],4,4);
        //        painter->drawEllipse(T[1],4,4);

        painter->drawRect(RR[0]);
        painter->drawRect(RR[1]);
        if (option->levelOfDetailFromTransform(painter->worldTransform())>FLEVELDETAIL){
            painter->drawPolygon(TR[0]);
            painter->drawPolygon(TR[1]);
        }
        if (!FSTATE_SRAB){
            painter->setPen(QPen(QBrush(Qt::blue),0,Qt::DashLine));
        } else {
            painter->setPen(QPen(QBrush(getColor(v_RC::Color_rc_busy)),0,Qt::DashLine));
        }
        painter->drawLine(T[0],T[1]);
        painter->setPen(QPen(QBrush(Qt::black),1,Qt::SolidLine));

        if (option->levelOfDetailFromTransform(painter->worldTransform())>FLEVELDETAIL){
            if (rect_text.height()>0){
                QFont font("Times",1);
                int flags=Qt::AlignCenter;
                font.setPointSizeF(rect_text.height()*0.7);
                painter->setFont(font);
                painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));
                painter->drawText(rect_text,flags,objectName(),&bound_rect_text);
            }
        }

    }
}

void v_RTDS::updateStatesFromModel()
{
    v_Model::updateStatesFromModel();
    SET_STATE_MODEL_BOOL(STATE_SRAB);
    if (FSTATE_SRAB) setZValue(-1); else setZValue(1);
}




