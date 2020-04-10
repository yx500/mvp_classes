#include "m_dso_rd_21.h"

#include <QDebug>
#include "mvp_objectfactory.h"
REGISTERELEMENT(m_DSO_RD_21,"ДСО-РД-21","MODEL ГОРКА")

m_DSO_RD_21::m_DSO_RD_21(QObject *parent) :
    m_DSO(parent),
    FSIGNAL_B0(),FSIGNAL_B1(),FSIGNAL_K()

{
    resetStates();
}

void m_DSO_RD_21::resetStates()
{
    m_DSO::resetStates();
    FSTATE_B0=0;
    FSTATE_B1=0;
    FSTATE_K=0;
}

void m_DSO_RD_21::validation(ListObjStr *l) const
{
    m_DSO::validation(l);
    if ((FSIGNAL_B0==FSIGNAL_B1)&&(!FSIGNAL_B0.isEmpty()))
        l->warning(this,"Одинаковый сигнал срабатывания 0 и 1");
}

bool m_DSO_RD_21::is33()
{
    if (m_DSO::is33()) return true;
    return FSIGNAL_B0.is33() |
            FSIGNAL_B1.is33() |
            FSIGNAL_K.is33();
}

void m_DSO_RD_21::updateStates()
{
    m_DSO::updateStates();
    if (FSTATE_33) return;
    setSTATE_B0(FSIGNAL_B0.value_1bit());
    setSTATE_B1(FSIGNAL_B1.value_1bit());
    setSTATE_SRAB(FSTATE_B0*10+FSTATE_B1);
    if (FSIGNAL_K.value_1bit()==0)  setSTATE_ERROR(1); else
                                    setSTATE_ERROR(0);
}



