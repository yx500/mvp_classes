#ifndef V_OTCEPS_H
#define V_OTCEPS_H

#include "v_otcep.h"

class m_Otceps;

class v_Otceps : public v_Model
{
    Q_OBJECT
public:
    MYPROP(MVP_Enums::TDirect,  DIRECT_D0)
    MYPROP(QSizeF,  SIZE)
public:
    Q_INVOKABLE v_Otceps(QObject *parent = nullptr);
    virtual ~v_Otceps();
    virtual void setModelObject(BaseObject *B);
    virtual void updateAfterLoad();
    v_RC* m_RC2v_RCV(m_Base *m);

protected:
    m_Otceps* mOtceps;
    QList<v_Otcep *> lOtcep;
    QRectF R;
    virtual void calculateGeometry();
    virtual QRectF boundingRect() const {return R;}
    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void updateStates();
    virtual void doPropertyChanged();
    QMap<m_Base *,v_RC *> mm2v;
};

#endif // V_OTCEPS_H
