#ifndef V_ARROW_H
#define V_ARROW_H

#include "v_base.h"

class v_Arrow : public v_Base
{
    Q_OBJECT
    MYPROPS(QString,POINTS)
    MYPROP(qreal,SH)
    MYPROP(qreal,ARR_LENGTH)
    MYPROP(qreal,ARR_WIDTH)
    MYPROP(bool,ARR_0)
    MYPROP(bool,ARR_1)
    MYPROP(QColor,PEN_COLOR)
    MYPROP(Qt::PenStyle,PEN_STYLE)
    MYPROP(QColor,BRUSH_COLOR)
    MYPROP(Qt::BrushStyle ,BRUSH_STYLE)
    MYPROP(bool,BRUSH_ANIMATE)
    MYPROP(QColor,BRUSH_ANIMATE_COLOR)
    MYPROP(int,BRUSH_ANIMATE_SPEED)

public:
    void setPOINTS(QString p);
public:

    Q_INVOKABLE v_Arrow(v_Base *parent = 0);
    virtual ~v_Arrow(){}


    virtual void calculateGeometry(); // перестраивает внтренние координаты
    virtual QPainterPath shape() const;
    virtual QMap<int, QPointF> getMarkersInfo() const;
    virtual void moveMarker(int markerId,QPointF xyItem);
    virtual void advance(int phase);


protected:

    QRectF  allign_rect;
    QRectF  boundrect_text;
    QPointF M[1];
    QList<QPointF> pointsOffset;
    QPolygonF basePolyLine;
    QPolygonF arrowPolygon;
    QString qreal2str(qreal r);
    qreal len_basePolyLine;
    qreal proclen,old_proclen;
    QTime animateT;


    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
private:

signals:

public slots:


};
#endif // V_ARROW_H
