#ifndef V_DBK_DIAG_H
#define V_DBK_DIAG_H

#include "v_stat_text.h"
#include "v_dbg_dbk_arrow.h"
#include "gmc_dbk.h"
class v_DBK_Diag : public v_StaticText
{
    Q_OBJECT
    MYPROP(int, DBK_ADDR)
    MYPROP(int, TO_SEC)
    MYPROP(SignalDescription,SIGNAL_STATUS1)
    MYPROP(SignalDescription,SIGNAL_STATUS2)
    MYPROP(SignalDescription,SIGNAL_STATUS3)
    MYPROP(SignalDescription,SIGNAL_STATUS4)

   MYSTATE(bool,STATE_33)

public:
    Q_INVOKABLE v_DBK_Diag(v_Base *parent = nullptr);
    virtual ~v_DBK_Diag(){}

    virtual void resetStates();
    virtual void updateAfterLoad();
    virtual void updateStates();

protected:
    QPointF M[1];
    QRectF rect;
    TDBK485PacketExchange DBK485PacketExchange;

    SignalDescription *SIGNAL_STATUS[4];
    quint64 _DatagramPacket2_tick[4];
    QDateTime packetRecivedTime;

    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);

};

#endif // V_DBK_DIAG_H
