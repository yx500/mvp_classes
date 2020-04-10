#include "m_svet_gmc4.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(m_Svet_GMC4,"Светофор ГМЦ4","Модель Горка ГМЦ")


m_Svet_GMC4::m_Svet_GMC4(QObject *parent) : m_Svet_GMC(parent)
{
    FSTATE_DBK_ERROR2=0;
    FSTATE_DBK_OPEN2=0;
    FSTATE_DBK_OPENBLINK2=0;
    FSTATE_DBK_ERROR3=0;
    FSTATE_DBK_OPEN3=0;
    FSTATE_DBK_OPENBLINK3=0;
    FSTATE_DBK_ERROR4=0;
    FSTATE_DBK_OPEN4=0;
    FSTATE_DBK_OPENBLINK4=0;
}

void m_Svet_GMC4::updateAfterLoad()
{
    m_Svet_GMC::updateAfterLoad();
    updateSignals();
}

void m_Svet_GMC4::updateStates()
{
    m_Svet_GMC::updateStates();

    setSTATE_DBK_OPEN2(FSIGNAL_DBK_OPEN[2].value_1bit());
    setSTATE_DBK_OPENBLINK2(FSIGNAL_DBK_OPEN_BLINK[2].value_1bit());
    setSTATE_DBK_ERROR2(FSIGNAL_DBK_ERROR[2].value_1bit());

    setSTATE_DBK_OPEN3(FSIGNAL_DBK_OPEN[3].value_1bit());
    setSTATE_DBK_OPENBLINK3(FSIGNAL_DBK_OPEN_BLINK[3].value_1bit());
    setSTATE_DBK_ERROR3(FSIGNAL_DBK_ERROR[3].value_1bit());

    setSTATE_DBK_OPEN4(FSIGNAL_DBK_OPEN[4].value_1bit());
    setSTATE_DBK_OPENBLINK4(FSIGNAL_DBK_OPEN_BLINK[4].value_1bit());
    setSTATE_DBK_ERROR4(FSIGNAL_DBK_ERROR[4].value_1bit());

}

void m_Svet_GMC4::updateSignals()
{
    m_Svet_GMC::updateSignals();
    if ((FDBK_LINE>0)&&(FDBK_NUM>0)&&(FDBK_OBJ>0)){
        QString chanelNameDBK=FSIGNAL_DBK_OPEN[0].chanelName();
        FSIGNAL_DBK_OPEN[2].setSignalTS(      chanelNameDBK,8+(FDBK_OBJ-1+2)*4+0);
        FSIGNAL_DBK_OPEN_BLINK[2].setSignalTS(chanelNameDBK,8+(FDBK_OBJ-1+2)*4+1);
        FSIGNAL_DBK_ERROR[2].setSignalTS(     chanelNameDBK,8+(FDBK_OBJ-1+2)*4+2);
        FSIGNAL_DBK_OPEN[3].setSignalTS(      chanelNameDBK,8+(FDBK_OBJ-1+3)*4+0);
        FSIGNAL_DBK_OPEN_BLINK[3].setSignalTS(chanelNameDBK,8+(FDBK_OBJ-1+3)*4+1);
        FSIGNAL_DBK_ERROR[3].setSignalTS(     chanelNameDBK,8+(FDBK_OBJ-1+3)*4+2);
        FSIGNAL_DBK_OPEN[4].setSignalTS(      chanelNameDBK,8+(FDBK_OBJ-1+4)*4+0);
        FSIGNAL_DBK_OPEN_BLINK[4].setSignalTS(chanelNameDBK,8+(FDBK_OBJ-1+4)*4+1);
        FSIGNAL_DBK_ERROR[4].setSignalTS(     chanelNameDBK,8+(FDBK_OBJ-1+4)*4+2);
    }

}
