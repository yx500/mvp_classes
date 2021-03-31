#ifndef M_UKVAG_H
#define M_UKVAG_H

#include "m_base.h"
#include "m_rc.h"

class m_UkVag : public m_Base
{
   Q_OBJECT
public:
    MYPROP(SignalDescription,SIGNAL_DATA)
    MYPROP(SignalDescription,SIGNAL_SET)
    MYSTATE(int,STATE_VAL1)
    MYSTATE(int,STATE_VAL2)
    MYSTATE(int,STATE_VAL3)


    Q_INVOKABLE m_UkVag(QObject *parent = nullptr);
    virtual ~m_UkVag(){}
    virtual void resetStates();
    virtual QString defaultGroupName() const {return "СВТ";}

    virtual void updateStates();
protected:
    quint8 value[3];
};

#endif // M_UKVAG_H
