#include "m_rtds.h"

#include "baseobjecttools.h"
#include <QDebug>
#include "mvp_objectfactory.h"
REGISTERELEMENT(m_RTDS,"РТДС","MODEL ГОРКА")




m_RTDS::m_RTDS(QObject *parent) : m_Base(parent)
{
    FSIGNAL_SRAB.clear();
    resetStates();
}

void m_RTDS::resetStates()
{
    m_Base::resetStates();
    FSTATE_SRAB=0;
}

void m_RTDS::validation(ListObjStr *l) const
{
    m_Base::validation(l);
    if ((FSIGNAL_SRAB.isEmpty()))
        l->warning(this,"Не задан сигнал срабатывания 0");

    if (objectCountLinked2Object(this)==0){
        l->warning(this,"Не используется");
    }
}

void m_RTDS::updateAfterLoad()
{
    m_Base::updateAfterLoad();
}

bool m_RTDS::is33()
{
    if (m_Base::is33()) return true;
    return FSIGNAL_SRAB.is33();
}

void m_RTDS::updateStates()
{
    m_Base::updateStates();
    if (FSTATE_33) return;
    setSTATE_SRAB(FSIGNAL_SRAB.value_1bit());
}

