#ifndef M_STREL_GOR_GMC_H
#define M_STREL_GOR_GMC_H

#include "m_strel_gor.h"

class m_Strel_Gor_GMC : public m_Strel_Gor
{
    Q_OBJECT
public:
    Q_PROPERTY(int DBK_LINE READ DBK_LINE WRITE setDBK_LINE DESIGNABLE true )
    Q_PROPERTY(int DBK_NUM READ DBK_NUM WRITE setDBK_NUM DESIGNABLE true )
    Q_PROPERTY(int DBK_OBJ READ DBK_OBJ WRITE setDBK_OBJ DESIGNABLE true )

    Q_PROPERTY(SignalDescription SIGNAL_ZAM READ SIGNAL_ZAM DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_BLOK READ SIGNAL_BLOK DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_A READ SIGNAL_A DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_PRP_ENABLE READ SIGNAL_PRP_ENABLE DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_PRM_ENABLE READ SIGNAL_PRM_ENABLE DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_PRP READ SIGNAL_PRP DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_PRM READ SIGNAL_ZAM DESIGNABLE true)

    Q_PROPERTY(SignalDescription SIGNAL_PKS READ SIGNAL_PKS DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_PRV READ SIGNAL_PRV DESIGNABLE true)


    MYPROP(ObjectLink, IPD)
    MYPROP(ObjectLink, RTDS)

    MYSTATE(bool, STATE_ZAM)
    MYSTATE(bool, STATE_BLOK)
    MYSTATE(bool, STATE_A)
    MYSTATE(bool, STATE_PRP_ENABLE)
    MYSTATE(bool, STATE_PRM_ENABLE)
    MYSTATE(bool, STATE_PRP)
    MYSTATE(bool, STATE_PRM)
    MYSTATE(bool, STATE_PKS)
    MYSTATE(bool, STATE_PRV)

    SignalDescription SIGNAL_ZAM() const {return FSIGNAL_ZAM;}
    SignalDescription SIGNAL_BLOK() const {return FSIGNAL_BLOK;}
    SignalDescription SIGNAL_A() const {return FSIGNAL_A;}
    SignalDescription SIGNAL_PRP_ENABLE() const {return FSIGNAL_PRP_ENABLE;}
    SignalDescription SIGNAL_PRM_ENABLE() const {return FSIGNAL_PRM_ENABLE;}
    SignalDescription SIGNAL_PRP() const {return FSIGNAL_PRP;}
    SignalDescription SIGNAL_PRM() const {return FSIGNAL_PRM;}

    SignalDescription SIGNAL_PKS() const {return FSIGNAL_PKS;}
    SignalDescription SIGNAL_PRV() const {return FSIGNAL_PRV;}

    int DBK_LINE()const{return FDBK_LINE;}
    int DBK_NUM()const{return FDBK_NUM;}
    int DBK_OBJ() const{return FDBK_OBJ;}
    void setDBK_LINE(int p);
    void setDBK_NUM(int p);
    void setDBK_OBJ(int p);

public:

    Q_INVOKABLE m_Strel_Gor_GMC(QObject *parent = 0);
    virtual ~m_Strel_Gor_GMC(){}
    virtual void validation(ListObjStr *l) const;
    virtual void updateAfterLoad();

    virtual bool is33();
    virtual void updateStates();

protected:
    void updateSignals();
    int FDBK_LINE;
    int FDBK_NUM;
    int FDBK_OBJ;
    SignalDescription FSIGNAL_ZAM;
    SignalDescription FSIGNAL_A;
    SignalDescription FSIGNAL_PRP_ENABLE;
    SignalDescription FSIGNAL_PRM_ENABLE;
    SignalDescription FSIGNAL_PRP;
    SignalDescription FSIGNAL_PRM;
    SignalDescription FSIGNAL_BLOK;
    SignalDescription FSIGNAL_PKS;
    SignalDescription FSIGNAL_PRV;
};

#endif // M_STREL_GOR_GMC_H
