#ifndef M_OTCEP_H
#define M_OTCEP_H

#include "m_base.h"
#include "m_rc.h"
#include "tGorka.h"
#include <QVector>

class m_Otceps;
class tos_OtcepData;


// описание отцепа
// живет всегда, не создется. Меняется только состояние.

enum TOtcepPart{
    _pOtcepPartUnknow=0,
    _pOtcepStart,
    _pOtcepEnd,
    _pOtcepStartEnd,
    _pOtcepMiddle
};


class m_Otcep : public m_Base
{
    Q_OBJECT
    Q_PROPERTY(int NUM READ NUM DESIGNABLE true STORED false )
    Q_PROPERTY(int STATE_NUM READ NUM DESIGNABLE true STORED false )
    Q_PROPERTY(int STATE_SP READ STATE_SP WRITE setSTATE_SP DESIGNABLE true STORED false )
    MYPROP(SignalDescription,SIGNAL_DATA)
public:
    MYSTATE(bool, STATE_ENABLED)  // участвует в роспуске
    int NUM() const {return FNUM;}
    int STATE_SP() const;
    void setSTATE_SP(int p);

    enum TOtcepLocation{
        locationUnknow=0, //
        locationOnPrib,  // до Выявления на ЗКР
        locationOnSpusk, // спускная часть, с момента выявления головы на ЗКР
        locationOnPark  // полностью в парке
    };
    Q_ENUM(TOtcepLocation)
    MYSTATE_S(TOtcepLocation, STATE_LOCATION)

    MYSTATE(quint32, STATE_ID_ROSP)
    MYSTATE(int, STATE_MAR)
    // основные слежение
    MYSTATE(int, STATE_MAR_F)
    MYSTATE(QString,IDS_RCS)
    MYSTATE(QString,IDS_RCF)
    MYSTATE(int, STATE_DIRECTION)
    MYSTATE(bool,STATE_NAGON)
    MYSTATE(int, STATE_ERROR)


    MYSTATE(int, STATE_ID_ROSP_VAG)
    MYSTATE(int, STATE_SP_VAG)

    // основные характеристики выбиираются из СЛ или ЗКР
    MYSTATE_R(int,  STATE_VAGON_CNT)
    MYSTATE_R(int,  STATE_OSY_CNT)
    MYSTATE_R(qreal,STATE_VES)     // Вес отцепа в тоннах/ось
    MYSTATE_R(bool, STATE_BAZA)    // признак наличиия дб
    MYSTATE_R(qreal,  STATE_LEN)     // длина в метрах
    // CЛ
    MYSTATE(int,  STATE_SL_VAGON_CNT)
    MYSTATE(int,  STATE_SL_OSY_CNT)
    MYSTATE(qreal,STATE_SL_VES)
    MYSTATE(qreal,STATE_SL_LEN)
    MYSTATE(bool, STATE_SL_BAZA)
    MYSTATE(int, STATE_SL_UR)
    MYSTATE(int, STATE_SL_OSO)
    // ЗКР
    MYSTATE(bool, STATE_ZKR_PROGRESS)    // отцеп не выехал с ЗКР
    MYSTATE(int,  STATE_ZKR_VAGON_CNT)
    MYSTATE(int,  STATE_ZKR_OSY_CNT)
    MYSTATE(qreal,STATE_ZKR_VES)
    MYSTATE(bool, STATE_ZKR_BAZA)
    MYSTATE(bool, STATE_ZKR_KAT)
    MYSTATE(int, STATE_PUT_NADVIG)
    // KZP
    enum TOnParkState{
        kzpUnknow=0,
        kzpMoving,
        kzpStopped,
        kzpClosed
    };
    Q_ENUM(TOnParkState)
    MYSTATE(TOnParkState, STATE_KZP_OS)
    MYSTATE(int,          STATE_KZP_D)

    //динамика
    MYSTATE(qreal, STATE_V)
    MYSTATE(qreal, STATE_V_RC)
    MYSTATE(qreal, STATE_V_ARS)
    MYSTATE(qreal, STATE_V_KZP)
    MYSTATE(qreal, STATE_V_DISO)
    // смещение головы, хвоста в сторону direct [0..LEN]
    MYSTATE(qreal, STATE_D_RCS_XOFFSET)
    MYSTATE(qreal, STATE_D_RCF_XOFFSET)
    // журнал для протокола
    MYSTATE_RS(qreal, STATE_V_IN_1)
    MYSTATE_RS(qreal, STATE_V_IN_2)
    MYSTATE_RS(qreal, STATE_V_IN_3)
    MYSTATE_RS(qreal, STATE_V_OUT_1)
    MYSTATE_RS(qreal, STATE_V_OUT_2)
    MYSTATE_RS(qreal, STATE_V_OUT_3)
    void setSTATE_V_INOUT(int io,int n,qreal p);
    const qreal& STATE_V_INOUT(int io,int n)const {return FSTATE_V_INOUT[io][n];}
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
    MYSTATE(qreal, STATE_SL_STUPEN)


    MYSTATE(int, STATE_CHANGE_COUNTER)






public:
    explicit m_Otcep(m_Otceps *parent,int num);
    virtual ~m_Otcep(){}
    virtual void resetStates();
    virtual void updateAfterLoad();
    virtual QString defaultGroupName() const {return "ОТЦЕПЫ";}
    virtual bool isStoredXML() const{return false;}
    void acceptSLStates(m_Otcep *o);

    QVector<tSlVagon> vVag;
    QList<m_RC *> vBusyRc;

    t_Descr  stored_Descr;

    m_RC * RCS=nullptr;
    m_RC * RCF=nullptr;
    m_RC * descr_RCS=nullptr;
    m_RC * descr_RCF=nullptr;


    void setBusyRC();

    virtual bool is33();
    virtual void updateStates();
    void update_descr();

    tos_OtcepData *tos=nullptr; // доп данные для work, внешняя либа.
public slots:
    void slotChanged(){updateStates();}

protected:
    int FNUM;
    qreal FSTATE_V_INOUT[2][3];
    int FSTATE_OT_RA[2][3];
    qreal FSTATE_V_ZAD[3];
    m_Otceps*otceps;
    void updateStates_0();
    void updateStates_1();
    QByteArray _storedA;

};


#endif // M_OTCEP_H
