#include "m_rc_gor_park.h"

#include <QDebug>
#include "baseobjecttools.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(m_RC_Gor_Park,"РЦ Путь парка","MODEL ГОРКА")

REGISTERPROPERTY(m_RC_Gor_Park,KZP,"КЗП","Ссылка на модель КЗП","")
REGISTERPROPERTY(m_RC_Gor_Park,PARK_WAY,"ПУТЬ ПАРКА","Путь парка","")

m_RC_Gor_Park::m_RC_Gor_Park(QObject *parent) : m_RC_Gor(parent)
{
    _kzp=0;
    FKZP.clear();
    FPARK_WAY=0;
    resetStates();
}

void m_RC_Gor_Park::resetStates()
{
    m_RC_Gor::resetStates();
    FSTATE_KZP_D=0;
    FSTATE_KZP_E=0;
    FSTATE_KZP_V=0;
    FSTATE_KZP_P=0;

}

void m_RC_Gor_Park::validation(ListObjStr *l) const
{
    m_RC_Gor::validation(l);
    if ((!FKZP.isNotUse())&&(FKZP.isNull()))
        l->error(this,"Не задан КЗП");
    if (FMINWAY==0)
        l->warning(this,"Не задан путь");
    if (FPARK_WAY==0)
        l->warning(this,"Не задан путь парка");
    if ((FMINWAY!=0) && (FMINWAY!=FMAXWAY))
        l->warning(this,"MINWAY!=MAXWAY");
}

void m_RC_Gor_Park::updateAfterLoad()
{
    m_RC_Gor::updateAfterLoad();
    updateLink(FKZP);
    _kzp=qobject_cast<m_KZP*>(FKZP.baseObject());
    if (_kzp) addDevice(_kzp);
}

bool m_RC_Gor_Park::is33()
{
    if (m_RC_Gor::is33()) return true;
    return _kzp->is33();
}

void m_RC_Gor_Park::updateStates()
{
    m_RC_Gor::updateStates();
    if (_kzp) {
        setSTATE_KZP_D(_kzp->STATE_D());
        setSTATE_KZP_E(_kzp->STATE_E());
        setSTATE_KZP_V(_kzp->STATE_V());
        setSTATE_KZP_P(_kzp->STATE_P());
    } else {
        setSTATE_KZP_D(0);
        setSTATE_KZP_E(0);
        setSTATE_KZP_V(0);
        setSTATE_KZP_P(0);
    }
}

