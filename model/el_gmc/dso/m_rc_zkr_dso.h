#ifndef M_RC_ZKR_DSO_H
#define M_RC_ZKR_DSO_H

#include "m_rc_gor.h"
#include "m_dso.h"
#include "m_dso2.h"



class m_RC_ZKR_DSO : public m_RC_Gor
{
    Q_OBJECT
    Q_PROPERTY(ObjectLink D0U READ D0U WRITE setD0U DESIGNABLE true )
    Q_PROPERTY(ObjectLink D0D READ D0D WRITE setD0D DESIGNABLE true )
    Q_PROPERTY(ObjectLink D1U READ D1U WRITE setD1U DESIGNABLE true )
    Q_PROPERTY(ObjectLink D1D READ D1D WRITE setD1D DESIGNABLE true )
    enum TDSO2CNTT{_dso2_cnt=2};
public:

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
    Q_INVOKABLE m_RC_ZKR_DSO(QObject *parent = 0);
    virtual ~m_RC_ZKR_DSO();

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
};

#endif // M_RC_ZKR_DSO_H
