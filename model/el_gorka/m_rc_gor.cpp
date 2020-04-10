#include "m_rc_gor.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(m_RC_Gor,"Горочная РЦ","MODEL ГОРКА")

REGISTERPROPERTY(m_RC_Gor,MINWAY,"MINWAY","MINWAY","WAY")
REGISTERPROPERTY(m_RC_Gor,MAXWAY,"MAXWAY","MAXWAY","WAY")
REGISTERPROPERTY(m_RC_Gor,MINWAY1,"MINWAY1","MINWAY1","WAY")
REGISTERPROPERTY(m_RC_Gor,MAXWAY1,"MAXWAY1","MAXWAY1","WAY")
REGISTERPROPERTY(m_RC,SIGNAL_WAY1,"СИГНАЛ ВЫБОР WAY1","СИГНАЛ ВЫБОР WAY1","WAY")



m_RC_Gor::m_RC_Gor(QObject *parent) :
    m_RC(parent),FMINWAY(0),FMAXWAY(0),FMINWAY1(0),FMAXWAY1(0),FSIGNAL_WAY1()
{
    FSTATE_BUSY=MVP_Enums::TRCBusy::free;
    FSIGNAL_WAY1.setIsNoUse(true);
    resetStates();
}

void m_RC_Gor::validation(ListObjStr *l) const
{
    m_RC::validation(l);
}

void m_RC_Gor::resetStates()
{
    m_RC::resetStates();
    FSTATE_WAY1=false;
}

void m_RC_Gor::updateStates()
{
    m_RC::updateStates();
    setSTATE_WAY1(FSIGNAL_WAY1.value_1bit());
}


