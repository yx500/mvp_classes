#ifndef M_DSO_H
#define M_DSO_H

#include "m_base.h"
#include "m_rc.h"

// абстрактный класс для всех типов ДСО

class m_DSO : public m_Base
{
    Q_OBJECT
public:


    MYPROP(ObjectLink,RC)
    MYPROP(qreal,RC_OFFSET)

    MYPROP(SignalDescription,SIGNAL_DSODATA)

    MYSTATE(qlonglong,STATE_OSY_COUNT)
    MYSTATE(int,STATE_DIRECT)
    MYSTATE(int,STATE_ERROR)
    MYSTATE(int,STATE_SRAB)

public:
    Q_INVOKABLE m_DSO(QObject *parent = 0);
    virtual ~m_DSO(){}
    virtual void resetStates();
    virtual QString defaultGroupName() const {return "ДСО";}
    virtual void validation(ListObjStr *l) const;
    virtual void updateAfterLoad();

    virtual void updateStates();
    virtual void reset();

protected:



};
#endif // M_DSO_H
