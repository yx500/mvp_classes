#ifndef V_BUTTON1X_H
#define V_BUTTON1X_H

#include "v_lamp1x.h"

class v_Button1x : public v_Lamp1x
{
    Q_OBJECT
    MYSTATE(bool,SDOWN)

public:
    Q_INVOKABLE v_Button1x(v_Base *parent = nullptr);
    virtual ~v_Button1x(){}

    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *style);

signals:

public slots:
};

#endif // V_BUTTON1X_H
