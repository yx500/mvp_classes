#ifndef M_RC_DSO_H
#define M_RC_DSO_H

#include "m_rc.h"
#include "m_dso.h"
#include "m_dso2.h"

// нумерация - первая цифра расположение по маршруту 0..1,для стрелок добавляются минус -2
// верх/низ 0/1 (U/D для свойства)
// дополнительный ля ПСВ - D

class m_RC_DSO : public m_RC
{
    Q_OBJECT

    Q_PROPERTY(ObjectLink D0U READ D0U WRITE setD0U DESIGNABLE true )
    Q_PROPERTY(ObjectLink D0D READ D0D WRITE setD0D DESIGNABLE true )
    Q_PROPERTY(ObjectLink D1U READ D1U WRITE setD1U DESIGNABLE true )
    Q_PROPERTY(ObjectLink D1D READ D1D WRITE setD1D DESIGNABLE true )
    enum TDSO2CNTT{_dso2_cnt=2};
public:
    enum TDSO_STATE_Mask{
        dso_state_busy=0x01,
        dso_state_error=0x02,
        dso_state_direct=0x04,
        dso_state_err1=0x08,
        dso_state_err2=0x10,
        dso_state_err3=0x20,
        dso_state_err4=0x40
    };

    MYPROP(SignalDescription, SIGNAL_DSO_STATE)
    MYPROP(SignalDescription, SIGNAL_DSO_OSY_COUNT)
    MYSTATE(int,  STATE_DSO_OSY_COUNT)
    MYSTATE(bool, STATE_DSO_BUSY)
    MYSTATE(bool, STATE_DSO_ERROR)
    MYSTATE(bool, STATE_DSO_DIRECT)
    MYSTATE(bool, STATE_DSO_ERR_D1)
    MYSTATE(bool, STATE_DSO_ERR_D2)

    MYSTATE(m_DSO2::TState_DSO2,DSO2_0)
    MYSTATE(m_DSO2::TState_DSO2,DSO2_1)


    void setD0U(ObjectLink p){setDSO(p,0,0);}
    void setD0D(ObjectLink p){setDSO(p,0,1);}
    void setD1U(ObjectLink p){setDSO(p,1,0);}
    void setD1D(ObjectLink p){setDSO(p,1,1);}
    ObjectLink D0U(){return flnkdso[0][0];}
    ObjectLink D0D(){return flnkdso[0][1];}
    ObjectLink D1U(){return flnkdso[1][0];}
    ObjectLink D1D(){return flnkdso[1][1];}



public:
    Q_INVOKABLE m_RC_DSO(QObject *parent = 0);
    virtual ~m_RC_DSO();

    virtual void validation(ListObjStr *l) const;
    virtual void updateAfterLoad();
    virtual int workLevel() const {return -50;} // первее стрелок, но позже m_DSO
    virtual void setName(const QString &n);

    m_DSO *dso(int n,int m){return fDSO[n][m];}
public slots:
    virtual void updateStates();
    virtual void work();
protected:
    ObjectLink flnkdso[_dso2_cnt][2];
    m_DSO * fDSO[_dso2_cnt][2];
    m_DSO2 * fDSO2[_dso2_cnt];

    virtual bool is_33() const;
    void setSTATE_DSO_ERR_D(int i,bool p=true);
    void setDSO(ObjectLink p,int n,int m);
private:


};


#endif // M_RC_DSO_H
