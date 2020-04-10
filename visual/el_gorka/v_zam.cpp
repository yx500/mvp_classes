#include "v_zam.h"



#include "qpfunction.h"
#include "m_zam.h"
#include "v_zam.h"
#include "v_rc.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_Zam,"Замедлитель","VISUAL ГОРКА")

REGISTERCOLOR(v_Zam,zam_tr_st_T14_p,"Замедлитель ступ.1-4 лин",Qt::black);
REGISTERCOLOR(v_Zam,zam_tr_st_T14_b,"Замедлитель ступ.1-4 зпл",Qt::red);
REGISTERCOLOR(v_Zam,zam_pr_st_T14_p,"Замедлитель шина.1-4 лин",Qt::black);
REGISTERCOLOR(v_Zam,zam_pr_st_T14_b,"Замедлитель шина.1-4 зпл",Qt::red);
REGISTERCOLOR(v_Zam,zam_tr_st_R_p,"Замедлитель ступ.Р лин",Qt::black);
REGISTERCOLOR(v_Zam,zam_tr_st_R_b,"Замедлитель ступ.Р зпл",Qt::white);
REGISTERCOLOR(v_Zam,zam_tr_st_Un_p,"Замедлитель ступ.ош. лин",Qt::black);
REGISTERCOLOR2(v_Zam,zam_tr_st_Un_b,"Замедлитель ступ.ош. зпл",Qt::white,Qt::green);
REGISTERCOLOR(v_Zam,zam_pr_st_R_p,"Замедлитель шина.ступ.Р лин",Qt::black);
REGISTERCOLOR(v_Zam,zam_pr_st_R_b,"Замедлитель шина.ступ.Р зпл",Qt::white);
REGISTERCOLOR(v_Zam,zam_pr_st_N_p,"Замедлитель шина.нет ступени лин",Qt::black);
REGISTERCOLOR(v_Zam,zam_pr_st_N_b,"Замедлитель шина.нет ступени зпл",Qt::gray);
REGISTERCOLOR(v_Zam,zam_pr_st_Un_p,"Замедлитель шина.ступ.ош. лин",Qt::black);
REGISTERCOLOR(v_Zam,zam_pr_st_Un_b,"Замедлитель шина.ступ.ош.зпл",Qt::gray);
REGISTERCOLOR(v_Zam,zam_podl_RRC_p,"Замедлитель подл. РРС лин",Qt::green);
REGISTERCOLOR(v_Zam,zam_podl_A_p,"Замедлитель подл.А-РРС лин",Qt::red);
REGISTERCOLOR(v_Zam,zam_podl_ARRC_p,"Замедлитель подл.А+РРС лин",Qt::yellow);
REGISTERCOLOR2(v_Zam,zam_podl_A_b,"Замедлитель подл.А-РРС зпл",Qt::green,Qt::red);
REGISTERCOLOR(v_Zam,zam_podl_ARRC_b,"Замедлитель подл.А+РРС зпл",Qt::green);



v_Zam::v_Zam(v_Base *parent) : v_Model(parent)
{
    M[0]=QPointF(0,0);
    FWIDTH=8;
    FHEIGHT=10;
    FSHRC=4;
    addCOMMON_PROPERTY("SHRC","SH_RC");
    resetStates();
}

void v_Zam::resetStates()
{
    v_Model::resetStates();
    FSTATE_A=false;
    FSTATE_KZ=false;
    FSTATE_OG=false;
    FSTATE_RRC=false;
    FSTATE_STUPEN=m_Zam::st_none;
    FSTATE_MK_RRC=false;
    FSTATE_MK_33=false;
}

void v_Zam::d_paint(QPainter *painter, const QStyleOptionGraphicsItem * )
{
    painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));
    m_Zam::TStupen st=m_Zam::st_none;
    QColor cp_tr=Qt::black;
    QColor cb_tr=Qt::gray;
    QColor cp_pr=Qt::black;
    QColor cb_pr=Qt::gray;
    QPen podl_pen=QPen(Qt::NoPen);
    QBrush podl_brush=QBrush(Qt::NoBrush);
    st=(m_Zam::TStupen) FSTATE_STUPEN;
    TStupPoly *_sp;
    switch(st){
    case m_Zam::st_none: _sp=&sp[0]; cp_pr=getColor(Color_zam_pr_st_N_p);cb_pr=getColor(Color_zam_pr_st_N_b);break;
    case m_Zam::st_T1:   _sp=&sp[1]; cp_pr=getColor(Color_zam_pr_st_T14_p);cb_pr=getColor(Color_zam_pr_st_T14_b); cp_tr=getColor(Color_zam_tr_st_T14_p);cb_tr=getColor(Color_zam_tr_st_T14_b);break;
    case m_Zam::st_T2:   _sp=&sp[2]; cp_pr=getColor(Color_zam_pr_st_T14_p);cb_pr=getColor(Color_zam_pr_st_T14_b); cp_tr=getColor(Color_zam_tr_st_T14_p);cb_tr=getColor(Color_zam_tr_st_T14_b);break;
    case m_Zam::st_T3:   _sp=&sp[3]; cp_pr=getColor(Color_zam_pr_st_T14_p);cb_pr=getColor(Color_zam_pr_st_T14_b); cp_tr=getColor(Color_zam_tr_st_T14_p);cb_tr=getColor(Color_zam_tr_st_T14_b);break;
    case m_Zam::st_T4:   _sp=&sp[4]; cp_pr=getColor(Color_zam_pr_st_T14_p);cb_pr=getColor(Color_zam_pr_st_T14_b); cp_tr=getColor(Color_zam_tr_st_T14_p);cb_tr=getColor(Color_zam_tr_st_T14_b);break;
    case m_Zam::st_R:    _sp=&sp[5]; cp_pr=getColor(Color_zam_pr_st_R_p);cb_pr=getColor(Color_zam_pr_st_R_b); cp_tr=getColor(Color_zam_pr_st_R_p);cb_tr=getColor(Color_zam_pr_st_R_b);break;

    case m_Zam::st_unknow:   _sp=&sp[4]; cp_pr=getColor(Color_zam_pr_st_Un_p);cb_pr=getColor(Color_zam_pr_st_Un_b); cp_tr=getColor(Color_zam_tr_st_Un_p);cb_tr=getColor(Color_zam_tr_st_Un_b);break;
    default:_sp=&sp[0];cp_tr=getColor(Color_zam_pr_st_Un_p);cb_tr=getColor(Color_zam_pr_st_Un_b);break;
    }
    if (FSTATE_33) {
        cb_pr=getColor(Color_state_33);
        cb_tr=getColor(Color_state_33);
    }

    // подлжка
    if (FSTATE_33){

    } else {
        if (FSTATE_MK_33){
            podl_pen=QPen(QBrush(Qt::black),0,Qt::SolidLine);
        } else {
            if (!FSTATE_A){
                if (FSTATE_MK_RRC){
                    podl_pen=QPen(QBrush(getColor(Color_zam_podl_RRC_p)),0,Qt::SolidLine);
                }
            } else {
                if (FSTATE_MK_RRC){
                    podl_pen=QPen(QBrush(getColor(Color_zam_podl_ARRC_p)),0,Qt::SolidLine);
                    podl_brush=QBrush(getColor(Color_zam_podl_ARRC_b));
                } else {
                    podl_pen=QPen(QBrush(getColor(Color_zam_podl_A_p)),0,Qt::SolidLine);
                    podl_brush=QBrush(getColor(Color_zam_podl_A_b));
                }
            }
        }
    }
    painter->setPen(podl_pen);
    painter->setBrush(podl_brush);
    //painter->drawRoundRect(PodlS,25,25);
    qreal radius_uglov=qMin(Podl[0].width()/10.,Podl[0].height()/10.);
        for (int d=0;d<2;d++)
            painter->drawRoundedRect(Podl[d], radius_uglov,radius_uglov);

    // треуголка
    painter->setPen(QPen(QBrush(cp_tr),0,Qt::SolidLine));
    painter->setBrush(cb_tr);
    for (int d=0;d<2;d++){
        foreach (QPolygonF P, _sp->Tr[d]) {
            painter->drawPolygon(P);
        }
    }
    // шина
    painter->setPen(QPen(QBrush(cp_pr),0,Qt::SolidLine));
    painter->setBrush(cb_pr);
    for (int d=0;d<2;d++)
        painter->drawPolygon(_sp->pr[d]);


}

void v_Zam::drawBackground(QPainter *painter)
{
    v_Base::drawBackground(painter);
    if (FSTATE_OG)
                showOG(painter);
}

void v_Zam::updateStatesFromModel()
{
    v_Model::updateStatesFromModel();
    SET_STATE_MODEL_BOOL(STATE_A);
    SET_STATE_MODEL_BOOL(STATE_KZ);
    SET_STATE_MODEL_BOOL(STATE_OG);
    SET_STATE_MODEL_BOOL(STATE_RRC);
    SET_STATE_MODEL_INT(STATE_STUPEN);
    SET_STATE_MODEL_BOOL(STATE_MK_RRC);
    SET_STATE_MODEL_BOOL(STATE_MK_33);
}



void v_Zam::calculateGeometry()
{
    S=FSHRC;//viewProperty_int(v_RC::groupProp_SHRC,FSHRC);

    qreal w=FWIDTH*gridDX();
    qreal h=FHEIGHT*gridDY();
    //qreal ddx=3;
    qreal wpr=w-3; // шина
    qreal wpodl=w-1; // подлжка

    h=(h-S*2)/2.; // высота одной части
    qreal h_tr=h/4.*2.; // высота/ширина тр
    qreal s_tr=h_tr;
    qreal h_pr=h/4.; // высота/ширина prтр
    qreal xl=M[0].x()-w/2.;
    qreal xlpr=xl+(w-wpr)/2.;
    qreal xltr=xlpr;
    qreal wtr=wpr;
    Podl[0]=QRectF(0,0,wpodl,h);Podl[0].moveCenter(QPointF(M[0].x(),M[0].y()-S-h/2-1));
    Podl[1]=QRectF(0,0,wpodl,h);Podl[1].moveCenter(QPointF(M[0].x(),M[0].y()+S+h/2+1));
    //Podl[0]=QRectF(xl,M[0].y()-S-h,w,h);
    //Podl[1]=QRectF(xl,M[0].y()+S,w,h);
    PodlS=QRectF(0,0,w,FHEIGHT*gridDY());
    PodlS.moveCenter(M[0]);



    // нет
    {
        int s=0;
        sp[s].Tr[0].clear();sp[s].Tr[1].clear();
        sp[s].pr[0].clear();sp[s].pr[1].clear();
        qreal y0=M[0].y()-S-h/2.+h_pr/2.;
        sp[s].pr[0] <<QPointF(xlpr,y0) << QPointF(xlpr,y0-h_pr) << QPointF(xlpr+wpr,y0-h_pr)  << QPointF(xlpr+wpr,y0);
        sp[s].pr[1] = sp[s].pr[0];
        QPFunction::reflectPLy(&sp[s].pr[1],0);
    }
    //1-4
    for (int s=1;s<=4;s++){
        qreal y0=M[0].y()-S;
        sp[s].Tr[0].clear();sp[s].Tr[1].clear();
        sp[s].pr[0].clear();sp[s].pr[1].clear();
        qreal trcnt=s;
        qreal dx_tr=wtr/(trcnt+1);
        for (int t=0;t<trcnt;t++){
            QPointF Tr=QPointF (xltr+dx_tr*(t+1),y0-h_pr);
            QPolygonF P;
            P.clear();
            P << Tr << QPointF(Tr.x()-s_tr/2.,Tr.y()-h_tr) << QPointF(Tr.x()+s_tr/2.,Tr.y()-h_tr);
            sp[s].Tr[0].push_back(P);
            QPFunction::reflectPLy(&P,0);
            sp[s].Tr[1].push_back(P);

        }
        sp[s].pr[0] <<QPointF(xlpr,y0) << QPointF(xlpr,y0-h_pr) << QPointF(xlpr+wpr,y0-h_pr)  << QPointF(xlpr+wpr,y0);
        sp[s].pr[1] = sp[s].pr[0];
        QPFunction::reflectPLy(&sp[s].pr[1],0);
    }
    // Р
    {
        int s=5;
        sp[s].Tr[0].clear();sp[s].Tr[1].clear();
        sp[s].pr[0].clear();sp[s].pr[1].clear();
        //qreal trcnt=2;
        //qreal dx_tr=wtr/(trcnt+1);
        //qreal y0=M[0].y()-S;
        qreal y0=M[0].y()-S-h_pr/2.;
        //        for (int t=0;t<trcnt;t++){
        //            QPointF Tr=QPointF (xltr+dx_tr*(t+1),y0-h_pr-h_tr);
        //            QPolygonF P;
        //            P.clear();
        //            P << Tr << QPointF(Tr.x()-s_tr/2.,Tr.y()+h_tr) << QPointF(Tr.x()+s_tr/2.,Tr.y()+h_tr);
        //            sp[s].Tr[0].push_back(P);
        //            QPFunction::reflectPLy(&P,0);
        //            sp[s].Tr[1].push_back(P);

        //        }
        //        sp[s].pr[0] <<QPointF(xlpr,y0-h_pr-h_tr) << QPointF(xlpr,y0-h_pr*2.-h_tr) << QPointF(xlpr+wpr,y0-h_pr*2.-h_tr)  << QPointF(xlpr+wpr,y0-h_pr-h_tr);
        //        sp[s].pr[1] = sp[s].pr[0];
        sp[s].pr[0] <<QPointF(xlpr,y0) << QPointF(xlpr,y0-h_pr) << QPointF(xlpr+wpr,y0-h_pr)  << QPointF(xlpr+wpr,y0);
        sp[s].pr[1] = sp[s].pr[0];
        QPFunction::reflectPLy(&sp[s].pr[1],0);
        ZablokR=PodlS;
        ZablokR.adjust(-gridDX()/2,-gridDY()/2, gridDX()/2,gridDY()/2);
    }

    {
        _boundingRect=QRectF(-FWIDTH*gridDX()/2.,-FHEIGHT*gridDY()/2.,FWIDTH*gridDX(),FHEIGHT*gridDY());
        _boundingRect=_boundingRect.united(ZablokR);
        _boundingRect.moveCenter(M[0]);
    }


}

void v_Zam::showOG(QPainter *painter)
{
    QColor clr=getColor(v_RC::Color_rc_blok);
    painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));
    painter->setBrush(clr);
    painter->drawRect(ZablokR);
}


