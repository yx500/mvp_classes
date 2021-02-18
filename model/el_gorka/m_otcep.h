#ifndef M_OTCEP_H
#define M_OTCEP_H

#include "m_base.h"
#include "m_rc.h"
#include "tGorka.h"
#include <QVector>

class m_Otceps;


// описание отцепа
// живет всегда, не создется. Меняется только состояние.

enum TOtcepPart{
    _pOtcepPartUnknow=-1,
    _pOtcepStart=0,
    _pOtcepEnd=1,
    _pOtcepStartEnd,
    _pOtcepMiddle
};


enum {_normal_track=1,_broken_track=0};
enum {_forw=0,_back=1};
enum {_first=0,_last=1};


class m_Otcep : public m_Base
{
    Q_OBJECT
    Q_PROPERTY(int NUM READ NUM DESIGNABLE true STORED false )
    Q_PROPERTY(int STATE_NUM READ NUM DESIGNABLE true STORED false )
    Q_PROPERTY(int STATE_SP READ STATE_SP WRITE setSTATE_SP DESIGNABLE true STORED false )
    Q_PROPERTY(int STATE_SP_F READ STATE_SP_F WRITE setSTATE_SP_F DESIGNABLE true STORED false )
    MYPROP(SignalDescription,SIGNAL_DATA)
public:
    int NUM() const {return FNUM;}
    int STATE_SP() const;
    void setSTATE_SP(int p);
    int STATE_SP_F() const;
    void setSTATE_SP_F(int p);

    enum TOtcepLocation{
        locationUnknow=0, //
        locationOnPrib,  // до Выявления на ЗКР
        locationOnSpusk, // спускная часть, с момента выявления головы на ЗКР
        locationOnPark  // полностью в парке
    };
    Q_ENUM(TOtcepLocation)
    MYSTATE(int, STATE_ENABLED)  // участвует в роспуске
    MYSTATE_S(int, STATE_LOCATION)

    MYSTATE(quint32, STATE_ID_ROSP)
    MYSTATE(int, STATE_MAR)
    // основные слежение
    MYSTATE(int, STATE_MAR_F)
    MYSTATE(int, STATE_MAR_R)
    MYSTATE(int, STATE_DIRECTION)
    MYSTATE(int,STATE_NAGON)
    MYSTATE(int, STATE_ERROR)
    MYSTATE(int, STATE_ERROR_TRACK)
    MYSTATE(int, STATE_IS_CURRENT)

    MYSTATE(int, STATE_GAC_ACTIVE)
    MYSTATE(int, STATE_GAC_W_STRA)
    MYSTATE(int, STATE_GAC_W_STRB)
    MYSTATE(int, STATE_ARS_ACTIVE)

    // основные характеристики выбиираются из СЛ или ЗКР
    MYSTATE_R(int,  STATE_VAGON_CNT)
    MYSTATE_R(int,  STATE_OSY_CNT)
    MYSTATE_R(double,STATE_VES)     // Вес отцепа в тоннах/ось
    MYSTATE_R(int, STATE_BAZA)    // признак наличиия дб
    MYSTATE_R(double,  STATE_LEN)     // длина в метрах
    // CЛ
    MYSTATE(int,  STATE_SL_VAGON_CNT)
    MYSTATE(int,  STATE_SL_OSY_CNT)
    MYSTATE(double,STATE_SL_VES)
    MYSTATE(double,STATE_SL_LEN)
    MYSTATE(int, STATE_SL_BAZA)
    MYSTATE(int, STATE_SL_UR)
    MYSTATE(int, STATE_SL_OSO)
    MYSTATE(qreal, STATE_SL_STUPEN)
    // ЗКР
    MYSTATE(int, STATE_ZKR_PROGRESS)    // отцеп не выехал с ЗКР
    MYSTATE(int, STATE_ZKR_S_IN)       // голова на ЗКР
    MYSTATE(int,  STATE_ZKR_VAGON_CNT)
    MYSTATE(int,  STATE_ZKR_OSY_CNT)
    MYSTATE(double,STATE_ZKR_VES)
    MYSTATE(int, STATE_ZKR_BAZA)
    MYSTATE(int, STATE_ZKR_TLG)
    //MYSTATE(int, STATE_ZKR_KAT)
    MYSTATE(int, STATE_PUT_NADVIG)
    // KZP
    enum TOnParkState{
        kzpUnknow=0,
        kzpMoving,
        kzpStopped,
        kzpClosed
    };
    Q_ENUM(TOnParkState)
    MYSTATE(int, STATE_KZP_OS)
    MYSTATE(int, STATE_KZP_D)

    //динамика
    MYSTATE(qreal, STATE_V)
    MYSTATE(qreal, STATE_V_RC)
    MYSTATE(qreal, STATE_V_ARS)
    MYSTATE(qreal, STATE_V_KZP)
    MYSTATE(qreal, STATE_V_DISO)
    // смещение головы, хвоста в сторону direct [0..LEN]
    MYSTATE(qreal, STATE_D_RCS_XOFFSET)
    MYSTATE(qreal, STATE_D_RCF_XOFFSET)
    MYSTATE(int,   STATE_D_ORDER_RC)
    // журнал для протокола
    MYSTATE_RS(qreal, STATE_V_IN_1)
    MYSTATE_RS(qreal, STATE_V_IN_2)
    MYSTATE_RS(qreal, STATE_V_IN_3)
    MYSTATE_RS(qreal, STATE_V_OUT_1)
    MYSTATE_RS(qreal, STATE_V_OUT_2)
    MYSTATE_RS(qreal, STATE_V_OUT_3)
    void setSTATE_V_INOUT(int io,int n,qreal p);
    const qreal& STATE_V_INOUT(int io,int n)const {return FSTATE_V_INOUT[io][n];}
    MYSTATE_RS(int, STATE_ADDR_TP_1)
    MYSTATE_RS(int, STATE_ADDR_TP_2)
    MYSTATE_RS(int, STATE_ADDR_TP_3)
    void setSTATE_ADDR_TP(int n,int p);
    const int& STATE_ADDR_TP(int n)const {return FSTATE_ADDR_TP[n];}

    MYSTATE_RS(int, STATE_OT_1)     //0- растарможка 1-4 ступени максимал ступень работы замедлителя
    MYSTATE_RS(int, STATE_OT_2)
    MYSTATE_RS(int, STATE_OT_3)
    MYSTATE_RS(int, STATE_RA_1)    // 0-автомат режим ручного вмешательсва
    MYSTATE_RS(int, STATE_RA_2)
    MYSTATE_RS(int, STATE_RA_3)
    void setSTATE_OT_RA(int ot_ra,int n,int p);
    const int& STATE_OT_RA(int ot_ra,int n)const {return FSTATE_OT_RA[ot_ra][n];}

    // АРС
    MYSTATE_RS(qreal, STATE_V_ZAD_1)
    MYSTATE_RS(qreal, STATE_V_ZAD_2)
    MYSTATE_RS(qreal, STATE_V_ZAD_3)

    void setSTATE_V_ZAD(int n,qreal p);
    const qreal& STATE_V_ZAD(int n)const {return FSTATE_V_ZAD[n];}
    MYSTATE(qreal, STATE_STUPEN)



    MYSTATE(int, STATE_TICK)




    void states2descr_ext(t_NewDescr &D) const;
    void descr_ext2states(const t_NewDescr &D);

public:
    explicit m_Otcep(m_Otceps *parent,int num);
    virtual ~m_Otcep(){}
    virtual void resetStates();
    virtual void updateAfterLoad();
    virtual QString defaultGroupName() const {return "ОТЦЕПЫ";}
    virtual bool isStoredXML() const{return false;}
    void acceptSLStates(const m_Otcep *o);

    QVector<tSlVagon> vVag;
    QList<m_RC *> vBusyRc;


    m_RC * RCS=nullptr;
    m_RC * RCF=nullptr;
    m_RC * descr_RCS=nullptr;
    m_RC * descr_RCF=nullptr;

    qreal FSTATE_V_INOUT[2][3];
    int FSTATE_OT_RA[2][3];
    int FSTATE_ADDR_TP[3];
    qreal FSTATE_V_ZAD[3];


    void setBusyRC();

    virtual bool is33();
    virtual void updateStates();
    void update_descr();

public slots:
    void slotChanged(){updateStates();}

protected:
    int FNUM;
    t_NewDescr stored_Descr;

    m_Otceps*otceps;
    void updateStates_0();
//    void updateStates_1();
    QByteArray _storedA;

};


#endif // M_OTCEP_H
