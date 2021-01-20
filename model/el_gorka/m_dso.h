#ifndef M_DSO_H
#define M_DSO_H

#include "m_base.h"
#include "m_rc.h"

// абстрактный класс для всех типов ДСО
#pragma pack(push, 1)
struct  DSO_Data{
    qint32 V;   // Значение
    qint8 D;   // посл. напр.
    qint8 E;   // сбой
    qint32 EV;   // кол-во сбоев
};
#pragma pack(pop)

class m_DSO : public m_Base
{
    Q_OBJECT
public:


    MYPROPLINK(RC)
    MYPROP(qreal,RC_OFFSET)

    MYPROP(SignalDescription,SIGNAL_DSODATA)

    MYPROPLINK(RC_NEXT)
    MYPROPLINK(RC_PREV)

    MYSTATE(qlonglong,STATE_OSY_COUNT)
    MYSTATE(int,STATE_DIRECT)
    MYSTATE(int,STATE_ERROR)
    MYSTATE(int,STATE_SRAB)
    MYSTATE(int,STATE_ERROR_TRACK)
    MYSTATE(int,STATE_ERROR_CNT)

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
