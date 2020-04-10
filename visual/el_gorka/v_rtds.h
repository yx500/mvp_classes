#ifndef V_RTDS_H
#define V_RTDS_H

#include "v_model.h"

class v_RTDS : public v_Model
{
    Q_OBJECT
    Q_PROPERTY(int     SHRC READ SHRC WRITE setSHRC DESIGNABLE true)
public:
    MYPROP(bool,CLASSIC_VIEW)

    MYPROP(QSizeF ,SIZE)
    MYPROP(qreal ,LENGTH)
    MYPROP(QPointF ,TEXT_XY)

    MYSTATE(bool,STATE_SRAB)
    void setSHRC(int p);
    int SHRC(){return FSHRC;}
public:
    Q_INVOKABLE v_RTDS(v_Base *parent = 0);
    virtual ~v_RTDS(){}
    virtual void resetStates();

    virtual void calculateGeometry();


signals:

public slots:

protected:
    int FSHRC;
    qreal S;
    QRectF RC;
    QRectF rect_text,bound_rect_text;
    QPointF M[1];
    QPointF T[2];

    QRectF RR[2];
    QPolygonF TR[2];

    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void updateStatesFromModel();

private:


};

#endif // V_RTDS_H
