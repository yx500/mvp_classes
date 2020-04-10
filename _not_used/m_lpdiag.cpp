#include "m_lpdiag.h"


#include "signaldescription.h"
#include "qnxdlp.h"

#include "mvp_objectfactory.h"
REGISTERMODEL(m_LPDiag,"модель ЛП")

extern SignalsManager *SManager;
m_LPDiag::m_LPDiag(QObject *parent):
    m_Base(parent)
{
    FCHANELNAME="";
    FADDR_KRUG=0;
    FNORDER=0;
    FPACKET_RECIVED13=0;
    FPACKET_RECIVED14=0;
    FPACKET_RECIVED15=0;
    FADDR00=FADDR01=FADDR10=FADDR11=0;
    FSTATE_ADDR_IN00=FSTATE_ADDR_IN01=FSTATE_ADDR_IN10=FSTATE_ADDR_IN11=0;

    tick_150=0;tick_158=0;
    tick_140=0;tick_148=0;
    tick_130=0;tick_138=0;
}

ListObjStr m_LPDiag::validation()
{
    ListObjStr l;
    return l;

}

m_LPDiag *m_LPDiag::find_lp_by_addrlink(int addr)
{
    QList<m_LPDiag*>   l=parent()->findChildren<m_LPDiag*>();
    foreach (m_LPDiag* mlp, l) {
        if (mlp->ADDR_KRUG()!=ADDR_KRUG()) continue;
        if (mlp->ADDR00()==addr) return mlp;
        if (mlp->ADDR01()==addr) return mlp;
        if (mlp->ADDR10()==addr) return mlp;
        if (mlp->ADDR11()==addr) return mlp;
    }
    return 0;
}

void m_LPDiag::updateStates()
{
   QString FCHANELNAME8=FCHANELNAME+"8";

    TChanelInformation I;
    SManager->getChanelInformation(15,FCHANELNAME,&I);
    if (tick_150!=I.tick[I.signalSource]){
        tick_150=I.tick[I.signalSource];
        setPACKET_RECIVED15(tick_150+tick_158);
    }
    SManager->getChanelInformation(15,FCHANELNAME8,&I);
    if (tick_158!=I.tick[I.signalSource]){
        tick_158=I.tick[I.signalSource];
        setPACKET_RECIVED15(tick_150+tick_158);
    }
    SManager->getChanelInformation(14,FCHANELNAME,&I);
    if (tick_140!=I.tick[I.signalSource]){
        tick_140=I.tick[I.signalSource];
        setPACKET_RECIVED14(tick_140+tick_148);
    }
    SManager->getChanelInformation(14,FCHANELNAME8,&I);
    if (tick_148!=I.tick[I.signalSource]){
        tick_148=I.tick[I.signalSource];
        setPACKET_RECIVED14(tick_140+tick_148);
    }
    SManager->getChanelInformation(13,FCHANELNAME,&I);
    if (tick_130!=I.tick[I.signalSource]){
        tick_130=I.tick[I.signalSource];
        setPACKET_RECIVED13(tick_130+tick_138);
    }
    SManager->getChanelInformation(13,FCHANELNAME8,&I);
    if (tick_138!=I.tick[I.signalSource]){
        tick_138=I.tick[I.signalSource];
        setPACKET_RECIVED13(tick_130+tick_138);
    }


    tKPMask *KPMask0=0;
    tKPMask *KPMask8=0;
    tQMsg * QMsg0=(tQMsg *)SManager->getChanelData(15,FCHANELNAME);
    //if (result==ISignalsManager::soChanelDie) setSTATE_33(true); else setSTATE_33(false);
    if ( QMsg0!=0) KPMask0=(tKPMask *) &QMsg0->LPY;

    tQMsg * QMsg8=(tQMsg *)SManager->getChanelData(15,FCHANELNAME8);
    if ( QMsg8!=0) KPMask8=(tKPMask *) &QMsg8->LPY;

    if (KPMask0!=0){
        setSTATE_ADDR_IN00(KPMask0->AddrUz[0]);
        setSTATE_ADDR_IN01(KPMask0->AddrUz[1]);
    }


    if (KPMask8!=0){
        setSTATE_ADDR_IN10(KPMask8->AddrUz[0]);
        setSTATE_ADDR_IN11(KPMask8->AddrUz[1]);
    }



}



void m_LPDiag::setCHANELNAME(QString p)
{
    if (FCHANELNAME!=p){
        FCHANELNAME=p;
        if (FCHANELNAME!=""){
            QString FCHANELNAME8=FCHANELNAME+"8";
            // инициализируем каналы
            SManager->getSignal1byte(13,FCHANELNAME,0);
            SManager->getSignal1byte(13,FCHANELNAME8,0);
            SManager->getSignal1byte(14,FCHANELNAME,0);
            SManager->getSignal1byte(14,FCHANELNAME8,0);
            SManager->getSignal1byte(15,FCHANELNAME,0);
            SManager->getSignal1byte(15,FCHANELNAME8,0);
            emit propertyChanged(this);
        }
    }
}

void m_LPDiag::setNORDER(int p)
{
    if (FNORDER!=p){
        FNORDER=p;
        int N=((0x00f0 & FNORDER*16)>> 3)+ 3;
        FADDR00=(0x40 | N);
        FADDR01=(0x80 | N);
        FADDR10=(0x40 | N)+1;
        FADDR11=(0x80 | N)+1;
//        FADDR00=67+FNORDER*2;
//        FADDR01=68+FNORDER*2;
//        FADDR10=131+FNORDER*2;
//        FADDR11=132+FNORDER*2;
        if (p==0){
            FADDR00=0x41;
            FADDR01=0x42;
            FADDR10=0x81;
            FADDR11=0x82;

        }
        emit propertyChanged(this);
    }
}

int m_LPDiag::ADDR()
{
    return FADDR_KRUG*0x100+FNORDER*0x10;
}
