#ifndef M_VES_H
#define M_VES_H

#include "m_rc.h"

// должен был быть абстрактным классом для всех типов весомеров
// но сейчас используется тлько КВГ-15

class m_Ves : public m_Base
{
    Q_OBJECT
public:
    MYPROPLINK(RC)
    MYPROP(SignalDescription,SIGNAL_STATE_VES)

    MYSTATE(quint64,STATE_OSY_COUNT)
    MYSTATE(qreal, STATE_OSY_SREDN_VES)
public:
    Q_INVOKABLE m_Ves(QObject *parent = nullptr);
    virtual ~m_Ves(){}
    virtual void resetStates();
    virtual QString defaultGroupName() const {return "ДСО";}
    virtual void validation(ListObjStr *l) const;
    virtual void updateAfterLoad();

    virtual bool is33();
    virtual void updateStates();
protected:
    m_RC *rc;
};

#endif // M_VES_H
