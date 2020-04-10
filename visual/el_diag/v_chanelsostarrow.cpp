#include "v_chanelsostarrow.h"

#include <qmath.h>

#include "qpfunction.h"
#include "mcolorpalette.h"

#include "mvp_system.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_ChanelSostArrow,"Стрела состояния канала","VISUAL Диагностика")

void v_ChanelSostArrow::setSIGNAL_1(SignalDescription p)
{
    if ((FSIGNAL_1.chanelName()!=p.chanelName()) || (FSIGNAL_1.chanelType()!=p.chanelType())){
        FSIGNAL_1.setChanelName(p.chanelName());
        FSIGNAL_1.setChanelType(p.chanelType());
        FSIGNAL_1.setIsNoUse(true);
        FSIGNAL_1.getBuffer();
        doPropertyChanged();
    }
}

v_ChanelSostArrow::v_ChanelSostArrow(v_Base *parent) : v_Arrow(parent)
{
    FOPACITY_ANIMATE=false;
    FCOLOR_33=getColor(Color_state_33);
    FBRUSH_COLOR=Qt::green;
    FBRUSH_ANIMATE_COLOR=Qt::yellow;
    FBRUSH_ANIMATE=true;
    FSH=2;
    FANIMATE_SRC=change_data;
}

void v_ChanelSostArrow::setModelObject(BaseObject *)
{

}

void v_ChanelSostArrow::d_paint(QPainter *painter, const QStyleOptionGraphicsItem * )
{
    
    QBrush B;

    if (!STATE_IS_EXISTS()){
        B=QBrush(Qt::black,FBRUSH_STYLE);
    } else {
        //        TChanelInformation I;
        //        if (FIP.isEmpty()) chanel->getChanelInformation(&I); else
        //                          chanel->getChanelInformation(QHostAddress(FIP),&I);
        if (FSIGNAL_1.is33()){
            B=QBrush(FCOLOR_33,FBRUSH_STYLE);
        } else
            if ((FBRUSH_ANIMATE)&&(proclen>0)){
                qreal xc=len_basePolyLine*(proclen)/100.;
                QPointF centr=QPFunction::pointOnPolygon(basePolyLine,xc);
                QRadialGradient gradient(centr, len_basePolyLine/10);
                gradient.setColorAt(0, FBRUSH_COLOR);
                gradient.setColorAt(1, FBRUSH_ANIMATE_COLOR);
                B=QBrush(gradient);
            } else {
                B=QBrush(FBRUSH_COLOR,FBRUSH_STYLE);
            }
    }

    painter->setBrush(B);
    painter->setPen(QPen(QBrush(FPEN_COLOR),0,FPEN_STYLE));
    painter->drawPolygon(arrowPolygon);

}

void v_ChanelSostArrow::advance(int phase)
{
    if (!phase) return;
    if ((!FBRUSH_ANIMATE)&&(!FOPACITY_ANIMATE)) return;
    qreal ms_period=FBRUSH_ANIMATE_SPEED;
    int ms=animateT.elapsed();
    bool animateRestart=false;
    if (STATE_IS_EXISTS()){
        //        TChanelInformation I;
        //        if (FIP.isEmpty()) chanel->getChanelInformation(&I); else
        //                           chanel->getChanelInformation(QHostAddress(FIP),&I);
        //        if (FANIMATE_SRC==change_data){
        //            if (old_TICKDATACHANGED!=I.tickDataChanged[0]){
        //                old_TICKDATACHANGED=I.tickDataChanged[0];
        //                if (ms>ms_period) animateRestart=true;
        //            }
        //        }
        //        if (FANIMATE_SRC==change_tick){
        //            if (old_TICK!=I.tick[0]){
        //                old_TICK=I.tick[0];
        //                animateRestart=true;
        //            }
        //        }
    }
    if (animateRestart){
        animateT.restart();
    }
    if (phase==0){
        proclen=ms/ms_period*100.;
        if (ms>=ms_period) proclen=0;
        if (old_proclen!=proclen) {
            old_proclen=proclen;
            update();
        }
        if ((FOPACITY_ANIMATE)) {
            qreal op=0;
            if (proclen>0){
                op=qSin(M_PI*proclen/100.)*100;
                //if (proclen<50) op=proclen*2; else op=100-(proclen-50)*2;
            }
            setOpacity(op/100.);
        } else {
            setOpacity(1);
        }

    }
}

bool v_ChanelSostArrow::STATE_IS_EXISTS()
{
    if (FSIGNAL_1.getBuffer()) return false;
    return true;

}


