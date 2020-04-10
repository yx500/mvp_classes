#include "m_svet_gmc.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(m_Svet_GMC,"Светофор ГМЦ","MODEL Горка ГМЦ")

m_Svet_GMC::m_Svet_GMC(QObject *parent) : m_Svet(parent)
{

    FSIGNAL_GMC_OPEN_ENABLE.clear();FSTATE_GMC_OPEN_ENABLE=0;
    FSIGNAL_GMC_BLOCK.clear();      FSTATE_GMC_BLOCK=0;
    FSIGNAL_GMC_TU_OPEN.clear();    FSTATE_GMC_TU_OPEN=0;
    FSIGNAL_GMC_TU_CLOSE.clear();   FSTATE_GMC_TU_CLOSE=0;
    FSIGNAL_GMC_ERROR.clear();      FSTATE_GMC_ERROR=0;
    FSIGNAL_GMC_OPEN_CONDITION.clear();FSTATE_GMC_OPEN_CONDITION=0;
    for(int i=0;i<5;i++){
        FSIGNAL_DBK_ERROR[i].clear();
        FSIGNAL_DBK_OPEN[i].clear();
        FSIGNAL_DBK_OPEN_BLINK[i].clear();
    }
    for(int i=0;i<2;i++){
        FSIGNAL_DBK_ERROR[i].setIsNoUse(true);
        FSIGNAL_DBK_OPEN[i].setIsNoUse(true);
        FSIGNAL_DBK_OPEN_BLINK[i].setIsNoUse(true);
    }
    FSTATE_DBK_ERROR0=0;
    FSTATE_DBK_OPEN0=0;
    FSTATE_DBK_OPENBLINK0=0;
    FSTATE_DBK_ERROR1=0;
    FSTATE_DBK_OPEN1=0;
    FSTATE_DBK_OPENBLINK1=0;

    FDBK_LINE=0;
    FDBK_NUM=0;
    FDBK_OBJ=0;
}

void m_Svet_GMC::validation(ListObjStr *l) const
{
    m_Svet::validation(l);
    if ((!FSIGNAL_GMC_OPEN_ENABLE.isNotUse()) &&(FSIGNAL_GMC_OPEN_ENABLE.isEmpty()))
        l->warning(this,"Не задан сигнал GMC_OPEN_ENABLE");
    if ((!FSIGNAL_GMC_BLOCK.isNotUse()) &&(FSIGNAL_GMC_BLOCK.isEmpty()))
        l->warning(this,"Не задан сигнал GMC_BLOCK");
    if ((!FSIGNAL_GMC_TU_OPEN.isNotUse()) &&(FSIGNAL_GMC_TU_OPEN.isEmpty()))
        l->warning(this,"Не задан сигнал GMC_TU_OPEN");
    if ((!FSIGNAL_GMC_TU_CLOSE.isNotUse()) &&(FSIGNAL_GMC_TU_CLOSE.isEmpty()))
        l->warning(this,"Не задан сигнал GMC_TU_CLOSE");
    if ((!FSIGNAL_GMC_ERROR.isNotUse()) &&(FSIGNAL_GMC_ERROR.isEmpty()))
        l->warning(this,"Не задан сигнал GMC_ERROR");
    if ((!FSIGNAL_GMC_OPEN_CONDITION.isNotUse()) &&(FSIGNAL_GMC_OPEN_CONDITION.isEmpty()))
        l->warning(this,"Не задан сигнал GMC_OPEN_CONDITION");

    if ((!FSIGNAL_DBK_ERROR[0].isNotUse()) &&(FSIGNAL_DBK_ERROR[0].isEmpty()))
        l->warning(this,"Не задан сигнал DBK_ERROR[0]");
}

void m_Svet_GMC::updateAfterLoad()
{
    m_Svet::updateAfterLoad();
    updateSignals();

}

bool m_Svet_GMC::is33()
{
     if (m_Svet::is33()) return true;
     return FSIGNAL_GMC_BLOCK.is33() |
            FSIGNAL_DBK_ERROR[0].is33() |
            FSIGNAL_GMC_OPEN_CONDITION.is33();
}

void m_Svet_GMC::updateStates()
{
    m_Svet::updateStates();
    if (FSTATE_33) return;

    setSTATE_GMC_OPEN_ENABLE(FSIGNAL_GMC_OPEN_ENABLE.value_1bit());
    setSTATE_GMC_BLOCK(FSIGNAL_GMC_BLOCK.value_1bit());
    setSTATE_GMC_TU_OPEN(FSIGNAL_GMC_TU_OPEN.value_1bit());
    setSTATE_GMC_TU_CLOSE(FSIGNAL_GMC_TU_CLOSE.value_1bit());
    setSTATE_GMC_ERROR(FSIGNAL_GMC_ERROR.value_1bit());
    setSTATE_GMC_OPEN_CONDITION(FSIGNAL_GMC_OPEN_CONDITION.value_1bit());

    setSTATE_DBK_OPEN0(FSIGNAL_DBK_OPEN[0].value_1bit());
    setSTATE_DBK_OPENBLINK0(FSIGNAL_DBK_OPEN_BLINK[0].value_1bit());
    setSTATE_DBK_ERROR0(FSIGNAL_DBK_ERROR[0].value_1bit());

    setSTATE_DBK_OPEN1(FSIGNAL_DBK_OPEN[1].value_1bit());
    setSTATE_DBK_OPENBLINK1(FSIGNAL_DBK_OPEN_BLINK[1].value_1bit());
    setSTATE_DBK_ERROR1(FSIGNAL_DBK_ERROR[1].value_1bit());

}

void m_Svet_GMC::setDBK_LINE(int p)
{
    FDBK_LINE=p;
    updateSignals();
}

void m_Svet_GMC::setDBK_NUM(int p)
{
    FDBK_NUM=p;
    updateSignals();
}

void m_Svet_GMC::setDBK_OBJ(int p)
{
    FDBK_OBJ=p;
    updateSignals();
}


void m_Svet_GMC::updateSignals()
{
    if ((FDBK_LINE>0)&&(FDBK_NUM>0)&&(FDBK_OBJ>0)){
        QString chanelNameGMC=QString("GMC_SVT");
        QString chanelNameDBK=QString("DBK_SVT%1").arg(FDBK_NUM);
        FSIGNAL_DBK_OPEN[0].setSignalTS(      chanelNameDBK,8+(FDBK_OBJ-1+0)*4+0);
        FSIGNAL_DBK_OPEN_BLINK[0].setSignalTS(chanelNameDBK,8+(FDBK_OBJ-1+0)*4+1);
        FSIGNAL_DBK_ERROR[0].setSignalTS(     chanelNameDBK,8+(FDBK_OBJ-1+0)*4+2);
        FSIGNAL_DBK_OPEN[1].setSignalTS(      chanelNameDBK,8+(FDBK_OBJ-1+1)*4+0);
        FSIGNAL_DBK_OPEN_BLINK[1].setSignalTS(chanelNameDBK,8+(FDBK_OBJ-1+1)*4+1);
        FSIGNAL_DBK_ERROR[1].setSignalTS(     chanelNameDBK,8+(FDBK_OBJ-1+1)*4+2);
        int N=(FDBK_NUM-1)*12+FDBK_OBJ-1;

        FSIGNAL_GMC_OPEN_ENABLE.setSignalTS(    chanelNameGMC,0+N*8+0);
        FSIGNAL_GMC_BLOCK.setSignalTS(          chanelNameGMC,0+N*8+1);
        FSIGNAL_GMC_TU_OPEN.setSignalTS(        chanelNameGMC,0+N*8+2);
        FSIGNAL_GMC_ERROR.setSignalTS(          chanelNameGMC,0+N*8+3);
        FSIGNAL_GMC_OPEN_CONDITION.setSignalTS( chanelNameGMC,0+N*8+4);
        FSIGNAL_GMC_TU_CLOSE.setSignalTS(       chanelNameGMC,0+N*8+5);

    }
}
