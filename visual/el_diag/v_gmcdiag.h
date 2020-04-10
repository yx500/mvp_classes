#ifndef V_GMCDIAG_H
#define V_GMCDIAG_H

#include "v_stat_text.h"

class v_GMCDiag : public v_StaticText
{
    Q_OBJECT
    Q_ENUMS(TGMCIndicator)
    Q_PROPERTY(TGMCIndicator  INDICATOR READ INDICATOR WRITE setINDICATOR DESIGNABLE true)
    Q_PROPERTY(int  COMPLECTNUMBER READ COMPLECTNUMBER WRITE setCOMPLECTNUMBER DESIGNABLE true)

    MYPROP(SignalDescription,SIGNAL_STATUS)
    MYPROP(int, TO_SEC)
    MYSTATE(bool,STATE_33)
    MYSTATE(int,STATE_REGIM)
    MYSTATE(int,STATE_RELE_VKL)
    MYSTATE(int,STATE_RELE_VIBOR)

public:
    enum TGMCIndicator{mainSost,wdSost,lineSost};
    TGMCIndicator  INDICATOR() const {return FINDICATOR;}
    void setINDICATOR(TGMCIndicator p){FINDICATOR=p;}

    int  COMPLECTNUMBER() const {return FCOMPLECTNUMBER;}
    void  setCOMPLECTNUMBER(int p);

public:
    DECLARECOLOR(gmc_mainSost_wait_start)
    DECLARECOLOR(gmc_mainSost_live)
    DECLARECOLOR(gmc_mainSost_error)
    DECLARECOLOR(gmc_wdSost_ok)
    DECLARECOLOR(gmc_wdSost_error)
    DECLARECOLOR(gmc_lineSost_ok)
    DECLARECOLOR(gmc_lineSost_error)

    Q_INVOKABLE v_GMCDiag(v_Base *parent = nullptr);
    virtual ~v_GMCDiag(){}

    virtual void resetStates();
    virtual void updateAfterLoad();
    virtual void updateStates();
protected:
    QPointF M[1];
    QRectF rect;
    TGMCIndicator  FINDICATOR ;
    quint64 _DatagramPacket2_tick;
    QTime packetRecivedTime;
    int  FCOMPLECTNUMBER;

    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);

};

Q_DECLARE_METATYPE(v_GMCDiag::TGMCIndicator)

#endif // V_GMCDIAG_H
