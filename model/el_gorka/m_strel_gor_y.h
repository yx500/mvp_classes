#ifndef M_STREL_GOR_Y_H
#define M_STREL_GOR_Y_H

#include "m_strel_gor.h"

#include "m_ipd.h"
#include "m_rtds.h"


class m_Strel_Gor_Y : public m_Strel_Gor
{
    Q_OBJECT
public:
    MYPROP(SignalDescription, SIGNAL_PRP)
    MYPROP(SignalDescription, SIGNAL_PRM)
    MYPROP(SignalDescription, SIGNAL_A)

    MYPROP(SignalDescription, SIGNAL_UVK_PRP)
    MYPROP(SignalDescription, SIGNAL_UVK_PRM)
    MYPROP(SignalDescription, SIGNAL_UVK_AV)

    MYPROP(SignalDescription, SIGNAL_UVK_TLG)
    MYPROP(SignalDescription, SIGNAL_UVK_NGBSTAT_PL)
    MYPROP(SignalDescription, SIGNAL_UVK_NGBSTAT_MN)
    MYPROP(SignalDescription, SIGNAL_UVK_NGBDYN_PL)
    MYPROP(SignalDescription, SIGNAL_UVK_NGBDYN_MN)

    MYPROP(SignalDescription, SIGNAL_UVK_WSTRA)
    MYPROP(SignalDescription, SIGNAL_UVK_WSTRB)

    MYPROP(SignalDescription, SIGNAL_UVK_BL_PER)

    MYPROP(SignalDescription, TU_PRP)
    MYPROP(SignalDescription, TU_PRM)
    MYPROP(int, NEGAB_RC_CNT_P)
    MYPROP(int, NEGAB_RC_CNT_M)
    MYPROP(qreal, NEGAB_VGRAN_P)
    MYPROP(qreal, NEGAB_VGRAN_M)


    MYPROPLINK( IPD)
    MYPROPLINK( RTDS)

    MYSTATE(bool, STATE_PRP)
    MYSTATE(bool, STATE_PRM)
    MYSTATE(bool, STATE_A)
    MYSTATE(bool, STATE_NEGAB_RC)

    MYSTATE(bool, STATE_UVK_PRP)
    MYSTATE(bool, STATE_UVK_PRM)
    MYSTATE(bool, STATE_UVK_AV)

    MYSTATE(bool, STATE_UVK_TLG)
    MYSTATE(bool, STATE_UVK_NGBSTAT_PL)
    MYSTATE(bool, STATE_UVK_NGBSTAT_MN)
    MYSTATE(bool, STATE_UVK_NGBDYN_PL)
    MYSTATE(bool, STATE_UVK_NGBDYN_MN)

    MYSTATE(bool, STATE_UVK_WSTRA)
    MYSTATE(bool, STATE_UVK_WSTRB)

public:
    Q_INVOKABLE m_Strel_Gor_Y(QObject *parent = nullptr);
    virtual ~m_Strel_Gor_Y(){}
    virtual void validation(ListObjStr *l) const;
    virtual void updateAfterLoad();
    virtual void resetStates();
    virtual void updateStates();
    m_IPD *get_ipd()const{return  ipd;}
    m_RTDS *get_rtds()const{return  rtds;}
    QList<m_RC*> l_ngb_rc[2];

protected:
    m_IPD *ipd;
    m_RTDS *rtds;
};

#endif // M_STREL_GOR_Y_H
