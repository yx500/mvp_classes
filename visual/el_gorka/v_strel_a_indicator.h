#ifndef V_STREL_A_INDICATOR_H
#define V_STREL_A_INDICATOR_H

#include "v_base.h"
#include "m_base.h"
#include "objectlink.h"

class v_Strel_A_Indicator : public v_Base
{
    Q_OBJECT
    Q_PROPERTY(ObjectLink TARGET READ TARGET WRITE setTARGET DESIGNABLE true )
public:
    MYPROP(QSizeF,SIZE)
    MYSTATE(bool, STATE_A)
    DECLARECOLOR(strel_auto)
    ObjectLink TARGET() const {return FTARGET;}
    void setTARGET(ObjectLink p);
public:
    Q_INVOKABLE v_Strel_A_Indicator(v_Base *parent = nullptr);
    virtual ~v_Strel_A_Indicator(){}
    virtual void updateAfterLoad();
    virtual void calculateGeometry(); // перестраивает внтренние координаты

    virtual void updateStates();
public slots:

    virtual void slotModelStateChanged(QObject *);  // следим за моделью
protected:
    QPointF M[1];
    ObjectLink FTARGET;
    m_Base *strel;
    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    QPolygonF AUTOPL;




};

#endif // V_STREL_A_INDICATOR_H
