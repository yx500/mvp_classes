#include "m_otceps.h"
#include "mvp_system.h"
#include "m_controllerars.h"
#include "m_rc_gor_park.h"
#include "modelgroupgorka.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(m_Otceps,"ОТЦЕПЫ","MODEL ГОРКА")

m_Otceps::m_Otceps(QObject *parent) : m_Base(parent)
{
    FTYPE_DESCR=0;
    setObjectName("ОТЦЕПЫ");
    for (int i=0;i<MAXCOUNT_OTCEPS;i++){
        l_otceps.push_back(new m_Otcep(this,i+1));
    }
}

m_Otceps::~m_Otceps()
{
    l_otceps.clear();

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
            if (rc1->idstr()==rc2->idstr()) {
                l->error(this,"РЦ с одинаковым IDS",QString("%1 %2").arg(rc1->objectName(),rc2->objectName()));
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
    modelGroupGorka=qobject_cast<ModelGroupGorka*>(parent());
    foreach (m_Otcep*otcep, l_otceps) {
        if (FTYPE_DESCR==0){
            QString packetName=QString("descr%1").arg(otcep->NUM());
            otcep->setSIGNAL_DATA(SignalDescription(9,packetName,0));
        }
        if (FTYPE_DESCR==1){
            QString packetName=QString("descr%1").arg(otcep->NUM());
            otcep->setSIGNAL_DATA(SignalDescription(109,packetName,0));
        }
        otcep->updateAfterLoad();

    }
    if (FTYPE_DESCR==0){
        for (int i=0;i<MaxVagon;i++){
            chanelVag[i]=SignalDescription::getBuffer(15,QString("vag%1").arg(i+1));
            connect(chanelVag[i],&GtBuffer::bufferChanged,this,&m_Otceps::updateVagons);
        }
    }
    if (modelGroupGorka){
        l_rc=parent()->findChildren<m_RC*>();
        foreach (m_RC *rc, l_rc) {
            if ((!rc->SIGNAL_BUSY().isNotUse()) &&(rc->SIGNAL_BUSY().chanelOffset()!=0)){
                mOffset2Rc[rc->SIGNAL_BUSY().chanelOffset()]=rc;
                mIDS2Rc[rc->idstr()]=rc;
            }
        }

    }
}

QList<m_Otcep *> m_Otceps::otceps() const
{
    return l_otceps;

}


m_Otcep *m_Otceps::otcepByNum(int n)
{
    foreach (m_Otcep*otcep, l_otceps)
        if (otcep->NUM()==n) return otcep;
    return nullptr;
}

m_Otcep *m_Otceps::otcepOnRc(m_RC *rc)
{
    foreach (m_Otcep*otcep, l_otceps)
        if ((otcep->STATE_LOCATION()!=m_Otcep::locationUnknow)&& (otcep->vBusyRc.indexOf(rc)>=0)) return  otcep;
    return 0;
}

m_RC *m_Otceps::find_RC(int chanelOffset)
{
    if (!mOffset2Rc.contains(chanelOffset)) return nullptr;
    return mOffset2Rc[chanelOffset];
}




void m_Otceps::updateVagons()
{
    if (disableUpdateStates) return;
    tSlVagon * SlVagon0=(tSlVagon *)chanelVag[0]->A.data();
    int idrosp=SlVagon0->Id;
    foreach (m_Otcep*otcep, l_otceps) {
        int idr=0;
        int sp=0;
        if (otcep->disableUpdateStates) continue;
        otcep->vVag.clear();
        int lenvag=0;
        for (int j=0;j<MaxVagon;j++){
            tSlVagon * SlVagon=(tSlVagon *)chanelVag[j]->A.data();
            if (SlVagon){
                if (SlVagon->NO==0) break;
                if (SlVagon->Id!=idrosp)break;
                if (SlVagon->NO<0) continue;
                if (otcep->NUM()==SlVagon->NO){
                    idr=SlVagon->Id;
                    sp=SlVagon->SP;
                    otcep->vVag.push_back(*SlVagon);
                    int Ln=0;
                    if (SlVagon->Ln==0) Ln=15;
                    if (SlVagon->Ln>1000) Ln=SlVagon->Ln/1000;
                    lenvag+=Ln;
                }
            }
        }
        otcep->setSTATE_ID_ROSP_VAG(idr);
        otcep->setSTATE_SP_VAG(sp);
        otcep->setSTATE_LEN_BY_VAGON(lenvag);
    }
}


m_Otcep *m_Otceps::otcepADDR_SLOT(int ADDR_SLOT,int ADDR,int NTP)
{
    if ((NTP>=1)&&(NTP<=3)){
        foreach (m_Otcep*otcep, l_otceps) {
            if (otcep->STATE_LOCATION()!=m_Otcep::locationUnknow){
                int a=(ADDR_SLOT-1)*100+ADDR;
                if (otcep->stored_Descr.addr_tp[NTP-1]==a)return  otcep;
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
