#include "m_dso.h"

#include "baseobjecttools.h"
#include <QDebug>
#include "mvp_objectfactory.h"




m_DSO::m_DSO(QObject *parent) :
    m_Base(parent),
    FSTATE_OSY_COUNT(0),FSTATE_ERROR(0)
{
    FSIGNAL_DSODATA.clear();
    FRC_OFFSET=0;
    FNGBDYN_OFFSET=0;
    FRC.clear();
    FRC_NEXT.clear();
    FRC_PREV.clear();

    rc=nullptr;rc_next[0]=nullptr; rc_next[1]=nullptr;
    dso_dubl=nullptr;
    dso_pair=nullptr;;
    resetStates();
}

void m_DSO::resetStates()
{
    m_Base::resetStates() ;
    FSTATE_OSY_COUNT=0;
    FSTATE_ERROR=0;
    FSTATE_DIRECT=0;
    FSTATE_SRAB=0;
    FSTATE_ERROR_TRACK=false;
    FSTATE_ERROR_CNT=0;
}

void m_DSO::validation(ListObjStr *l) const
{
    m_Base::validation(l);
    if ((!FRC.isNotUse())&&((FRC.isNull())||(qobject_cast<m_RC*>(FRC.baseObject())==nullptr)))
        l->warning(this,"Нет привязки к РЦ","","RC");
    if ((!FRC_NEXT.isNotUse())&&((FRC_NEXT.isNull())||(qobject_cast<m_RC*>(FRC_NEXT.baseObject())==nullptr)))
        l->warning(this,"Нет привязки к РЦ","","RC_NEXT");
    if ((!FRC_PREV.isNotUse())&&((FRC_PREV.isNull())||(qobject_cast<m_RC*>(FRC_PREV.baseObject())==nullptr)))
        l->warning(this,"Нет привязки к РЦ","","RC_PREV");
    if ((rc_next[0]!=nullptr) && (rc_next[1]!=nullptr)){
        if ((rc_next[0]->getNextRC(1,0)!=rc_next[1]) &&
            (rc_next[0]->getNextRC(1,1)!=rc_next[1]))
            l->error(this,"Разные PREV у ДСО и РЦ","","RC_PREV");
        if ((rc_next[1]->getNextRC(0,0)!=rc_next[0]) &&
            (rc_next[1]->getNextRC(0,1)!=rc_next[0]))
            l->error(this,"Разные NEXT у ДСО и РЦ","","RC_NEXT");
    }

    if ((!FDSO_DUBL.isNotUse())&&((FDSO_DUBL.isNull())||(qobject_cast<m_DSO*>(FDSO_DUBL.baseObject())==nullptr)))
        l->warning(this,"Нет привязки к дубликату","","DSO_DUBL");
    if ((!FDSO_PAIR.isNotUse())&&((FDSO_PAIR.isNull())||(qobject_cast<m_DSO*>(FDSO_PAIR.baseObject())==nullptr)))
        l->warning(this,"Нет привязки к паре","","DSO_PAIR");

    if (dso_dubl!=nullptr){
        if (dso_dubl->dso_dubl!=this)
            l->warning(this,"У дубля нет обратной привязки","","DSO_DUBL");
    }


    if (FSIGNAL_DSODATA.chanelOffset()>DSO_Data_Max){
        l->error(this,"Выход за пределы буфера","","SIGNAL_DSODATA");
    }

}

void m_DSO::updateAfterLoad()
{
    m_Base::updateAfterLoad();
    updateLink(FRC);
    if ((!FRC.isNull())&&(qobject_cast<m_RC*>(FRC.baseObject())==nullptr)){
        qWarning() << objectName() << "ДСО ссылается не на РЦ " << FRC.baseObject()->objectName();
        FRC.clear();
    } else {
        rc=qobject_cast<m_RC*>(FRC.baseObject());
    }
    rc_next[0]=nullptr;
    updateLink(FRC_NEXT);
    if ((!FRC_NEXT.isNull())&&(qobject_cast<m_RC*>(FRC_NEXT.baseObject())==nullptr)){
        qWarning() << objectName() << "ДСО ссылается не на РЦ " << FRC_NEXT.baseObject()->objectName();
        FRC_NEXT.clear();
    } else {
        rc_next[0]=qobject_cast<m_RC*>(FRC_NEXT.baseObject());
    }
    rc_next[1]=nullptr;
    updateLink(FRC_PREV);
    if ((!FRC_PREV.isNull())&&(qobject_cast<m_RC*>(FRC_PREV.baseObject())==nullptr)){
        qWarning() << objectName() << "ДСО ссылается не на РЦ " << FRC_PREV.baseObject()->objectName();
        FRC_PREV.clear();
    } else {
        rc_next[1]=qobject_cast<m_RC*>(FRC_PREV.baseObject());
    }
    dso_dubl=nullptr;
    updateLink(FDSO_DUBL);
    if ((!FDSO_DUBL.isNull())&&(qobject_cast<m_DSO*>(FDSO_DUBL.baseObject())==nullptr)){
        qWarning() << objectName() << "ДСО ДУБЛЬ ссылается не на ДСО " << FDSO_DUBL.baseObject()->objectName();
        FRC_PREV.clear();
    } else {
        dso_dubl=qobject_cast<m_DSO*>(FDSO_DUBL.baseObject());
    }
    dso_pair=nullptr;
    updateLink(FDSO_PAIR);
    if ((!FDSO_PAIR.isNull())&&(qobject_cast<m_DSO*>(FDSO_PAIR.baseObject())==nullptr)){
        qWarning() << objectName() << "ДСО ПАРА ссылается не на ДСО " << FDSO_PAIR.baseObject()->objectName();
        FDSO_PAIR.clear();
    } else {
        dso_pair=qobject_cast<m_DSO*>(FDSO_PAIR.baseObject());
    }
}

void m_DSO::updateStates()
{
    m_Base::updateStates();
    if ((FSIGNAL_DSODATA.isUsed())&&(!FSIGNAL_DSODATA.isInnerUse())){
        DSO_Data *d=(DSO_Data *)FSIGNAL_DSODATA.value_data(sizeof(DSO_Data));
        if (d!=nullptr){
            setSTATE_OSY_COUNT(d->V);
            setSTATE_DIRECT(d->D);
            setSTATE_ERROR_TRACK(d->E);
            setSTATE_ERROR_CNT(d->EV);
        } else {
            setSTATE_OSY_COUNT(0);
            setSTATE_DIRECT(0);
            setSTATE_ERROR_TRACK(false);
            setSTATE_ERROR_CNT(0);
        }
    }
}
void m_DSO::reset()
{
    setSTATE_OSY_COUNT(0);
    setSTATE_DIRECT(0);
    setSTATE_ERROR_TRACK(false);
    setSTATE_ERROR_CNT(0);
}






