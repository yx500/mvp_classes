#ifndef M_DSO_H
#define M_DSO_H

#include "m_base.h"
#include "m_rc.h"

// абстрактный класс для всех типов ДСО
#pragma pack(push, 1)
struct  DSO_Data{
    quint8 direct;
    qlonglong count;
};
#pragma pack(pop)
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
    Q_INVOKABLE m_DSO(QObject *parent = nullptr);
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
