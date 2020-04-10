#ifndef M_RTDS_H
#define M_RTDS_H

#include "m_base.h"


class m_RTDS : public m_Base
{
    Q_OBJECT
public:
    MYPROP(SignalDescription,SIGNAL_SRAB)
    MYSTATE(bool,STATE_SRAB)
public:
    Q_INVOKABLE m_RTDS(QObject *parent = 0);
    virtual ~m_RTDS(){}
    virtual void resetStates();
    virtual QString defaultGroupName() const {return "ИПД_РТДС";}
    virtual void validation(ListObjStr *l) const;
    virtual void updateAfterLoad();
    virtual bool is33();
    virtual void updateStates();
protected:
};

#endif // M_RTDS_H
