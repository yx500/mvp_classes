#ifndef M_ZAM_H
#define M_ZAM_H

#include "m_base.h"
#include "m_rc.h"
#include "objectlink.h"
#include "m_controllerars.h"
#include "m_ris.h"

class m_Zam : public m_Base
{
    Q_OBJECT
    MYPROP(int,NTP)
    MYPROP(int,TIPZM)
    MYPROP(int,TIPSTUPEN)
    MYPROP(SignalDescription,SIGNAL_T1)
    MYPROP(SignalDescription,SIGNAL_T2)
    MYPROP(SignalDescription,SIGNAL_T3)
    MYPROP(SignalDescription,SIGNAL_T4)
    MYPROP(SignalDescription,SIGNAL_R)
    MYPROP(SignalDescription,SIGNAL_A)
    MYPROP(SignalDescription,SIGNAL_KZ)
    MYPROP(SignalDescription,SIGNAL_OG)
    MYPROP(SignalDescription,SIGNAL_RRC)

    MYPROP(ObjectLink, CONTR_ARS)
    MYPROP(ObjectLink, RIS)
    MYPROP(ObjectLink, RC)


    MYSTATE(bool,STATE_T1)
    MYSTATE(bool,STATE_T2)
    MYSTATE(bool,STATE_T3)
    MYSTATE(bool,STATE_T4)
    MYSTATE(bool,STATE_R)
    MYSTATE(bool,STATE_A)
    MYSTATE(bool,STATE_KZ)
    MYSTATE(bool,STATE_OG)
    MYSTATE(bool,STATE_RRC)
    MYSTATE(int,STATE_STUPEN)
    MYSTATE(bool,STATE_MK_RRC)
    MYSTATE(bool,STATE_MK_33)

public:
    enum TStupen :int {st_unknow=-2,st_none=0,st_R=-1,st_T1=1,st_T2=2,st_T3=3,st_T4=4};
    Q_ENUM(TStupen)




public:
    Q_INVOKABLE m_Zam(QObject *parent = 0);
    virtual ~m_Zam(){}
    virtual QString defaultGroupName() const {return "ЗАМ";}
    virtual void validation(ListObjStr *l) const;
    virtual void updateAfterLoad();
    m_ControllerARS *controllerARS() const {return _controllerARS;}
    m_RIS *ris() const {return _ris;}
    m_RC * rc() const {return _rc;}
    virtual bool is33();
    virtual void updateStates();
protected:
    m_RIS *_ris;
    m_RC * _rc;
    m_ControllerARS *_controllerARS;
};
//Q_DECLARE_METATYPE(m_Zam::TStupen)

#endif // M_ZAM_H
