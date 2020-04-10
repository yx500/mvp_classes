#ifndef M_KZP_H
#define M_KZP_H

#include <qdatetime.h>
#include "m_base.h"

class m_KZP : public m_Base
{
    Q_OBJECT
public:
    MYPROP(SignalDescription,SIGNAL_ADDR)

    MYSTATE(int,STATE_D)
    MYSTATE(int,STATE_E)
    MYSTATE(qreal,STATE_V)
    MYSTATE(int,STATE_P)
public:
    struct Ttd{
        QDateTime t;
        int d;
        Ttd(){t=QDateTime();d=0;}
        Ttd(QDateTime _t,int _d){t=_t;d=_d;}
    };


    Q_INVOKABLE m_KZP(QObject *parent = nullptr);
    virtual ~m_KZP(){}
    virtual void resetStates();
    virtual QString defaultGroupName() const {return "КЗП";}
    virtual void validation(ListObjStr *l) const;
    virtual void updateAfterLoad();
    virtual void update_V(QDateTime T);

    virtual bool is33();
    virtual void updateStates();
//    virtual void work(const QDateTime &T);
protected:
    QList<Ttd> l_td;
};

#endif // M_KZP_H
