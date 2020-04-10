#include "m_svet_gor_4n.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(m_Svet_Gor_4N,"Светофор горочный 4знН","MODEL ГОРКА")

m_Svet_Gor_4N::m_Svet_Gor_4N(QObject *parent) : m_Svet(parent)
{
    FSTATE_OPEN=false;
    FSIGNAL_K.clear();
    FSIGNAL_J.clear();
    FSIGNAL_Z.clear();
    FSIGNAL_B.clear();
    FSIGNAL_NAZAD.clear();

    FSTATE_K=false;
    FSTATE_J=false;
    FSTATE_Z=false;
    FSTATE_B=false;
    FSTATE_NAZAD=false;
}

void m_Svet_Gor_4N::validation(ListObjStr *l) const
{
    m_Svet::validation(l);
    //    if ((!FSIGNAL_K.isNotUse()) &&(FSIGNAL_K.isEmpty())) l->warning(this,"Не задан сигнал красного огня");
    //    if ((!FSIGNAL_J.isNotUse()) &&(FSIGNAL_J.isEmpty())) l->warning(this,"Не задан сигнал желтого огня");
    //    if ((!FSIGNAL_Z.isNotUse()) &&(FSIGNAL_Z.isEmpty())) l->warning(this,"Не задан сигнал зеленого огня");
    //    if ((!FSIGNAL_B.isNotUse()) &&(FSIGNAL_B.isEmpty())) l->warning(this,"Не задан сигнал белого огня");
    //    if ((!FSIGNAL_NAZAD.isNotUse()) &&(FSIGNAL_NAZAD.isEmpty())) l->warning(this,"Не задан сигнал НАЗАД");
}

bool m_Svet_Gor_4N::is33()
{
    if (m_Svet::is33()) return true;
    return FSIGNAL_K.is33() | FSIGNAL_J.is33() | FSIGNAL_Z.is33() | FSIGNAL_B.is33() | FSIGNAL_NAZAD.is33();

}

void m_Svet_Gor_4N::updateStates()
{
    m_Svet::updateStates();
    if (FSTATE_33) return;

    setSTATE_K(FSIGNAL_K.value_1bit());
    setSTATE_J(FSIGNAL_J.value_1bit());
    setSTATE_Z(FSIGNAL_Z.value_1bit());
    setSTATE_B(FSIGNAL_B.value_1bit());
    setSTATE_NAZAD(FSIGNAL_NAZAD.value_1bit());

    setSTATE_OPEN(FSTATE_J | FSTATE_Z | FSTATE_B);
}
