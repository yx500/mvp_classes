#ifndef M_SVET_GOR_1_H
#define M_SVET_GOR_1_H

#include "m_svet.h"

class m_Svet_Gor_1 : public m_Svet
{
    Q_OBJECT
public:
    MYPROP(SignalDescription, SIGNAL_OPEN)
    MYPROP(SignalDescription, SIGNAL_OPEN_BLINK)
    MYSTATE(bool, STATE_OPEN_BLINK)

public:
    Q_INVOKABLE m_Svet_Gor_1(QObject *parent = 0);
    virtual ~m_Svet_Gor_1(){}
    virtual void resetStates();
    virtual void validation(ListObjStr *l) const;
    virtual bool is33();
    virtual void updateStates();
protected:
};

#endif // M_SVET_GOR_1_H
