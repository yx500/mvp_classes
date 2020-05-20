#ifndef M_RC_GOR_ZKR_H
#define M_RC_GOR_ZKR_H

#include "m_rc_gor.h"
#include "m_controllerars.h"
#include "m_rc_gor.h"
#include "m_svet_gor_4n.h"
#include "m_rtds.h"
#include "m_dso.h"

// абстрактный класс ЗКР


class m_RC_Gor_ZKR : public m_RC_Gor
{
    Q_OBJECT
public:


    MYPROP(ObjectLink, SVET)
    MYPROP(ObjectLink, CONTR_NADVIG)
    MYPROP(ObjectLink, RTDS1)
    MYPROP(ObjectLink, RTDS2)

    MYPROP(ObjectLink, DSO11)
    MYPROP(ObjectLink, DSO12)
    MYPROP(ObjectLink, DSO21)
    MYPROP(ObjectLink, DSO22)

    MYPROP(int,PUT_NADVIG)
    MYPROP(SignalDescription,SIGNAL_ROSPUSK)

    MYPROP(SignalDescription,SIGNAL_STATE_ERROR_RTDS)
    MYPROP(SignalDescription,SIGNAL_STATE_ERROR_NERASCEP)
    MYPROP(SignalDescription,SIGNAL_STATE_ERROR_OSYCOUNT)


    MYPROP(bool,RTDS_USL_OR)


    MYSTATE(bool, STATE_ROSPUSK)

    MYSTATE(bool, STATE_ERROR_RTDS)
    MYSTATE(bool, STATE_ERROR_NERASCEP)
    MYSTATE(bool, STATE_ERROR_OSYCOUNT)


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

    m_DSO *dso[2][2];

    virtual bool is33();
    virtual void updateStates();
protected:
    m_ControllerARS *controllerARSNadvig;
    m_Svet_Gor_4N *svetZKR;
    m_RTDS *rtds1;
    m_RTDS *rtds2;
};

#endif // M_RC_GOR_ZKR_H
