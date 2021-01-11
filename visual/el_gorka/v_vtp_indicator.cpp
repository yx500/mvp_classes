#include "v_vtp_indicator.h"

#include <QPalette>
#include "baseobjecttools.h"
#include "qdrawutil.h"
#include "m_otceps.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_VTP_Indicator,"Индикатор скорости на ТП","VISUAL ГОРКА")

REGISTERCOLOR(v_VTP_Indicator,tpind_in,"ТП индикатор вход",Qt::yellow);
REGISTERCOLOR(v_VTP_Indicator,tpind_out,"ТП индикатор выход",Qt::gray);
REGISTERCOLOR(v_VTP_Indicator,tpind_podl,"ТП индикатор подложка",Qt::gray);

v_VTP_Indicator::v_VTP_Indicator(v_Base *parent) : v_Base(parent)
{
    FZAM1=0;
    FZAM2=0;
    FHEIGHT=4;
    FWIDTH_FACT=8;
    FWIDTH_ZAD=8;
    FWIDTH_VES=4;
    FIS_SMALL=false;
    FFONT=QFont("Times", 12);
    inoutDescr=0;
    FSMALL_SIZE=QSizeF(2,1);
    zam1=0;zam2=0;
    mOtceps=nullptr;
    FSTATE_VF=0;
    FSTATE_VZ=0;
    FSTATE_VO=0;
    FSTATE_ST=-1;
    FSTATE_INOUT=0;
}

void v_VTP_Indicator::setZAM1(ObjectLink p)
{
    if (FZAM1!=p){
        FZAM1=p;
        zam1=qobject_cast<m_Zam*>(updateLink(FZAM1));
        update();
    }
}
void v_VTP_Indicator::setZAM2(ObjectLink p)
{
    if (FZAM2!=p){
        FZAM2=p;
        zam2=qobject_cast<m_Zam*>(updateLink(FZAM2));
        update();
    }
}

void v_VTP_Indicator::setOTCEPS(ObjectLink p)
{
    if (FOTCEPS!=p){
        FOTCEPS=p;
        mOtceps=qobject_cast<m_Otceps*>(updateLink(FOTCEPS));
        update();
    }
}



void v_VTP_Indicator::validation(ListObjStr *l) const
{
    v_Base::validation(l);
}

void v_VTP_Indicator::updateAfterLoad()
{
    mOtceps=nullptr;
    v_Base::updateAfterLoad();
    if (!FZAM1.id()==0){
        zam1=qobject_cast<m_Zam*>(updateLink(FZAM1));
        if (!zam1)
            qCritical() << objectName() << "Ошибочная ссылка ZAM1" <<endl ;
    }
    if (!FZAM2.id()==0){
        zam2=qobject_cast<m_Zam*>(updateLink(FZAM2));
        if (!zam2)
            qCritical() << objectName() << "Ошибочная ссылка ZAM2" <<endl ;
    }
    if (!FOTCEPS.id()==0){
        mOtceps=qobject_cast<m_Otceps*>(updateLink(FOTCEPS));
        if (!mOtceps)
            qCritical() << objectName() << "Ошибочная ссылка OTCEPS" <<endl ;
    }

}

void v_VTP_Indicator::calculateGeometry()
{
    qreal griddx=gridDX();
    qreal griddy=gridDY();
    R_SMALL=QRectF(0,0,FSMALL_SIZE.width()*gridDX(),FSMALL_SIZE.height()*gridDY());
    R=QRectF(0,0,(FWIDTH_FACT+FWIDTH_VES+FWIDTH_ZAD)*griddx,FHEIGHT*griddy);
    RF=QRectF(0,0,FWIDTH_FACT*griddx,FHEIGHT*griddy);
    RZ=QRectF(0,0,FWIDTH_ZAD*griddx ,FHEIGHT*griddy);
    RV=QRectF(0,0,FWIDTH_VES*griddx ,FHEIGHT*griddy);
    RF.adjust(2,2,-2,-2);
    RV.adjust(0,2, 0,-2);
    RZ.adjust(2,2,-2,-2);
    R.moveCenter(QPointF(0,0));
    R_SMALL.moveCenter(QPointF(0,0));
    if (FINVERTDRAWX){
        RF.moveCenter(QPointF(R.right()-FWIDTH_FACT/2*griddx,R.center().y()));
        RV.moveCenter(QPointF(R.right()-(FWIDTH_FACT+FWIDTH_VES/2)*griddx,R.center().y()));
        RZ.moveCenter(QPointF(R.right()-(FWIDTH_FACT+FWIDTH_VES+FWIDTH_ZAD/2)*griddx,R.center().y()));
    } else {
        RF.moveCenter(QPointF(R.left()+FWIDTH_FACT/2*griddx,R.center().y()));
        RV.moveCenter(QPointF(R.left()+(FWIDTH_FACT+FWIDTH_VES/2)*griddx,R.center().y()));
        RZ.moveCenter(QPointF(R.left()+(FWIDTH_FACT+FWIDTH_VES+FWIDTH_ZAD/2)*griddx,R.center().y()));
    }

    if (FIS_SMALL) _boundingRect=R_SMALL; else
        _boundingRect=R;

}

m_Otcep *v_VTP_Indicator::optcep_on_zam(m_Zam *zam)
{
    m_Otcep*otc=0;
    if (!zam) return nullptr;
    if (!mOtceps)return nullptr;
    m_RC*rc=zam->rc();
    if (otc==0){
        // ищем cпереди и на ТП
        if (zam->controllerARS())
            otc=mOtceps->otcepADDR_SLOT(zam->controllerARS()->ADDR_SLOT(),zam->controllerARS()->ADDR(),zam->NTP());
        if (!otc){
            for (int i=0;i<5;i++){
                if (rc==0) break;
                otc=mOtceps->otcepOnRc(rc) ;
                if (otc!=0) break;
                rc=rc->getNextRCpolcfb(1);
            }
        }
        if (otc!=0) inoutDescr=inDescr;
    }
    if (otc==0){
        // ищем позади
        if (zam->controllerARS())
            otc=mOtceps->otcepADDR_SLOT(zam->controllerARS()->ADDR_SLOT(),zam->controllerARS()->ADDR(),zam->NTP());
        if (!otc){
            rc=zam->rc()->getNextRCpolcfb(0);
            for (int i=0;i<5;i++){
                if (rc==0) break;
                otc=mOtceps->otcepOnRc(rc) ;
                if (otc!=0) break;
                rc=rc->getNextRCpolcfb(0);
            }
        }
        if (otc!=0) inoutDescr=outDescr;
    }
    return otc;
}



void v_VTP_Indicator::updateStates()
{
    inoutDescr=_noDescr;
    qreal vF=-1;
    qreal vZ=-1;
    qreal vO=-1;
    int stupen=-1;
    m_Otcep*otc=0;
    if (zam2){
        otc=optcep_on_zam(zam2);
    }
    if ((!otc) && (zam1)) otc=optcep_on_zam(zam1);
    // выбираем нужный зам
    m_Zam *zam=zam1;
    if (zam2){
        if ((zam2->controllerARS()!=0)&&(zam2->controllerARS()->STATE_RC()==1)){
            zam=zam2;
        }
    }
    // ищем фактическую
    if ((zam!=0)&&(zam->controllerARS()!=0)&&(zam->controllerARS()->STATE_RC()==1)){
        vF=zam->controllerARS()->STATE_V();
    }

    if (((inoutDescr==_noDescr)&&(vF<=0))&&(!ISEDITORMODE)){
        setVisible(false);
    } else {
        setVisible(true);
    }
    if (otc!=0){
        stupen=otc->stored_Descr.st;
        vZ=otc->stored_Descr.V_zad/10.;vO=otc->stored_Descr.V_out/10.;
        if (zam!=nullptr){
            if (zam->NTP()==2) {vZ=otc->stored_Descr.V_zad2/10.;vO=otc->stored_Descr.V_out2/10.;}
            if (zam->NTP()==3) {vZ=otc->stored_Descr.V_zad3/10.;vO=otc->stored_Descr.V_out3/10.;}
        }
    }
    setSTATE_VF(vF);
    setSTATE_VZ(vZ);
    setSTATE_VO(vO);
    setSTATE_ST(stupen);
    setSTATE_INOUT(inoutDescr);



}

void v_VTP_Indicator::setSTATE_VF(const qreal &p)
{
    if (FSTATE_VF!=p){
        FSTATE_VF=p;
        if (FSTATE_VF>1.4)
            stVF=stVF.sprintf("%2.1f",FSTATE_VF); else
            stVF="";
        doStateChanged();
    }
}
void v_VTP_Indicator::setSTATE_VZ(const qreal &p)
{
    if (FSTATE_VZ!=p){
        FSTATE_VZ=p;
        if (FSTATE_VZ!=-1)
            stVZ=stVZ.sprintf("%2.1f",FSTATE_VZ); else
            stVZ="";
        doStateChanged();
    }
}

void v_VTP_Indicator::setSTATE_VO(const qreal &p)
{
    if (FSTATE_VO!=p){
        FSTATE_VO=p;
        if (FSTATE_VO>0)
            stVO=stVO.sprintf("%2.1f",FSTATE_VO); else
            stVO="";
        doStateChanged();
    }
}

void v_VTP_Indicator::setSTATE_ST(const int &p)
{
    if (FSTATE_ST!=p){
        FSTATE_ST=p;
        stVV="";
        if (FSTATE_ST==1) stVV="Л";
        if (FSTATE_ST==2) stVV="лс";
        if (FSTATE_ST==3) stVV="С";
        if (FSTATE_ST==4) stVV="Т";

        doStateChanged();
    }
}

void v_VTP_Indicator::d_paint(QPainter *painter, const QStyleOptionGraphicsItem *)
{

    QPen P;
    QBrush B;
    QColor clr;
    clr=getColor(Color_tpind_podl);
    B=QBrush(clr);
    P=QPen(clr);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(P);
    if (FIS_SMALL) painter->drawRect(R_SMALL); else
        painter->drawRect(R);
    if (inoutDescr==inDescr)
        clr=getColor(Color_tpind_in);else
        clr=getColor(Color_tpind_out);


    QPalette pal=QPalette(clr);
    P=QPen(Qt::black);
    B=QBrush(clr);
    painter->setBrush(B);
    painter->setPen(P);

    QString stV=stVF;
    if ((inoutDescr==outDescr) &&(FSTATE_VF<=0)) stV=stVO;


    if (FIS_SMALL){
        if (FWIDTH_FACT>0)
        {
            // фактическая
            (R_SMALL.height()*0.8>0) ? FFONT.setPointSizeF(R_SMALL.height()*0.8):FFONT.setPointSizeF(1);
            painter->setFont(FFONT);
            qDrawShadeRect(painter,R_SMALL.toRect(),pal,false,1,0);
            painter->drawText(R_SMALL,Qt::AlignCenter,stV);
        } else
            if (FWIDTH_ZAD>0)
            {
                // заданная
                (R_SMALL.height()*0.5>0) ? FFONT.setPointSizeF(R_SMALL.height()*0.5):FFONT.setPointSizeF(1);
                painter->setFont(FFONT);
                qDrawShadeRect(painter,R_SMALL.toRect(),pal,false,1,0);
                painter->drawText(R_SMALL,Qt::AlignCenter,stVZ);
            }
    } else {
        if (FWIDTH_FACT>0)
        {
            // фактическая
            (R.height()*0.8>0) ? FFONT.setPointSizeF(R.height()*0.8):FFONT.setPointSizeF(1);
            painter->setFont(FFONT);
            qDrawShadeRect(painter,RF.toRect(),pal,false,1,0);
            painter->drawText(RF,Qt::AlignCenter,stV);
        }
        if (FWIDTH_ZAD>0)
        {
            // заданная
            (R.height()*0.5>0) ? FFONT.setPointSizeF(R.height()*0.5):FFONT.setPointSizeF(1);
            painter->setFont(FFONT);
            qDrawShadeRect(painter,RZ.toRect(),pal,false,1,0);
            painter->drawText(RZ,Qt::AlignCenter,stVZ);
        }
        if (FWIDTH_VES>0)
        {
            // вес
            qDrawShadeRect(painter,RV.toRect(),pal,false,1,0);
            //painter->drawRect(RV);
            (R.height()*0.5>0) ? FFONT.setPointSizeF(R.height()*0.5):FFONT.setPointSizeF(1);
            painter->setFont(FFONT);
            painter->drawText(RV,Qt::AlignCenter,stVV);

        }
    }
}

