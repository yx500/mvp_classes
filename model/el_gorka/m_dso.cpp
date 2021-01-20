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
    FRC.clear();
    FRC_NEXT.clear();
    FRC_PREV.clear();


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

}

void m_DSO::updateAfterLoad()
{
    m_Base::updateAfterLoad();
    updateLink(FRC);
    if ((!FRC.isNull())&&(qobject_cast<m_RC*>(FRC.baseObject())==nullptr)){
        qWarning() << objectName() << "ДСО ссылается не на РЦ " << FRC.baseObject()->objectName();
        FRC.clear();
    }
    updateLink(FRC_NEXT);
    if ((!FRC_NEXT.isNull())&&(qobject_cast<m_RC*>(FRC_NEXT.baseObject())==nullptr)){
        qWarning() << objectName() << "ДСО ссылается не на РЦ " << FRC_NEXT.baseObject()->objectName();
        FRC_NEXT.clear();
    }
    updateLink(FRC_PREV);
    if ((!FRC_PREV.isNull())&&(qobject_cast<m_RC*>(FRC_PREV.baseObject())==nullptr)){
        qWarning() << objectName() << "ДСО ссылается не на РЦ " << FRC_PREV.baseObject()->objectName();
        FRC_PREV.clear();
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






