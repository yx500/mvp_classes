#ifndef V_SVET_GOR_1_H
#define V_SVET_GOR_1_H

#include "v_svet.h"

class v_Svet_Gor_1 : public v_Svet
{
    Q_OBJECT

    MYPROP(v_Svet::TColorOpen, COLOR_OPEN)
    MYPROP(v_Svet::TColorClose, COLOR_CLOSE)
    MYSTATE(bool, STATE_OPEN)
    MYSTATE(bool, STATE_OPEN_BLINK)

public:
    Q_INVOKABLE v_Svet_Gor_1(v_Base *parent = 0);
    virtual ~v_Svet_Gor_1(){}
    virtual void resetStates();

signals:

public slots:

protected:
    QColor colorOpen(bool blink);
    QColor colorClose() ;
    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void updateStatesFromModel();

};

#endif // V_SVET_GOR_1_H
