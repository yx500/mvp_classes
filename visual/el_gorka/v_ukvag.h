#ifndef V_UKVAG_H
#define V_UKVAG_H

#include "v_model.h"
class m_UkVag;


class v_UkVag : public v_Model
{
    Q_OBJECT
public:
    MYPROP(QSizeF ,SIZE)
    MYPROP(int ,TEXT_HEIGHT)
    MYPROP(QFont,FONT)
public:
    Q_INVOKABLE v_UkVag(QObject *parent = nullptr);
    virtual ~v_UkVag(){}
    virtual void updateAfterLoad();
    virtual void calculateGeometry();

signals:
protected:
    QRectF  R;
    QRectF  boundrect_text;
    m_UkVag* _m_UkVag=nullptr;

    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *);


};

#endif // V_UKVAG_H
