#include "m_ris.h"

#include "baseobjecttools.h"
#include "mvp_objectfactory.h"

REGISTERELEMENT(m_RIS,"РИС-В3М","MODEL ГОРКА")

m_RIS::m_RIS(QObject *parent) : m_Base(parent),
   FSIGNAL_RRC()
{
    _controllerARS=0;
    rc=0;
    resetStates();
}

void m_RIS::resetStates()
{
    FSTATE_V=0;
    FSTATE_RRC=0;
}

void m_RIS::validation(ListObjStr *l) const
{
    m_Base::validation(l);
    if ((!FSIGNAL_RRC.isNotUse()) &&(FSIGNAL_RRC.isEmpty()))
        l->warning(this,"Не задан сигнал РРС");
    if (FCONTR_ARS.isNull())
         l->error(this,"Не задан контроллер АРС");
    if (FRC.isNull())
        l->error(this,"Не задана РЦ");
}

void m_RIS::updateAfterLoad()
{
    m_Base::updateAfterLoad();
    _controllerARS=qobject_cast<m_ControllerARS*>(reLink(this,FCONTR_ARS));
    if (!_controllerARS)
        qCritical() << objectName() << "Ошибочная ссылка CONTR_ARS" <<endl ;
    rc=qobject_cast<m_RC*>(reLink(this,FRC));
    if (!rc)
        qCritical() << objectName() << "Ошибочная ссылка RC" <<endl ; else
        rc->addDevice(this);

    //setidstr(QString("#РИС_%1").arg(objectName()));
}

bool m_RIS::is33()
{
    if (m_Base::is33()) return true;
    if (FSIGNAL_RRC.is33()) return true;
    if (_controllerARS) return _controllerARS->is33();
    return false;
}

void m_RIS::updateStates()
{
    m_Base::updateStates();
    if (FSTATE_33) return;
    setSTATE_RRC(FSIGNAL_RRC.value_1bit());
    if (_controllerARS) setSTATE_V(_controllerARS->STATE_V()); else setSTATE_V(0);
}
