#ifndef V_VES_H
#define V_VES_H

#include "v_model.h"

class m_Ves;

class v_Ves : public v_Model
{
    Q_OBJECT
public:
    MYPROP(QSizeF ,SIZE)
    MYPROP(int ,TEXT_HEIGHT)
    MYPROP(QFont,FONT)
    MYSTATE(quint64,STATE_OSY_COUNT)
    MYSTATE(qreal, STATE_OSY_SREDN_VES)
public:
    Q_INVOKABLE v_Ves(QObject *parent = nullptr);
    virtual ~v_Ves(){}
    virtual void resetStates();
    virtual void updateAfterLoad();
    virtual void calculateGeometry();

signals:

public slots:

protected:
    QRectF  R;
    QRectF  boundrect_text;
    m_Ves* _m_Ves;

    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *);

};

#endif // V_VES_H
