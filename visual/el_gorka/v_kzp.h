#ifndef V_KZP_H
#define V_KZP_H

#include "v_model.h"

class v_KZP : public v_Model
{
    Q_OBJECT
public:
    MYPROP(QSizeF ,SIZE)
    MYPROP(int ,TEXT_HEIGHT)

    MYSTATE(int,STATE_D)
    MYSTATE(int,STATE_E)
    MYSTATE(qreal,STATE_V)
    MYSTATE(int,STATE_P)


    Q_INVOKABLE v_KZP(v_Base *parent = 0);
    virtual ~v_KZP(){}
    virtual void resetStates();
    virtual void calculateGeometry();


signals:

public slots:
protected:

    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void updateStatesFromModel();

    QRectF R;
    QRectF rect_text;
    QRectF rect_D;
    QRectF rect_V;

private:

};

#endif // V_KZP_H
