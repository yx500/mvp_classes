#ifndef M_STREL_DSO_DBK_H
#define M_STREL_DSO_DBK_H

#include "m_strel.h"
#include "m_rc_dso.h"
#include "dbk_description.h"


// нумерация - первая цифра расположение по маршруту 0..1,для стрелок добавляются минус -2
// верх/низ 0/1 (U/D для свойства)
// дополнительный ля ПСВ - D

class m_STREL_DSO_DBK: public m_Strel
{
    Q_OBJECT
    Q_ENUMS(TDSOCNTTYPE)
    Q_PROPERTY(ObjectLink D0U READ D0U WRITE setD0U DESIGNABLE true )
    Q_PROPERTY(ObjectLink D0D READ D0D WRITE setD0D DESIGNABLE true )
    Q_PROPERTY(ObjectLink D1U READ D1U WRITE setD1U DESIGNABLE true )
    Q_PROPERTY(ObjectLink D1D READ D1D WRITE setD1D DESIGNABLE true )
    Q_PROPERTY(ObjectLink D2U READ D2U WRITE setD2U DESIGNABLE true )
    Q_PROPERTY(ObjectLink D2D READ D2D WRITE setD2D DESIGNABLE true )
    Q_PROPERTY(ObjectLink D0U2 READ D0U2 WRITE setD0U2 DESIGNABLE true )
    Q_PROPERTY(ObjectLink D0D2 READ D0D2 WRITE setD0D2 DESIGNABLE true )


public:
    enum TDSOCNTTYPE{dso6=6,dso8=8};
    enum TMAXDSOCNT{maxdsocnt=dso8};
    enum TMAXDSO2CNT{maxdso2cnt=maxdsocnt/2};

    MYPROP(SignalDescription, SIGNAL_SRED_POL)
    MYPROP(SignalDescription, SIGNAL_PLUS_PEREVOD)
    MYPROP(SignalDescription, SIGNAL_MINUS_PEREVOD)
    MYPROP(SignalDescription, SIGNAL_SRED_PEREVOD)

    MYPROP(TDSOCNTTYPE, DSOCNT)
    MYPROP(SignalDescription, SIGNAL_DSO_STATE)
    MYPROP(SignalDescription, SIGNAL_DSO_OSY_COUNT)
    MYPROP(DBK_Description, DBK_TU)

    MYSTATE(int,  STATE_DSO_OSY_COUNT)
    MYSTATE(bool, STATE_DSO_BUSY)
    MYSTATE(bool, STATE_DSO_ERROR)
    MYSTATE(bool, STATE_DSO_DIRECT)
    MYSTATE(bool, STATE_DSO_ERR_D1)
    MYSTATE(bool, STATE_DSO_ERR_D2)
    MYSTATE(bool, STATE_DSO_ERR_D3)
    MYSTATE(bool, STATE_DSO_ERR_D4)

    MYSTATE(m_DSO2::TState_DSO2,DSO2_0)
    MYSTATE(m_DSO2::TState_DSO2,DSO2_1)
    MYSTATE(m_DSO2::TState_DSO2,DSO2_2)
    MYSTATE(m_DSO2::TState_DSO2,DSO2_3)

    void setD0U(ObjectLink p){ setDSO(p,0,0);}
    void setD0D(ObjectLink p){ setDSO(p,0,1);}
    void setD1U(ObjectLink p){ setDSO(p,1,0);}
    void setD1D(ObjectLink p){ setDSO(p,1,1);}
    void setD2U(ObjectLink p){ setDSO(p,2,0);}
    void setD2D(ObjectLink p){ setDSO(p,2,1);}
    void setD0U2(ObjectLink p){setDSO(p,3,0);}
    void setD0D2(ObjectLink p){setDSO(p,3,1);}
    ObjectLink D0U(){return  flnkdso[0][0];}
    ObjectLink D0D(){return  flnkdso[0][1];}
    ObjectLink D1U(){return  flnkdso[1][0];}
    ObjectLink D1D(){return  flnkdso[1][1];}
    ObjectLink D2U(){return  flnkdso[2][0];}
    ObjectLink D2D(){return  flnkdso[2][1];}
    ObjectLink D0U2(){return flnkdso[3][0];}
    ObjectLink D0D2(){return flnkdso[3][1];}


public:

    Q_INVOKABLE m_STREL_DSO_DBK(QObject *parent = 0);
    virtual ~m_STREL_DSO_DBK();

    virtual void validation(ListObjStr *l) const;
    virtual void updateAfterLoad();
    virtual int workLevel() const {return -50;} // первее стрелок, но позже m_DSO
    virtual void setName(const QString &n);
    m_DSO *dso(int n,int m){return fDSO[n][m];}

public slots:
    virtual void updateStates();
    virtual void work();
protected:
    ObjectLink flnkdso[maxdsocnt/2][2];
    m_DSO * fDSO[maxdsocnt/2][2];
    m_DSO2 * fDSO2[maxdsocnt/2];

    void setSTATE_DSO_ERR_D(int i,bool p=true);
    void setDSO(ObjectLink p,int n,int m);
    virtual bool is_33() const;
private:
    void work6();
    void work8();
};

Q_DECLARE_METATYPE(m_STREL_DSO_DBK::TDSOCNTTYPE)

#endif // M_STREL_DSO_DBK_H
