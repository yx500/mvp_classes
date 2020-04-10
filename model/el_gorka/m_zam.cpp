#include "m_zam.h"

#include "baseobjecttools.h"

#include "mvp_objectfactory.h"

#include "m_rc_gor_zam.h"

REGISTERELEMENT(m_Zam,"Замедлитель","MODEL ГОРКА")

m_Zam::m_Zam(QObject *parent) : m_Base(parent),
  FSIGNAL_T1(),
  FSIGNAL_T2(),
  FSIGNAL_T3(),
  FSIGNAL_T4(),
  FSIGNAL_R(),
  FSIGNAL_A(),
  FSIGNAL_KZ(),
  FSIGNAL_OG()

{
    _ris=nullptr;
    _rc=nullptr;
    _controllerARS=nullptr;
    FSTATE_A=false;
    FSTATE_KZ=false;
    FSTATE_OG=false;FSTATE_R=false;FSTATE_RRC=false;
    FSTATE_T1=false;FSTATE_T2=false;FSTATE_T3=false;FSTATE_T4=false;
    FSTATE_STUPEN=st_R;
    FSTATE_MK_RRC=false;
    FSTATE_MK_33=false;
//    FSIGNAL_A.setIsNoUse(true);
//    FSIGNAL_KZ.setIsNoUse(true);
//    FSIGNAL_OG.setIsNoUse(true);
//    FSIGNAL_T1.setIsNoUse(true);
//    FSIGNAL_T2.setIsNoUse(true);
//    FSIGNAL_T3.setIsNoUse(true);
//    FSIGNAL_T4.setIsNoUse(true);
//    FSIGNAL_R.setIsNoUse(true);
//    FSIGNAL_RRC.setIsNoUse(true);

}

void m_Zam::validation(ListObjStr *l) const
{
    m_Base::validation(l);
    if ((FNTP<=0) ||(FNTP>3))
        l->error(this,"Не задан НТП");
    if ((FTIPZM<=0) ||(FTIPZM>3))
        l->error(this,"Не задан  ТИП ЗМ");
    if ((!FSIGNAL_T1.isNotUse()) &&(FSIGNAL_T1.isEmpty()))
        l->warning(this,"Не задан сигнал Т1");
    if ((!FSIGNAL_T2.isNotUse()) &&(FSIGNAL_T2.isEmpty()))
        l->warning(this,"Не задан сигнал Т2");
    if ((!FSIGNAL_T3.isNotUse()) &&(FSIGNAL_T3.isEmpty()))
        l->warning(this,"Не задан сигнал Т3");
    if ((!FSIGNAL_T4.isNotUse()) &&(FSIGNAL_T4.isEmpty()))
        l->warning(this,"Не задан сигнал Т4");
    if ((!FSIGNAL_R.isNotUse()) &&(FSIGNAL_R.isEmpty()))
        l->warning(this,"Не задан сигнал Р");
    if ((!FSIGNAL_A.isNotUse()) &&(FSIGNAL_A.isEmpty()))
        l->warning(this,"Не задан сигнал А");
    if ((!FSIGNAL_KZ.isNotUse()) &&(FSIGNAL_KZ.isEmpty()))
        l->warning(this,"Не задан сигнал КЗ");
    if ((!FSIGNAL_OG.isNotUse()) &&(FSIGNAL_OG.isEmpty()))
        l->warning(this,"Не задан сигнал ОГ");
    if ((!FSIGNAL_RRC.isNotUse()) &&(FSIGNAL_RRC.isEmpty()))
        l->warning(this,"Не задан сигнал РРС");
    if (FRIS.isNull())
         l->error(this,"Не задан скоростемер");
    if (FRC.isNull())
         l->error(this,"Не задана РЦ");
    if ((_ris!=nullptr)&&(_ris->controllerARS()!=_controllerARS))
        l->error(this,"Не совпадают контроллеры с РИС");

}

void m_Zam::updateAfterLoad()
{
    m_Base::updateAfterLoad();
    _controllerARS=qobject_cast<m_ControllerARS*>(reLink(this,FCONTR_ARS));
    if (!_controllerARS)
        qCritical() << objectName() << "Ошибочная ссылка CONTR_ARS" <<endl ;
    _rc=qobject_cast<m_RC*>(reLink(this,FRC));
    _ris=qobject_cast<m_RIS*>(reLink(this,FRIS));
    if (!_ris)
        qCritical() << objectName() << "Ошибочная ссылка РИС" <<endl ;
    _rc=qobject_cast<m_RC*>(reLink(this,FRC));
    if (!_rc)
        qCritical() << objectName() << "Ошибочная ссылка RC" <<endl ; else {
        _rc->addDevice(this);
        _rc->addDevice(_ris);
        _rc->addDevice(_controllerARS);
    }
}

bool m_Zam::is33()
{
    if (m_Base::is33()) return true;
    return FSIGNAL_T1.is33() |
            FSIGNAL_A.is33() | FSIGNAL_RRC.is33();
}


void m_Zam::updateStates()
{
    m_Base::updateStates();
    if (FSTATE_33) return;

    setSTATE_T1(FSIGNAL_T1.value_1bit());
    setSTATE_T2(FSIGNAL_T2.value_1bit());
    setSTATE_T3(FSIGNAL_T3.value_1bit());
    setSTATE_T4(FSIGNAL_T4.value_1bit());
    setSTATE_R(FSIGNAL_R.value_1bit());
    setSTATE_A(FSIGNAL_A.value_1bit());
    setSTATE_KZ(FSIGNAL_KZ.value_1bit());
    setSTATE_OG(FSIGNAL_OG.value_1bit());
    setSTATE_RRC(FSIGNAL_RRC.value_1bit());
    TStupen st=st_unknow;
    if ((!FSTATE_T1) &&(!FSTATE_T2) &&(!FSTATE_T3) &&(!FSTATE_T4) &&(!FSTATE_R)) st=st_none;
    if ((!FSTATE_T1) &&(!FSTATE_T2) &&(!FSTATE_T3) &&(!FSTATE_T4) &&( FSTATE_R)) st=st_R;
    if (( FSTATE_T1) &&(!FSTATE_T2) &&(!FSTATE_T3) &&(!FSTATE_T4) &&(!FSTATE_R)) st=st_T1;
    if ((!FSTATE_T1) &&( FSTATE_T2) &&(!FSTATE_T3) &&(!FSTATE_T4) &&(!FSTATE_R)) st=st_T2;
    if ((!FSTATE_T1) &&(!FSTATE_T2) &&( FSTATE_T3) &&(!FSTATE_T4) &&(!FSTATE_R)) st=st_T3;
    if ((!FSTATE_T1) &&(!FSTATE_T2) &&(!FSTATE_T3) &&( FSTATE_T4) &&(!FSTATE_R)) st=st_T4;

    if (_controllerARS) setSTATE_MK_33(_controllerARS->STATE_33()); else setSTATE_MK_33(false);
    if (_controllerARS) setSTATE_MK_RRC(_controllerARS->STATE_RRC()); else setSTATE_MK_RRC(false);

//    st=st_R;
//    setSTATE_RRC(1);

    setSTATE_STUPEN(st);
}
