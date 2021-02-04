#include "m_controllerars.h"


#include "baseobjecttools.h"
#include <QDebug>
#include "mvp_system.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(m_ControllerARS,"Контроллер АРС","MODEL ГОРКА")


qreal t_ARSDescrCellsBiostime::V() const
{
    return m_ControllerARS::TPVal(cell.V);
}


m_ControllerARS::m_ControllerARS(QObject *parent) : m_Base(parent)
{
    FADDR=0;
    FADDR_SLOT=0;
    FSIGNAL_ADDR.clear();
    resetStates();
}

void m_ControllerARS::resetStates()
{
    m_Base::resetStates();
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
    FSTATE_UFO=0;
//    lARSDescrCellsPackets.clear();
}

void m_ControllerARS::validation(ListObjStr *l) const
{
    m_Base::validation(l);
    if (FADDR_SLOT==0) l->error(this,"Не задан номер слота");
    if (FADDR==0) l->error(this,"Не задан адрес");
    if (objectCountLinked2Object(this)==0){
        l->warning(this,"Не используется");
    }
}

void m_ControllerARS::updateAfterLoad()
{
    m_Base::updateAfterLoad();
    QString packetName=QString("ARS%1").arg(FADDR_SLOT);
    FSIGNAL_ADDR=SignalDescription(4,packetName,FADDR-1);
    FSIGNAL_ADDR.acceptGtBuffer();
    if (FSIGNAL_ADDR.getBuffer()!=nullptr) FSIGNAL_ADDR.getBuffer()->setMsecPeriodLive(1000);

}

qreal m_ControllerARS::TPVal(quint16 w)
{
    return (!w || w == 0xffff) ?  0. : (1000000. / (70.*w));
}

bool m_ControllerARS::isValidState() const
{
    if (FSTATE_33) return false;
    if (FSTATE_ERRCS) return false;
    if (!FSTATE_RRC) return false;
    return true;

}

bool m_ControllerARS::is33()
{
    if (m_Base::is33()) return true;
    if (FSIGNAL_ADDR.is33()) return true;
    const t_ARSDescrCellsPacket *A=reinterpret_cast <const t_ARSDescrCellsPacket *>(FSIGNAL_ADDR.value_data(0,sizeof(t_ARSDescrCellsPacket)));
    if ((!A)||(FADDR<=0)||(FADDR>32)){
        return true;
    }
    return false;
}

void m_ControllerARS::updateStates()
{
    m_Base::updateStates();
    if (FSTATE_33) return ;
    const t_ARSDescrCellsPacket *A=reinterpret_cast <const t_ARSDescrCellsPacket *>(FSIGNAL_ADDR.value_data(0,sizeof(t_ARSDescrCellsPacket)));
    if ((!A)||(FADDR<=0)||(FADDR>32)){
    } else {
        if (FADDR==A->Cells[FADDR-1].addr){
            setSTATE_ADDR(true);
            setSTATE_ERRCS(A->Cells[FADDR-1].RC.ErrCS);
            setSTATE_RC(A->Cells[FADDR-1].RC.RC);
            setSTATE_RC_PRED(A->Cells[FADDR-1].RC.RC_pred);
            setSTATE_RRC(A->Cells[FADDR-1].RC.RRC);
            setSTATE_T1(A->Cells[FADDR-1].Step.T1);
            setSTATE_T2(A->Cells[FADDR-1].Step.T2);
            setSTATE_T3(A->Cells[FADDR-1].Step.T3);
            setSTATE_T4(A->Cells[FADDR-1].Step.T4);
            setSTATE_R( A->Cells[FADDR-1].Step.R);
            setSTATE_UFO(A->Cells[FADDR-1].RC.UFO);
            setSTATE_V(TPVal(A->Cells[FADDR-1].V));
            setSTATE_VOLEG(TPVal(A->Cells[FADDR-1].Voleg));
//            if ((lARSDescrCellsPackets.isEmpty())||
//                    (lARSDescrCellsPackets.first().biostime!=A->biostime) ||
//                    (memcmp(&lARSDescrCellsPackets.first().cell,&A->Cells[FADDR-1],sizeof(t_ARSDescrCell)))){
//                t_ARSDescrCellsBiostime AB;
//                AB.t=FSIGNAL_ADDR.getBuffer()->timeDataRecived;
//                AB.biostime=A->biostime;
//                memcpy(&AB.cell,&A->Cells[FADDR-1],sizeof(t_ARSDescrCell));
//                lARSDescrCellsPackets.push_front(AB);
//                if (lARSDescrCellsPackets.size()>1024) lARSDescrCellsPackets.pop_back();
//            }

        } else {
            if (FSTATE_ADDR) resetStates();
            setSTATE_ADDR(false);
        }
    }

}



void m_ControllerARS::setADDR_SLOT(int p)
{
    if (FADDR_SLOT!=p){
        FADDR_SLOT=p;

    }
}

