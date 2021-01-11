#ifndef M_RC_GOR_PARK_H
#define M_RC_GOR_PARK_H

#include "m_rc_gor.h"
#include "m_kzp.h"
class m_RC_Gor_Park : public m_RC_Gor
{
    Q_OBJECT
    MYPROPLINK( KZP)
    MYPROP(int, PARK_WAY)
    MYSTATE(int,STATE_KZP_D)
    MYSTATE(int,STATE_KZP_E)
    MYSTATE(qreal,STATE_KZP_V)
    MYSTATE(int,STATE_KZP_P)

public:
    Q_INVOKABLE m_RC_Gor_Park(QObject *parent = 0);
    virtual ~m_RC_Gor_Park(){}
    virtual void resetStates();
    virtual void validation(ListObjStr *l) const;
    virtual void updateAfterLoad();
    m_KZP * kzp()const {return _kzp;}

    virtual bool is33();
    virtual void updateStates();
protected:

    m_KZP * _kzp;
};

#endif // M_RC_GOR_PARK_H
