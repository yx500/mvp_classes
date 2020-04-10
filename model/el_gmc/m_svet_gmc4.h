#ifndef M_SVET_GMC4_H
#define M_SVET_GMC4_H

#include "m_svet_gmc.h"

class m_Svet_GMC4 : public m_Svet_GMC
{
    Q_OBJECT
public:

    Q_PROPERTY(SignalDescription SIGNAL_DBK_OPEN2       READ SIGNAL_DBK_OPEN2 DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_DBK_OPEN_BLINK2 READ SIGNAL_DBK_OPEN_BLINK2 DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_DBK_ERROR2      READ SIGNAL_DBK_ERROR2 DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_DBK_OPEN3       READ SIGNAL_DBK_OPEN3 DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_DBK_OPEN_BLINK3 READ SIGNAL_DBK_OPEN_BLINK3 DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_DBK_ERROR3      READ SIGNAL_DBK_ERROR3 DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_DBK_OPEN4       READ SIGNAL_DBK_OPEN4 DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_DBK_OPEN_BLINK4 READ SIGNAL_DBK_OPEN_BLINK4 DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_DBK_ERROR4      READ SIGNAL_DBK_ERROR4 DESIGNABLE true)

    MYSTATE(bool, STATE_DBK_OPEN2)
    MYSTATE(bool, STATE_DBK_OPENBLINK2)
    MYSTATE(bool, STATE_DBK_ERROR2)
    MYSTATE(bool, STATE_DBK_OPEN3)
    MYSTATE(bool, STATE_DBK_OPENBLINK3)
    MYSTATE(bool, STATE_DBK_ERROR3)
    MYSTATE(bool, STATE_DBK_OPEN4)
    MYSTATE(bool, STATE_DBK_OPENBLINK4)
    MYSTATE(bool, STATE_DBK_ERROR4)


    SignalDescription SIGNAL_DBK_OPEN2() const {return FSIGNAL_DBK_OPEN[2];}
    SignalDescription SIGNAL_DBK_OPEN_BLINK2() const {return FSIGNAL_DBK_OPEN_BLINK[2];}
    SignalDescription SIGNAL_DBK_ERROR2() const {return FSIGNAL_DBK_ERROR[2];}

    SignalDescription SIGNAL_DBK_OPEN3() const {return FSIGNAL_DBK_OPEN[3];}
    SignalDescription SIGNAL_DBK_OPEN_BLINK3() const {return FSIGNAL_DBK_OPEN_BLINK[3];}
    SignalDescription SIGNAL_DBK_ERROR3() const {return FSIGNAL_DBK_ERROR[3];}

    SignalDescription SIGNAL_DBK_OPEN4() const {return FSIGNAL_DBK_OPEN[4];}
    SignalDescription SIGNAL_DBK_OPEN_BLINK4() const {return FSIGNAL_DBK_OPEN_BLINK[4];}
    SignalDescription SIGNAL_DBK_ERROR4() const {return FSIGNAL_DBK_ERROR[4];}

public:

    Q_INVOKABLE m_Svet_GMC4(QObject *parent = 0);
    virtual ~m_Svet_GMC4(){}
    virtual void updateAfterLoad();
    virtual void updateStates();

protected:
    void updateSignals();
};

#endif // M_SVET_GMC4_H
