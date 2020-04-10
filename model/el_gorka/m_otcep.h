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
    Q_PROPERTY(int STATE_SP READ STATE_SP DESIGNABLE true STORED false )
    MYPROP(SignalDescription,SIGNAL_ADDR)
public:
    MYSTATE(bool, STATE_ENABLED)  // участвует в роспуске
    int NUM() const {return FNUM;}
    int STATE_SP() const;

    enum TOtcepLocation{
        locationUnknow=0, //
        locationOnPrib,  // до Выявления на ЗКР
        locationOnSpusk, // спускная часть, с момента выявления головы на ЗКР
        locationOnPark  // полностью в парке
    };
    Q_ENUM(TOtcepLocation)
    MYSTATES(TOtcepLocation, STATE_LOCATION)

    MYSTATE(int, STATE_MAR)
    MYSTATE(int, STATE_MAR_F)
    MYSTATE(int, STATE_DIRECTION)
    MYSTATE(bool,STATE_NAGON)
    MYSTATE(int, STATE_ERROR)
    MYSTATE(int, STATE_ID_ROSP)

    // основные характеристики
    MYSTATE(int,  STATE_VAGON_CNT)
    MYSTATE(int,  STATE_OSY_CNT)
    MYSTATE(qreal,STATE_VES)     // Вес отцепа в тоннах
    MYSTATE(bool, STATE_BAZA)    // признак наличиия дб
    MYSTATE(int,  STATE_LEN)     // длина в метрах
    // CЛ
    MYSTATE(int,  STATE_SL_VAGON_CNT)
    MYSTATE(int,  STATE_SL_OSY_CNT)
    MYSTATE(qreal,STATE_SL_VES)
    MYSTATE(bool, STATE_SL_BAZA)
    // ЗКР
    MYSTATE(bool, STATE_ZKR_PROGRESS)    // отцеп не выехал с ЗКР
    MYSTATE(int,  STATE_ZKR_VAGON_CNT)
    MYSTATE(int,  STATE_ZKR_OSY_CNT)
    MYSTATE(bool, STATE_ZKR_BAZA)
    // KZP
    enum TOnParkState{
        kzpUnknow=0,
        kzpMoving,
        kzpStopped,
        kzpClosed
    };
    Q_ENUM(TOnParkState)
    MYSTATE(TOnParkState, STATE_KZP_OS)
    // вычисляемые
    MYSTATE(qreal, STATE_LEN_BY_VAGON)
    MYSTATE(qreal, STATE_LEN_BY_RC_MIN)
    MYSTATE(qreal, STATE_LEN_BY_RC_MAX)
    //динамика
    MYSTATE(qreal, STATE_V)
    MYSTATE(qreal, STATE_V_RC)
    MYSTATE(qreal, STATE_V_ARS)
    MYSTATE(qreal, STATE_V_KZP)
    MYSTATE(qreal, STATE_V_DISO)
    // смещение головы, хвоста в сторону direct [0..LEN]
    MYSTATE(qreal, STATE_RCS_XOFFSET)
    MYSTATE(qreal, STATE_RCF_XOFFSET)
    // журнал для протокола
    MYSTATE(qreal, STATE_V_IN_1)
    MYSTATE(qreal, STATE_V_IN_2)
    MYSTATE(qreal, STATE_V_IN_3)
    MYSTATE(qreal, STATE_V_OUT_1)
    MYSTATE(qreal, STATE_V_OUT_2)
    MYSTATE(qreal, STATE_V_OUT_3)

    MYSTATE(int, STATE_CHANGE_COUNTER)
    MYSTATE(QString,IDS_RC_BUSY)





public:
    explicit m_Otcep(m_Otceps *parent,int num);
    virtual ~m_Otcep(){}
    virtual void resetStates();
    virtual void updateAfterLoad();
    virtual QString defaultGroupName() const {return "ОТЦЕПЫ";}
    virtual bool isStoredXML() const{return false;}

    QVector<tSlVagon> vVag;
    QList<m_RC *> vBusyRc;

    t_Descr  stored_Descr;

    m_RC * RCS=nullptr;
    m_RC * RCF=nullptr;
    m_RC * descr_RCS=nullptr;
    m_RC * descr_RCF=nullptr;


    void setLenByVagon();
    void setBusyRC();

    virtual bool is33();
    virtual void updateStates();

    tos_OtcepData *tos=nullptr; // доп данные для work, внешняя либа.
public slots:
    void slotChanged(){updateStates();}

protected:
    int FNUM;
    m_Otceps*otceps;

};

#endif // M_OTCEP_H
