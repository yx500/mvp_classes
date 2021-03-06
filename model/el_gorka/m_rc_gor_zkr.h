#ifndef M_RC_GOR_ZKR_H
#define M_RC_GOR_ZKR_H

#include "m_rc_gor.h"
#include "m_ris.h"
#include "m_rc_gor.h"
#include "m_svet_gor_4n.h"
#include "m_rtds.h"
#include "m_dso.h"

// абстрактный класс ЗКР


class m_RC_Gor_ZKR : public m_RC_Gor
{
    Q_OBJECT
public:


    MYPROPLINK( SVET)
    MYPROPLINK( RIS_NADVIG)
    MYPROPLINK( RTDS1)
    MYPROPLINK( RTDS2)

//    MYPROPLINK( DSO11)
//    MYPROPLINK( DSO12)
//    MYPROPLINK( DSO21)
//    MYPROPLINK( DSO22)

    MYPROPLINK( DSO_OSY)
    MYPROPLINK( DSO_VAG_0)
    MYPROPLINK( DSO_VAG_1)
    MYPROPLINK( DSO_DB_0)
    MYPROPLINK( DSO_DB_1)


    MYPROP(int,PUT_NADVIG)
    MYPROP(SignalDescription,SIGNAL_ROSPUSK)

    MYPROP(SignalDescription,SIGNAL_STATE_ERROR_RTDS)
    MYPROP(SignalDescription,SIGNAL_STATE_ERROR_NERASCEP)
    MYPROP(SignalDescription,SIGNAL_STATE_ERROR_OSYCOUNT)
    MYPROP(SignalDescription,SIGNAL_STATE_OTCEP_UNKNOW)
    MYPROP(SignalDescription,SIGNAL_STATE_OTCEP_FREE)
    MYPROP(SignalDescription,SIGNAL_STATE_OTCEP_IN)
    MYPROP(SignalDescription,SIGNAL_STATE_OTCEP_VAGADD)
    MYPROP(SignalDescription,SIGNAL_STATE_OTCEP_BEVAGADD)


    MYPROP(bool,RTDS_USL_OR)


    MYSTATE(bool, STATE_ROSPUSK)

    MYSTATE(bool, STATE_ERROR_RTDS)
    MYSTATE(bool, STATE_ERROR_NERASCEP)
    MYSTATE(bool, STATE_ERROR_OSYCOUNT)
    MYSTATE(bool, STATE_OTCEP_UNKNOW)
    MYSTATE(bool, STATE_OTCEP_FREE)
    MYSTATE(bool, STATE_OTCEP_IN)
    MYSTATE(bool, STATE_OTCEP_VAGADD)
    MYSTATE(bool, STATE_OTCEP_BEVAGADD)


public:
    Q_INVOKABLE m_RC_Gor_ZKR(QObject *parent = nullptr);
    virtual ~m_RC_Gor_ZKR(){}
    virtual void resetStates();
    virtual void validation(ListObjStr *l) const;
    virtual void updateAfterLoad();
    m_ControllerARS *ControllerARSNadvig() const {return controllerARSNadvig;}
    m_Svet_Gor_4N *svet() const {return svetZKR;}
    m_RTDS *rtds_1() const {return rtds1;}
    m_RTDS *rtds_2() const {return rtds2;}

    m_DSO *dso_osy;
    m_DSO *dso_vag[2];
    m_DSO *dso_db[2];


    virtual bool is33();
    virtual void updateStates();
protected:
    m_ControllerARS *controllerARSNadvig;
    m_RIS * ris_nadvig;
    m_Svet_Gor_4N *svetZKR;
    m_RTDS *rtds1;
    m_RTDS *rtds2;
};

#endif // M_RC_GOR_ZKR_H
