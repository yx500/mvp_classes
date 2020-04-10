#ifndef V_STAT_RECT_H
#define V_STAT_RECT_H

#include "v_base.h"

class v_Stat_Rect : public v_Base
{
    Q_OBJECT
    MYPROP(QSizeF,SIZE)
    MYPROP(QColor,PEN_COLOR)
    MYPROP(Qt::PenStyle,PEN_STYLE)
    MYPROP(QColor,BRUSH_COLOR)
    MYPROP(Qt::BrushStyle ,BRUSH_STYLE)
    MYPROP(QSizeF,ROUND_SIZE)
public:
public:
    Q_INVOKABLE v_Stat_Rect(v_Base *parent = 0);
    virtual ~v_Stat_Rect(){}

    virtual void calculateGeometry(); // перестраивает внтренние координаты


signals:

public slots:
protected:
    QPointF M[1];
    QRectF rect;

    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
};

Q_DECLARE_METATYPE(QBrush)

#endif // V_STAT_RECT_H
