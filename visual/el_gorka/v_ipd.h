#ifndef V_IPD_H
#define V_IPD_H

#include "v_rtds.h"

class v_IPD : public v_RTDS
{
    Q_OBJECT
public:
    Q_INVOKABLE v_IPD(v_Base *parent = 0);
    virtual ~v_IPD(){}

    virtual void calculateGeometry();

signals:

public slots:
protected:
    QPointF M[1];

    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
private:
};

#endif // V_IPD_H
