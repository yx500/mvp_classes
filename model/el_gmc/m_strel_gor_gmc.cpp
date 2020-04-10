#include "m_strel_gor_gmc.h"

#include "baseobjecttools.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(m_Strel_Gor_GMC,"Горочная стрелка ГМЦ","MODEL Горка ГМЦ")


m_Strel_Gor_GMC::m_Strel_Gor_GMC(QObject *parent) : m_Strel_Gor(parent)
{
    FIPD.clear();
    FRTDS.clear();
    FSIGNAL_ZAM.clear();FSTATE_ZAM=0;
    FSIGNAL_A.clear();FSTATE_A=0;
    FSIGNAL_PRP_ENABLE.clear();FSTATE_PRP_ENABLE=0;
    FSIGNAL_PRM_ENABLE.clear();FSTATE_PRM_ENABLE=0;
    FSIGNAL_PRP.clear();FSTATE_PRP=0;
    FSIGNAL_PRM.clear();FSTATE_PRM=0;
    FSIGNAL_BLOK.clear();FSTATE_BLOK=0;
    FSIGNAL_PKS.clear();FSTATE_PKS=0;
    FSIGNAL_PRV.clear();FSTATE_PRV=0;
    FDBK_LINE=0;
    FDBK_NUM=0;
    FDBK_OBJ=0;
}

void m_Strel_Gor_GMC::validation(ListObjStr *l) const
{
    m_Strel_Gor::validation(l);

    if ((!FIPD.isNotUse())&&(FIPD.isNull()))
        l->error(this,"Не задан ИПД");
    if ((!FRTDS.isNotUse())&&(FRTDS.isNull()))
        l->error(this,"Не задан РТДС");

}

void m_Strel_Gor_GMC::updateAfterLoad()
{
    m_Strel_Gor::updateAfterLoad();
    if (!FIPD.isNotUse()){
        FIPD.linkObj(superFindObjectById(this,FIPD.id()));
        if (FIPD.isNull())
            qCritical() << objectName() << "Ошибочная ссылка IPD" <<endl ;
    }
    if (!FRTDS.isNotUse()){
        FRTDS.linkObj(superFindObjectById(this,FRTDS.id()));
        if (FRTDS.isNull())
            qCritical() << objectName() << "Ошибочная ссылка RTDS" <<endl ;
    }
    updateSignals();

}

bool m_Strel_Gor_GMC::is33()
{
    if (m_Strel_Gor::is33()) return true;
    return FSIGNAL_ZAM.is33();
}

void m_Strel_Gor_GMC::updateStates()
{
    m_Strel_Gor::updateStates();
    if (FSTATE_33) return;
    setSTATE_ZAM(FSIGNAL_ZAM.value_1bit());
    setSTATE_BLOK(FSIGNAL_BLOK.value_1bit());
    setSTATE_A(FSIGNAL_A.value_1bit());
    setSTATE_PRP_ENABLE(FSIGNAL_PRP_ENABLE.value_1bit());
    setSTATE_PRM_ENABLE(FSIGNAL_PRM_ENABLE.value_1bit());
    setSTATE_PRP(FSIGNAL_PRP.value_1bit());
    setSTATE_PRM(FSIGNAL_PRM.value_1bit());
    setSTATE_PKS(FSIGNAL_PKS.value_1bit());
    setSTATE_PRV(FSIGNAL_PRV.value_1bit());
}



void m_Strel_Gor_GMC::setDBK_LINE(int p)
{
    FDBK_LINE=p;
    updateSignals();
}

void m_Strel_Gor_GMC::setDBK_NUM(int p)
{
    FDBK_NUM=p;
    updateSignals();
}

void m_Strel_Gor_GMC::setDBK_OBJ(int p)
{
    FDBK_OBJ=p;
    updateSignals();
}
void m_Strel_Gor_GMC::updateSignals()
{
    if ((FDBK_LINE>0)&&(FDBK_NUM>0)&&(FDBK_OBJ>0)){
        QString chanelNameGMC=QString("GMC_STR");
        QString chanelNameDBK=QString("DBK_STR%1").arg(FDBK_NUM);
        int N=(FDBK_NUM-1)*4+FDBK_OBJ-1;
        FSIGNAL_BUSY.setSignalTS(       chanelNameGMC,0+N*8+0);
        FSIGNAL_ZAM.setSignalTS(        chanelNameGMC,0+N*8+1);
        FSIGNAL_BLOK.setSignalTS(       chanelNameGMC,0+N*8+2);
        FSIGNAL_A.setSignalTS(          chanelNameGMC,0+N*8+3);
        FSIGNAL_PRP_ENABLE.setSignalTS( chanelNameGMC,0+N*8+4);
        FSIGNAL_PRM_ENABLE.setSignalTS( chanelNameGMC,0+N*8+5);
        FSIGNAL_PRP.setSignalTS(        chanelNameGMC,0+N*8+6);
        FSIGNAL_PRM.setSignalTS(        chanelNameGMC,0+N*8+7);

        FSIGNAL_PLUS.setSignalTS(       chanelNameDBK,8+(FDBK_OBJ-1)*8+0);
        FSIGNAL_MINUS.setSignalTS(      chanelNameDBK,8+(FDBK_OBJ-1)*8+1);
        FSIGNAL_PKS.setSignalTS(        chanelNameDBK,8+(FDBK_OBJ-1)*8+2);
        FSIGNAL_PRV.setSignalTS(        chanelNameDBK,8+(FDBK_OBJ-1)*8+3);

    }
}

