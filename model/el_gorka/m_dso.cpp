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

    resetStates();
}

void m_DSO::resetStates()
{
    m_Base::resetStates() ;
    FSTATE_OSY_COUNT=0;
    FSTATE_ERROR=0;
    FSTATE_DIRECT=0;
    FSTATE_SRAB=0;
}

void m_DSO::validation(ListObjStr *l) const
{
    m_Base::validation(l);
    if (FRC.isNull())
        l->warning(this,"Нет привязки к РЦ");

}

void m_DSO::updateAfterLoad()
{
    m_Base::updateAfterLoad();
    FRC.linkObj(superFindObjectById(this,FRC.id()));
    if ((!FRC.isNull())&&(dynamic_cast<m_RC*>(FRC.obj())==nullptr)){
        qWarning() << objectName() << "ДСО ссылается не на РЦ " << FRC.obj()->objectName();
        FRC.clear();
    }
}

void m_DSO::updateStates()
{
    m_Base::updateStates();
    if ((FSIGNAL_DSODATA.isUsed())&&(!FSIGNAL_DSODATA.isInnerUse())){
        DSO_Data *d=(DSO_Data *)FSIGNAL_DSODATA.value_data(sizeof(DSO_Data));
        if (d!=nullptr){
            setSTATE_OSY_COUNT(d->count);
            setSTATE_DIRECT(d->direct);
        } else {
            setSTATE_OSY_COUNT(0);
            setSTATE_DIRECT(0);
        }
    }
}
void m_DSO::reset()
{
    setSTATE_OSY_COUNT(0);
    setSTATE_DIRECT(0);
}






