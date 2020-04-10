#ifndef V_TRAIN_NUMS_H
#define V_TRAIN_NUMS_H

#include <QObject>
#include "v_base.h"
#include "v_train_num.h"
#include "m_otceps.h"
class SignalChanel;

class v_Train_Nums : public v_Base
{
    Q_OBJECT
public:
    Q_INVOKABLE v_Train_Nums(v_Base *parent = 0);
    virtual ~v_Train_Nums(){}

    virtual void updateStates();

signals:

protected:
    m_Otceps *Otceps;
    QRectF R;
    virtual void calculateGeometry();
    virtual QRectF d_boundingRect() const {return R;}
    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    QList<v_Train_Num *> trainsVisible;
    void prepare();
    v_Train_Num *add_Train_Num(int number);

};

#endif // V_TRAIN_NUMS_H
