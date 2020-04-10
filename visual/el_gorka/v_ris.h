#ifndef V_RIS_H
#define V_RIS_H

#include "v_model.h"
class m_RIS;

class v_RIS : public v_Model
{
    Q_OBJECT
public:
    MYPROP(QSizeF ,SIZE)
    MYPROP(QPointF ,TEXT_XY)
    MYPROP(QSizeF ,SIZE_TEXT)

    MYSTATE(qreal,STATE_V)
    MYSTATE(bool,STATE_ERRCS)
public:
    Q_INVOKABLE v_RIS(v_Base *parent = nullptr);
    virtual ~v_RIS(){}
    virtual void resetStates();

    virtual void calculateGeometry();

signals:

public slots:

protected:
    QRectF R;
    QRectF rect_text,bound_rect_text;
    QPointF T;
    m_RIS *ris;

    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void updateStatesFromModel();

};

#endif // V_RIS_H
