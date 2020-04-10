#include "m_svet_gor_1.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(m_Svet_Gor_1,"Светофор горочный 1зн","MODEL ГОРКА")

m_Svet_Gor_1::m_Svet_Gor_1(QObject *parent) : m_Svet(parent)
{
    FSIGNAL_OPEN.clear();
    FSIGNAL_OPEN_BLINK.clear();
    FSIGNAL_OPEN_BLINK.setIsNoUse(true);
    resetStates();
}

void m_Svet_Gor_1::resetStates()
{
    m_Svet::resetStates();
    FSTATE_OPEN_BLINK=false;
}

void m_Svet_Gor_1::validation(ListObjStr *l) const
{
    m_Svet::validation(l);
    if ((!FSIGNAL_OPEN.isNotUse()) &&(FSIGNAL_OPEN.isEmpty()))
        l->warning(this,"Не задан сигнал открытия");
    if ((!FSIGNAL_OPEN_BLINK.isNotUse()) &&(FSIGNAL_OPEN_BLINK.isEmpty()))
        l->warning(this,"Не задан сигнал мигания");
}

bool m_Svet_Gor_1::is33()
{
    if (m_Svet::is33()) return true;
    return FSIGNAL_OPEN.is33() |
            FSIGNAL_OPEN_BLINK.is33();
}

void m_Svet_Gor_1::updateStates()
{
    m_Svet::updateStates();
    if (FSTATE_33) return;
    setSTATE_OPEN(FSIGNAL_OPEN.value_1bit());
    setSTATE_OPEN_BLINK(FSIGNAL_OPEN_BLINK.value_1bit());
}
