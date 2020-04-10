#include "m_strel_gor.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(m_Strel_Gor,"Горочная стрелка","MODEL ГОРКА")
REGISTERPROPERTY(m_Strel_Gor,NEXTM0,"СЛЕД.МИНУС","Следующая РЦ в минусовом направлении","СВЯЗИ")
REGISTERPROPERTY(m_Strel_Gor,NEXTM1,"ПРЕД.МИНУС","Предыдущая РЦ в минусовом направлении","СВЯЗИ")
REGISTERPROPERTY(m_Strel_Gor,DIRECTM,"НАПР.МИНУС","Направление минуса","СВЯЗИ")
REGISTERPROPERTY(m_Strel_Gor,SIGNAL_PLUS,"СИГНАЛ ПЛЮС","Сигнал плюсового положения","")
REGISTERPROPERTY(m_Strel_Gor,SIGNAL_MINUS,"СИГНАЛ МИНУС","Сигнал минусового положения","")


void m_Strel_Gor::setSTATE_POL(MVP_Enums::TStrelPol p)
{
    if (FSTATE_POL!=p){
        FSTATE_POL=p;
        emit stateChanged(this);

    }
}

m_Strel_Gor::m_Strel_Gor(QObject *parent):m_RC_Gor(parent)
{
    FSTATE_POL=MVP_Enums::TStrelPol::pol_plus;
    FSIGNAL_PLUS.clear();FSIGNAL_MINUS.clear();FSIGNAL_BLOCK.clear();
    FDIRECTM=0;
    resetStates();
}

void m_Strel_Gor::validation(ListObjStr *l) const
{
    m_RC_Gor::validation(l);
    if ((!FSIGNAL_PLUS.isNotUse()) &&(FSIGNAL_PLUS.isEmpty()))
        l->warning(this,"Не задан сигнал плюс");
    if ((!FSIGNAL_MINUS.isNotUse()) &&(FSIGNAL_MINUS.isEmpty()))
        l->warning(this,"Не задан сигнал минус");
    if ((!FSIGNAL_BLOCK.isNotUse()) &&(FSIGNAL_BLOCK.isEmpty()))
        l->warning(this,"Не задан сигнал блокировки");
}

void m_Strel_Gor::resetStates()
{
    m_RC_Gor::resetStates();
    FSTATE_BLOCK=false;
}

int m_Strel_Gor::getNextCount(int d) const
{
    if (!Fnext[d][1].isNull()) return 2;
    return 1;
}

bool m_Strel_Gor::is33()
{
    if (m_RC_Gor::is33()) return true;
    return FSIGNAL_PLUS.is33() |
           FSIGNAL_MINUS.is33() |
           FSIGNAL_BLOCK.is33();
}


void m_Strel_Gor::updateStates()
{
    m_RC_Gor::updateStates();
    if (FSTATE_33) return;

    bool plus=FSIGNAL_PLUS.value_1bit();
    bool minus=FSIGNAL_MINUS.value_1bit();
    MVP_Enums::TStrelPol p=MVP_Enums::TStrelPol::pol_w;
    if ((plus)&&(!minus))  p=MVP_Enums::TStrelPol::pol_plus;
    if ((!plus)&&(minus))  p=MVP_Enums::TStrelPol::pol_minus;
    SETPROP(FSTATE_POL);
    setSTATE_BLOCK(FSIGNAL_BLOCK.value_1bit());
}


