#ifndef V_CONTROLLERARS_H
#define V_CONTROLLERARS_H

#include "v_model.h"

class v_ControllerARS : public v_Model
{
    Q_OBJECT
public:
    MYPROP(QSizeF ,SIZE)
    MYPROP(int ,TEXT_HEIGHT)

    MYSTATE(bool,STATE_ADDR)
    MYSTATE(qreal,STATE_V)  // уже пересчитанная
    MYSTATE(qreal,STATE_VOLEG)
    MYSTATE(bool,STATE_T1)
    MYSTATE(bool,STATE_T2)
    MYSTATE(bool,STATE_T3)
    MYSTATE(bool,STATE_T4)
    MYSTATE(bool,STATE_R)
    MYSTATE(bool,STATE_RC)
    MYSTATE(bool,STATE_RRC)
    MYSTATE(bool,STATE_ERRCS)
    MYSTATE(bool,STATE_RC_PRED)

    Q_INVOKABLE v_ControllerARS(v_Base *parent = 0);
    virtual ~v_ControllerARS(){}
    virtual void resetStates();
    virtual void calculateGeometry();


signals:

public slots:
protected:
    QRectF R;
    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void updateStatesFromModel();

    QRectF rect_text;
    QRectF rect_T[5];
    QRectF rect_V;
    QRectF rect_VOLEG;
    QRectF rect_RC;
    QRectF rect_RC_PRED;
    QRectF rect_RRC;
    QRectF rect_ERRCS;
    QRectF rect_RRC_B;
    QRectF rect_ERRCS_B;


private:

};

#endif // V_CONTROLLERARS_H
