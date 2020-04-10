#include "v_train_num.h"
#include "qpfunction.h"

v_Train_Num::v_Train_Num(v_Base *parent) : v_Base(parent)
{
    setZValue(30);
}

void v_Train_Num::calculateGeometry()
{
    P.clear();
    qreal h2=height/2;
    qreal xx=h2;
    P << M[0]
      << QPointF(M[0].x()+xx,M[0].y()-h2)
      << QPointF(M[0].x()+width,M[0].y()-h2)
      << QPointF(M[0].x()+width,M[0].y()+h2)
      << QPointF(M[0].x()+xx,M[0].y()+h2);

    textR=QRectF(0,0,width-xx,height);
    textR.moveTopLeft(QPointF(M[0].x()+xx,M[0].y()-h2));

    if (direct==1){
        QPFunction::reflectPLx(&P,M[0].x());
        textR.moveTopLeft(QPointF(M[0].x()-width,M[0].y()-h2));
    }


    R=P.boundingRect();
}

QRectF v_Train_Num::d_boundingRect() const
{
    return R;
}

void v_Train_Num::d_paint(QPainter *painter, const QStyleOptionGraphicsItem *)
{
    QBrush B=QBrush(color);
    QPen   p=QPen(Qt::black);
    painter->setPen(p);painter->setBrush(B);
    painter->drawPolygon(P);
    QFont font("Times", height-2);
    painter->setFont(font);
    painter->drawText(textR, Qt::AlignCenter,QString("%1").arg(number));
}
