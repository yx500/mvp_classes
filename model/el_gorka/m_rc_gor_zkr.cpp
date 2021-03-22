#include "m_rc_gor_zkr.h"

#include "baseobjecttools.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(m_RC_Gor_ZKR,"РЦ ЗКР","MODEL ГОРКА")

REGISTERPROPERTY(m_RC_Gor_ZKR,RTDS_USL_OR,"Условия срабатывания РТДС","и / или","")



m_RC_Gor_ZKR::m_RC_Gor_ZKR(QObject *parent) : m_RC_Gor(parent)
{
    ris_nadvig=nullptr;
    controllerARSNadvig=nullptr;
    svetZKR=nullptr;
    FPUT_NADVIG=0;
    FSIGNAL_ROSPUSK.clear();
    FSIGNAL_STATE_ERROR_RTDS.clear();
    FSIGNAL_STATE_ERROR_NERASCEP.clear();
    FSIGNAL_STATE_ERROR_OSYCOUNT.clear();
    FRTDS_USL_OR=false;
    dso_osy=nullptr;
    memset(dso_vag,0,sizeof(dso_vag));
    memset(dso_db,0,sizeof(dso_db));
    resetStates();
}

void m_RC_Gor_ZKR::resetStates()
{
    m_RC_Gor::resetStates();
    FSTATE_ERROR_RTDS=false;
    FSTATE_ROSPUSK=false;
    FSTATE_ERROR_NERASCEP=false;
    FSTATE_ERROR_OSYCOUNT=false;
    FSTATE_OTCEP_UNKNOW=false;
    FSTATE_OTCEP_FREE=false;
    FSTATE_OTCEP_IN=false;
    FSTATE_OTCEP_VAGADD=false;
    FSTATE_OTCEP_BEVAGADD=false;
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

    ris_nadvig=qobject_cast<m_RIS*>(updateLink(FRIS_NADVIG));
    if (!ris_nadvig) {
        qCritical() << objectName() << "Ошибочная ссылка CONTR_NADVIG" <<endl ;
    }else{
        ris_nadvig->updateAfterLoad();
        controllerARSNadvig=ris_nadvig->controllerARS();
        addDevice(ris_nadvig);
    }

    svetZKR=qobject_cast<m_Svet_Gor_4N*>(updateLink(FSVET));
    if (!svetZKR) qCritical() << objectName() << "Ошибочная ссылка SVET" <<endl ; else addDevice(svetZKR);
    rtds1=qobject_cast<m_RTDS*>(updateLink(FRTDS1));
    if (!rtds1) qCritical() << objectName() << "Ошибочная ссылка RTDS1" <<endl ; else addDevice(rtds1);
    rtds2=qobject_cast<m_RTDS*>(updateLink(FRTDS2));
    if (!rtds2) qCritical() << objectName() << "Ошибочная ссылка RTDS2" <<endl ; else addDevice(rtds2);


    dso_osy=qobject_cast<m_DSO*>(updateLink(FDSO_OSY));
    dso_vag[0]=qobject_cast<m_DSO*>(updateLink(FDSO_VAG_0));
    dso_vag[1]=qobject_cast<m_DSO*>(updateLink(FDSO_VAG_1));
    dso_db[0]=qobject_cast<m_DSO*>(updateLink(FDSO_DB_0));
    dso_db[1]=qobject_cast<m_DSO*>(updateLink(FDSO_DB_1));

    if (!dso_osy) qCritical() << objectName() << QString("Ошибочная ссылка DSO_OSY") <<endl ; else addDevice(dso_osy);
    for (int i=0;i<2;i++){
            if (!dso_vag[i]) qCritical() << objectName() << QString("Ошибочная ссылка DSO_VAG_%1").arg(i) <<endl ; else addDevice(dso_vag[i]);
            if (!dso_db[i]) qCritical() << objectName() << QString("Ошибочная ссылка FDSO_DB_%1").arg(i) <<endl ; else addDevice(dso_db[i]);
    }
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
    setSignalState(FSIGNAL_STATE_OTCEP_UNKNOW,FSTATE_OTCEP_UNKNOW);
    setSignalState(FSIGNAL_STATE_OTCEP_FREE,FSTATE_OTCEP_FREE);
    setSignalState(FSIGNAL_STATE_OTCEP_IN,FSTATE_OTCEP_IN);
    setSignalState(FSIGNAL_STATE_OTCEP_VAGADD,FSTATE_OTCEP_VAGADD);
    setSignalState(FSIGNAL_STATE_OTCEP_BEVAGADD,FSTATE_OTCEP_BEVAGADD);

}

