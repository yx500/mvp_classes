#ifndef M_RIS_H
#define M_RIS_H

#include "m_base.h"
#include "m_rc.h"
#include "m_controllerars.h"

class m_RIS : public m_Base
{
    Q_OBJECT
    MYPROPLINK( CONTR_ARS)
    MYPROPLINK( RC)
    MYPROP(SignalDescription,SIGNAL_RRC)

    MYSTATE(bool,STATE_RRC)
    MYSTATE(qreal,STATE_V)
public:
    Q_INVOKABLE m_RIS(QObject *parent = nullptr);
    virtual ~m_RIS(){}
    virtual void resetStates();
    virtual QString defaultGroupName() const {return "РИС";}
    virtual void validation(ListObjStr *l) const;
    virtual void updateAfterLoad();
    m_ControllerARS *controllerARS() const {return _controllerARS;}
    quint16 dd;
    virtual bool is33();
    virtual void updateStates();
protected:
    m_ControllerARS *_controllerARS;
    m_RC * rc;
};

#endif // M_RIS_H
