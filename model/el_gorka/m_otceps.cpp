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

    for (int i=0;i<MaxVagon;i++){
        l_vagons.push_back(new m_Vagon(this,i+1));
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

    modelGroupGorka=qobject_cast<ModelGroupGorka*>(parent());
    FTYPE_DESCR=0;
    foreach (m_Otcep*otcep, l_otceps) {
        otcep->updateAfterLoad();
    }
    foreach (auto *v, l_vagons) {

        v->updateAfterLoad();
    }


    if (modelGroupGorka){
        l_rc=parent()->findChildren<m_RC*>();
        foreach (m_RC *rc, l_rc) {
            if ((!rc->SIGNAL_BUSY().isNotUse()) &&(rc->SIGNAL_BUSY().chanelOffset()!=0)){
                mOffset2Rc[rc->SIGNAL_BUSY().chanelOffset()]=rc;
            }
            mID2Rc[rc->id()]=rc;
        }

    }
    m_Base::updateAfterLoad();
}

const QList<m_Otcep *> &m_Otceps::otceps() const
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
        if ((otcep->STATE_LOCATION()!=m_Otcep::locationUnknow)&& (otcep->vBusyRc.contains(rc))) return  otcep;
    return nullptr;
}

int m_Otceps::otcepCountOnRc(m_RC *rc)
{
    int count=0;
    foreach (m_Otcep*otcep, l_otceps)
        if ((otcep->STATE_LOCATION()!=m_Otcep::locationUnknow)&& (otcep->vBusyRc.contains(rc))) count++;
    return count;
}

QList<m_Otcep *> m_Otceps::otcepsOnRc(m_RC *rc)
{
    QList<m_Otcep *>l;
    foreach (m_Otcep*otcep, l_otceps)
        if ((otcep->STATE_LOCATION()!=m_Otcep::locationUnknow)&& (otcep->vBusyRc.contains(rc))) l.push_back(otcep);
    return l;
}

m_RC *m_Otceps::find_RC(int chanelOffset) const
{
    if (!mOffset2Rc.contains(chanelOffset)) return nullptr;
    return mOffset2Rc[chanelOffset];
}

QList<m_Otcep *> m_Otceps::enabledOtceps() const
{
    QList<m_Otcep *> l;
    foreach (m_Otcep*otcep, l_otceps)
        if (otcep->STATE_ENABLED()) l.push_back(otcep);
    return  l;
}


m_Otcep *m_Otceps::otcepADDR_SLOT(int ADDR_SLOT,int ADDR,int NTP)
{
    if ((NTP>=1)&&(NTP<=3)){
        foreach (m_Otcep*otcep, l_otceps) {
            if (otcep->STATE_LOCATION()!=m_Otcep::locationUnknow){
                int a=(ADDR_SLOT-1)*100+ADDR;
                if (otcep->STATE_ADDR_TP(NTP-1)==a)return  otcep;
            }
        }
    }
    return nullptr;
}

m_RC *m_Otceps::find_RC_ID(quint64 id) const
{
    if (mID2Rc.contains(id)) return mID2Rc[id];
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

bool m_Otceps::isFirstOtcep(m_Otcep *otcep)
{
    if (otcep==topOtcep()) return true;
    return false;
}

void m_Otceps::updateStates()
{
    if (disableUpdateStates) return;
    m_Base::updateStates();
    //    updateVagons();
}

void m_Otceps::otceps2Vagons()
{
    // перестроим модель вагонов
    foreach (auto v, l_vagons) {
        v->resetStates();
    }
    int iall=0;
    foreach (auto otcep, otceps()) {
        if (otcep->STATE_ENABLED()) {
            if (otcep->vVag.size()>0){
                for (int i=0;i<otcep->vVag.size();i++){
                    auto v=&otcep->vVag[i];
                    if (iall<l_vagons.size()){
                        auto vall=l_vagons[iall];
                        vall->assign(v);
                        vall->setSTATE_SP(otcep->STATE_SP());
                        vall->setSTATE_ENABLED(otcep->STATE_ENABLED());
                        vall->setSTATE_ID_ROSPUSK(otcep->STATE_ID_ROSP());
                        vall->setSTATE_N_IN_OTCEP(i+1);
                        vall->setSTATE_TICK(otcep->STATE_TICK());
                    }
                    iall++;
                }
            }  else {
                // добавляем пустой вагон
                if (iall<l_vagons.size()){
                    auto vall=l_vagons[iall];
                    vall->setSTATE_NUM_OTCEP(otcep->NUM());
                    vall->setSTATE_SP(otcep->STATE_SP());
                    vall->setSTATE_ENABLED(otcep->STATE_ENABLED());
                    vall->setSTATE_ID_ROSPUSK(otcep->STATE_ID_ROSP());
                    vall->setSTATE_LOCATION(otcep->STATE_LOCATION());
                    vall->setSTATE_N_IN_OTCEP(0);
                    vall->setSTATE_TICK(otcep->STATE_TICK());
                    iall++;
                }
            }
        }
    }
}

void m_Otceps::vagons2Otceps()
{
    foreach (auto otcep, otceps()) {
        if (otcep->STATE_ENABLED()) {
            foreach (auto v, l_vagons) {
                if (!v->STATE_ENABLED()) continue;
                if (v->STATE_NUM_OTCEP()==otcep->NUM()){
                    otcep->setVagon(v);
                }
            }
        }
    }
}
