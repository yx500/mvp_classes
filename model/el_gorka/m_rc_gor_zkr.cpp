#include "m_rc_gor_zkr.h"

#include "baseobjecttools.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(m_RC_Gor_ZKR,"РЦ ЗКР","MODEL ГОРКА")

REGISTERPROPERTY(m_RC_Gor_ZKR,RTDS_USL_OR,"Условия срабатывания РТДС","и / или","")



m_RC_Gor_ZKR::m_RC_Gor_ZKR(QObject *parent) : m_RC_Gor(parent)
{
    controllerARSNadvig=nullptr;
    svetZKR=nullptr;
    FPUT_NADVIG=0;
    FSIGNAL_ROSPUSK.clear();
    FSIGNAL_STATE_ERROR_RTDS.clear();
    FSIGNAL_STATE_ERROR_NERASCEP.clear();
    FSIGNAL_STATE_ERROR_OSYCOUNT.clear();
    FRTDS_USL_OR=false;
    memset(dso,0,sizeof(dso));
    resetStates();
}

void m_RC_Gor_ZKR::resetStates()
{
    m_RC_Gor::resetStates();
    FSTATE_ERROR_RTDS=false;
    FSTATE_ROSPUSK=false;
    FSTATE_ERROR_NERASCEP=false;
    FSTATE_ERROR_OSYCOUNT=false;
}




void m_RC_Gor_ZKR::validation(ListObjStr *l) const
{
    m_RC_Gor::validation(l);
    if ((FPUT_NADVIG!=1) && (FPUT_NADVIG!=2))
        l->warning(this,"Не задан путь роспуска");

}
void m_RC_Gor_ZKR::updateAfterLoad()
{
    m_RC_Gor::updateAfterLoad();

    controllerARSNadvig=qobject_cast<m_ControllerARS*>(updateLink(FCONTR_NADVIG));
    if (!controllerARSNadvig) qCritical() << objectName() << "Ошибочная ссылка CONTR_NADVIG" <<endl ; else addDevice(controllerARSNadvig);

    svetZKR=qobject_cast<m_Svet_Gor_4N*>(updateLink(FSVET));
    if (!svetZKR) qCritical() << objectName() << "Ошибочная ссылка SVET" <<endl ; else addDevice(svetZKR);
    rtds1=qobject_cast<m_RTDS*>(updateLink(FRTDS1));
    if (!rtds1) qCritical() << objectName() << "Ошибочная ссылка RTDS1" <<endl ; else addDevice(rtds1);
    rtds2=qobject_cast<m_RTDS*>(updateLink(FRTDS2));
    if (!rtds2) qCritical() << objectName() << "Ошибочная ссылка RTDS2" <<endl ; else addDevice(rtds2);


    dso[0][0]=qobject_cast<m_DSO*>(updateLink(FDSO11));
    dso[0][1]=qobject_cast<m_DSO*>(updateLink(FDSO12));
    dso[1][0]=qobject_cast<m_DSO*>(updateLink(FDSO21));
    dso[1][1]=qobject_cast<m_DSO*>(updateLink(FDSO22));
    for (int d=0;d<2;d++)
        for (int m=0;m<2;m++)
            if (!dso[d][m]) qCritical() << objectName() << QString("Ошибочная ссылка DSO%1%2").arg(d+1).arg(m+1) <<endl ; else addDevice(dso[0][0]);
}

bool m_RC_Gor_ZKR::is33()
{
//    if (m_RC_Gor::is33()) return true;
    return FSIGNAL_ROSPUSK.is33();
}

void m_RC_Gor_ZKR::updateStates()
{
    m_RC_Gor::updateStates();
    setSignalState(FSIGNAL_ROSPUSK,FSTATE_ROSPUSK);
    setSignalState(FSIGNAL_STATE_ERROR_RTDS,FSTATE_ERROR_RTDS);
    setSignalState(FSIGNAL_STATE_ERROR_NERASCEP,FSTATE_ERROR_NERASCEP);
    setSignalState(FSIGNAL_STATE_ERROR_OSYCOUNT,FSTATE_ERROR_OSYCOUNT);
}
