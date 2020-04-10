#ifndef M_SVET_GOR_4N_H
#define M_SVET_GOR_4N_H

#include "m_svet.h"

class m_Svet_Gor_4N : public m_Svet
{
    Q_OBJECT
public:
    MYPROP(SignalDescription,SIGNAL_K)
    MYPROP(SignalDescription,SIGNAL_J)
    MYPROP(SignalDescription,SIGNAL_Z)
    MYPROP(SignalDescription,SIGNAL_B)
    MYPROP(SignalDescription,SIGNAL_NAZAD)

    MYSTATE(bool,STATE_K)
    MYSTATE(bool,STATE_J)
    MYSTATE(bool,STATE_Z)
    MYSTATE(bool,STATE_B)
    MYSTATE(bool,STATE_NAZAD)

public:
    Q_INVOKABLE m_Svet_Gor_4N(QObject *parent = 0);
    virtual ~m_Svet_Gor_4N(){}
    virtual void validation(ListObjStr *l) const;
    virtual bool is33();
    virtual void updateStates();
protected:
};

#endif // M_SVET_GOR_4N_H
