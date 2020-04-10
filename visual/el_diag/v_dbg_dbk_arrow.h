#ifndef V_DBG_DBK_ARROW_H
#define V_DBG_DBK_ARROW_H

#include "v_chanelsostarrow.h"


class v_dbg_DBK_Arrow : public v_Arrow
{
    Q_OBJECT
    Q_PROPERTY(int  COMPLECTNUMBER READ COMPLECTNUMBER WRITE setCOMPLECTNUMBER DESIGNABLE true)

public:
    MYPROP(int, DBK_LINE)
    MYPROP(int, DBK_ADDR)
    MYPROP(int, TO_SEC)
    MYPROP(QColor,COLOR_33)
    MYPROP(bool,DEFCOLORS)

    MYPROP(SignalDescription,SIGNAL_STATUS)

    MYSTATE(bool,STATE_33)
    MYSTATE(int,STATE_COMPLECTNUMBER)
    MYSTATE(int,STATE_DBKLINE)
    MYSTATE(int,STATE_MOXA485ERROR)
    MYSTATE(int,STATE_ONLINE)

    Q_ENUMS(TDBKInOut)
    Q_ENUMS(TGMCComplect)
    Q_ENUMS(TGMCInComplect)


    int  COMPLECTNUMBER() const {return FCOMPLECTNUMBER;}
    void  setCOMPLECTNUMBER(int p);

public:
    Q_INVOKABLE v_dbg_DBK_Arrow(v_Base *parent = nullptr);
    virtual ~v_dbg_DBK_Arrow(){}

    virtual void resetStates();
    virtual void updateAfterLoad();

    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void updateStates();
    virtual void calculateGeometry(); // перестраивает внтренние координаты

protected:
    quint64 _DatagramPacket2_tick;
    int  FCOMPLECTNUMBER;
    QDateTime packetRecivedTime;
};
#endif // V_DBG_DBK_ARROW_H
