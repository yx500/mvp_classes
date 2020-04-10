#ifndef V_SVET_GOR_4N_H
#define V_SVET_GOR_4N_H

#include "v_svet.h"

class v_Svet_Gor_4N : public v_Svet
{
    Q_OBJECT

    MYSTATE(bool,STATE_K)
    MYSTATE(bool,STATE_J)
    MYSTATE(bool,STATE_Z)
    MYSTATE(bool,STATE_B)
    MYSTATE(bool,STATE_NAZAD)

public:
    Q_INVOKABLE v_Svet_Gor_4N(v_Base *parent = nullptr);
    virtual ~v_Svet_Gor_4N(){}
    virtual void resetStates();

    virtual void calculateGeometry(); // перестраивает внтренние координаты


signals:

public slots:

protected:
    QRectF NAZADR;
    QColor clrB[4];
    QRectF RSV4[4];
    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void updateStatesFromModel();

};

#endif // V_SVET_GOR_4N_H
