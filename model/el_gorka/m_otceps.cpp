#include "m_otceps.h"
#include "mvp_system.h"
#include "m_controllerars.h"
#include "m_rc_gor_park.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(m_Otceps,"ОТЦЕПЫ","MODEL ГОРКА")

m_Otceps::m_Otceps(QObject *parent) : m_Base(parent)
{
    setObjectName("ОТЦЕПЫ");
    for (int i=0;i<MAXCOUNT_OTCEPS;i++){
        OTCEPS[i]=new m_Otcep(this,i+1);
        l_otceps.push_back(OTCEPS[i]);
    }
}

m_Otceps::~m_Otceps()
{
    l_otceps.clear();
    for (int i=0;i<MAXCOUNT_OTCEPS;i++){
        delete OTCEPS[i];
    }
}

void m_Otceps::resetStates()
{
    m_Base::resetStates();
}

void m_Otceps::validation(ListObjStr *l) const
{
    m_Base::validation(l);

    foreach (m_RC *rc1, l_rc) {
        foreach (m_RC *rc2, l_rc) {
            if (rc1==rc2) continue;
            if ((!rc1->SIGNAL_BUSY().isNotUse()) &&
                    (!rc2->SIGNAL_BUSY().isNotUse()) &&
                    (rc1->SIGNAL_BUSY().chanelOffset()==rc2->SIGNAL_BUSY().chanelOffset())){
                l->error(this,"РЦ с одинаковым сигналом занятия",QString("%1 %2").arg(rc1->objectName(),rc2->objectName()));
            }
        }
        m_RC_Gor_Park*rcp=qobject_cast<m_RC_Gor_Park*>(rc1);
        if ((rcp) && (rcp->PARK_WAY()<=0)) l->error(this,"РЦ без номера пути парка",QString("%1").arg(rc1->objectName()));
        if ((rcp) && (rcp->MINWAY()<=0)) l->error(this,"РЦ без номера маршрута",QString("%1").arg(rc1->objectName()));
        if ((rcp) && (rcp->MAXWAY()<=0)) l->error(this,"РЦ без номера маршрута",QString("%1").arg(rc1->objectName()));
        if ((rcp) && (rcp->MINWAY()!=rcp->MAXWAY())) l->error(this,"РЦ c разным номера маршрута",QString("%1").arg(rc1->objectName()));
    }
}

void m_Otceps::updateAfterLoad()
{
    m_Base::updateAfterLoad();
    for (int i=0;i<MAXCOUNT_OTCEPS;i++)
        OTCEPS[i]->updateAfterLoad();

    registerChanels();
    if (parent()){
        l_rc=parent()->findChildren<m_RC*>();
        foreach (m_RC *rc, l_rc) {
            if ((!rc->SIGNAL_BUSY().isNotUse()) &&(rc->SIGNAL_BUSY().chanelOffset()!=0))
                mOffset2Rc[rc->SIGNAL_BUSY().chanelOffset()]=rc;
        }
        QList<m_RC_Gor_Park*> l_rcp=parent()->findChildren<m_RC_Gor_Park*>();
        foreach (m_RC_Gor_Park*rcp, l_rcp) {
            mSP2MAR[rcp->PARK_WAY()]=rcp->MINWAY();
            mMAR2SP[rcp->MINWAY()]=rcp->PARK_WAY();

        }
    }
}

QList<m_Otcep *> m_Otceps::otceps() const
{
    QList<m_Otcep *> vOtceps;
    for (int i=0;i<MAXCOUNT_OTCEPS;i++)
        if (OTCEPS[i]->STATE_LOCATION()!=m_Otcep::locationUnknow) vOtceps.push_back(OTCEPS[i]);
    return vOtceps;
}

QList<m_Otcep *> m_Otceps::all_otceps() const
{
    QList<m_Otcep *> vOtceps;
    for (int i=0;i<MAXCOUNT_OTCEPS;i++)
        vOtceps.push_back(OTCEPS[i]);
    return vOtceps;
}

m_Otcep *m_Otceps::otcepByNum(int n)
{
    for (int i=0;i<MAXCOUNT_OTCEPS;i++)
        if (OTCEPS[i]->NUM()==n) return  OTCEPS[i];
    return nullptr;
}

m_Otcep *m_Otceps::otcepOnRc(m_RC *rc)
{
    for (int i=0;i<MAXCOUNT_OTCEPS;i++)
        if ((OTCEPS[i]->STATE_LOCATION()!=m_Otcep::locationUnknow)&& (OTCEPS[i]->vBusyRc.indexOf(rc)>=0)) return  OTCEPS[i];
    return 0;
}

m_RC *m_Otceps::find_RC(int chanelOffset)
{
    if (!mOffset2Rc.contains(chanelOffset)) return nullptr;
    return mOffset2Rc[chanelOffset];
//    if (l_rc.size()==0)
//        l_rc=parent()->findChildren<m_RC*>();
//    foreach (m_RC*rc, l_rc) {
//        if (chanelOffset==rc->SIGNAL_BUSY().chanelOffset())
//            return rc;
//    }
    return nullptr;
}

void m_Otceps::set_lrc(QList<m_RC *> &l)
{
    l_rc=l;
}

void m_Otceps::registerChanels()
{
    for (int i=0;i<MaxVagon;i++){
        chanelVag[i]=SignalDescription::getBuffer(15,QString("vag%1").arg(i+1));
        connect(chanelVag[i],&GtBuffer::bufferChanged,this,&m_Otceps::updateVagons);
    }

}

void m_Otceps::updateVagons()
{
    if (disableUpdateStates) return;
    for (int i=0;i<MAXCOUNT_OTCEPS;i++){
        if (OTCEPS[i]->disableUpdateStates) continue;
        OTCEPS[i]->vVag.clear();
        int lenvag=0;
        for (int i=0;i<MaxVagon;i++){
            tSlVagon * SlVagon=(tSlVagon *)chanelVag[i]->A.data();
            if (SlVagon){
                if (SlVagon->NO==0) break;
                if (SlVagon->NO<0) continue;
                if (OTCEPS[i]->NUM()==SlVagon->NO){
                    OTCEPS[i]->setSTATE_ENABLED(true);
                    if (OTCEPS[i]->STATE_ID_ROSP()==0)
                        OTCEPS[i]->setSTATE_ID_ROSP(SlVagon->Id);
                    if (OTCEPS[i]->STATE_MAR()==0)
                        OTCEPS[i]->setSTATE_MAR(mSP2MAR[SlVagon->SP]);
                    OTCEPS[i]->vVag.push_back(*SlVagon);
                    int Ln=0;
                    if (SlVagon->Ln==0) Ln=15;
                    if (SlVagon->Ln>1000) Ln=SlVagon->Ln/1000;
                    lenvag+=Ln;

                }
            }
        }
        OTCEPS[i]->setSTATE_LEN_BY_VAGON(lenvag);
    }
}


m_Otcep *m_Otceps::otcepADDR_SLOT(int ADDR_SLOT,int ADDR,int NTP)
{
    if ((NTP>=1)&&(NTP<=3)){
        for (int i=0;i<MAXCOUNT_OTCEPS;i++){
            if (OTCEPS[i]->STATE_LOCATION()!=m_Otcep::locationUnknow){
                int a=(ADDR_SLOT-1)*100+ADDR;
                if (OTCEPS[i]->stored_Descr.addr_tp[NTP-1]==a)return  OTCEPS[i];
            }
        }
    }
    return nullptr;
}

m_Otcep *m_Otceps::topOtcep() const
{
    // возвращаем первый в парке отпр
    foreach (auto otcep, l_otceps) {
        if ((otcep->STATE_ENABLED())&&(otcep->STATE_LOCATION()==m_Otcep::locationOnPrib)) return otcep;
    }
    return nullptr;
}

void m_Otceps::updateStates()
{
    if (disableUpdateStates) return;
//    m_Base::updateStates();
//    updateVagons();
}
