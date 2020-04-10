#ifndef V_TRAIN_NUM_H
#define V_TRAIN_NUM_H

#include "v_base.h"

class v_Train_Num : public v_Base
{
    Q_OBJECT
public:
    explicit v_Train_Num(v_Base *parent = 0);
    virtual ~v_Train_Num(){}
    virtual bool isStoredXML() const{return false;}
    int number;
    bool exists;
    int direct;
    QColor color;
    int width;
    int height;
    virtual void calculateGeometry(); // перестраивает внтренние координаты



signals:

public slots:
protected:
    QPointF M[1];
    QPolygonF P;
    QRectF R;
    QRectF textR;

    virtual QRectF d_boundingRect() const;
    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);

};

#endif // V_TRAIN_NUM_H
