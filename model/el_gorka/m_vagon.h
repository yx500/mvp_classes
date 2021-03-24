#ifndef M_VAGON_H
#define M_VAGON_H

#include "tGorka.h"
#include "m_base.h"
class m_Otceps;

class m_Vagon  : public m_Base
{
    Q_OBJECT
    Q_PROPERTY(int NUM READ NUM DESIGNABLE true STORED false )
    MYPROP(SignalDescription,SIGNAL_DATA)
public:
    int NUM() const {return FNUM;}
    MYSTATE(int, STATE_ENABLED)  // участвует в роспуске
    MYSTATE(int, STATE_LOCATION)
    MYSTATE(int, STATE_ZKR_PROGRESS)    // вагон не выехал с ЗКР

    MYSTATE(quint32, STATE_ID_ROSPUSK)  // идентификатор роспуска
    MYSTATE(int, STATE_NUM_OTCEP)
    MYSTATE(int, STATE_N_IN_OTCEP)
    MYSTATE(int, STATE_IV)   // Номер вагона по НСЛ
    MYSTATE(int, STATE_SP)
    MYSTATE(quint32, STATE_NUMV) // Номер вагона физический
    MYSTATE(int, STATE_MASSG)   // масса груза
    MYSTATE(int, STATE_MASSV)   // масса
    MYSTATE(int, STATE_LN)   // длина в мм
    MYSTATE(int, STATE_OSY)   // оси
    MYSTATE(int, STATE_ROD)     // род вагон
    MYSTATE(int, STATE_DB)  // длинобаз
    MYSTATE(int, STATE_NG)   // негабарит
    MYSTATE(int, STATE_KTP)   // код услов типа вагона
    MYSTATE(int, STATE_OSO) // ОСО
    MYSTATE(int, STATE_UR)
    MYSTATE(int, STATE_TICK)    //  счет изменений

    tSlVagon toSlVagon() const;
    void fromSlVagon(tSlVagon &slv);
    void assign(const m_Vagon *v);

    explicit m_Vagon(m_Otceps *parent,int num);
    m_Vagon();
    m_Vagon(const m_Vagon& v);
    m_Vagon& operator = (const m_Vagon& v){assign(&v);FNUM=NUM();return *this;}
    virtual ~m_Vagon(){}
    virtual void resetStates();
    virtual void updateAfterLoad();
    void resetTrackingStates();
    void resetZKRStates();

    virtual bool is33();
    virtual void updateStates();
    void inc_tick(){FSTATE_TICK++;}
public slots:
    void slotChanged(){updateStates();}
protected:
    int FNUM=0;
    tSlVagon stored_SlVagon;
    m_Otceps*otceps=nullptr;


};

QVariantHash tSlVagon2Map(const tSlVagon &v);
//tSlVagon Map2tSlVagon(const QVariantHash &m);

#endif // M_VAGON_H
