#ifndef M_DSO_RD_21_H
#define M_DSO_RD_21_H

#include "m_dso.h"

class m_DSO_RD_21 : public m_DSO
{
    Q_OBJECT
public:



    MYPROP(SignalDescription,SIGNAL_B0)
    MYPROP(SignalDescription,SIGNAL_B1)
    MYPROP(SignalDescription,SIGNAL_K)

    MYSTATE(bool,STATE_B0)
    MYSTATE(bool,STATE_B1)
    MYSTATE(bool,STATE_K)

public:
    Q_INVOKABLE m_DSO_RD_21(QObject *parent = nullptr);
    virtual ~m_DSO_RD_21(){}
    virtual void resetStates();
    virtual QString defaultGroupName() const {return "ДСО";}
    virtual void validation(ListObjStr *l) const;

    virtual bool is33();
    virtual void updateStates();

protected:

};

#endif // M_DSO_RD_21_H
