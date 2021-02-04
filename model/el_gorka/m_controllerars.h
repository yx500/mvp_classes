#ifndef M_CONTROLLERARS_H
#define M_CONTROLLERARS_H

#include <QDateTime>
#include "m_base.h"
#include "tGorka.h"

// для удобства
struct  t_ARSDescrCellsBiostime {
    QDateTime t;
    uint32     biostime;
    t_ARSDescrCell cell;
    qreal V() const;
};

class m_ControllerARS : public m_Base
{
    Q_OBJECT
    Q_PROPERTY(int ADDR_SLOT READ ADDR_SLOT WRITE setADDR_SLOT DESIGNABLE true )
public:
    MYPROP(SignalDescription,SIGNAL_ADDR)
    MYPROP(int,ADDR)
    MYSTATE(bool,STATE_ADDR)
    MYSTATE(qreal,STATE_V)  // уже пересчитанная
    MYSTATE(qreal,STATE_VOLEG)
    MYSTATE(bool,STATE_T1)
    MYSTATE(bool,STATE_T2)
    MYSTATE(bool,STATE_T3)
    MYSTATE(bool,STATE_T4)
    MYSTATE(bool,STATE_R)
    MYSTATE(bool,STATE_RC)
    MYSTATE(bool,STATE_RRC)
    MYSTATE(bool,STATE_ERRCS)
    MYSTATE(bool,STATE_RC_PRED)
    MYSTATE(bool,STATE_UFO)
    int ADDR_SLOT(){return FADDR_SLOT;}
    void setADDR_SLOT(int p);

public:
    Q_INVOKABLE m_ControllerARS(QObject *parent = nullptr);
    virtual ~m_ControllerARS(){}
    virtual void resetStates();
    virtual QString defaultGroupName() const {return "АРС";}
    virtual void validation(ListObjStr *l) const;
    virtual void updateAfterLoad();
    static qreal TPVal(quint16 w);
    bool isValidState() const;
    virtual bool is33();
    virtual void updateStates();

protected:
    int FADDR_SLOT;
};

#endif // M_CONTROLLERARS_H
