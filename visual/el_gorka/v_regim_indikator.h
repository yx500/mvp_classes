#ifndef V_REGIM_INDIKATOR_H
#define V_REGIM_INDIKATOR_H

#include "v_model.h"

class v_Regim_Indikator : public v_Model
{
    Q_OBJECT
    MYPROP(QSizeF,SIZE)
    DECLARECOLOR(p_regimRospusk)
    DECLARECOLOR(p_regimPausa)
    DECLARECOLOR(p_regimStop)
    DECLARECOLOR(b_regimRospusk)
    DECLARECOLOR(b_regimPausa)
    DECLARECOLOR(b_regimStop)

public:

    Q_INVOKABLE v_Regim_Indikator(QObject *parent = nullptr);

    virtual ~v_Regim_Indikator(){}

    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void resetStates();
    virtual void calculateGeometry(); // перестраивает внтренние координаты
    virtual void updateStates();
signals:

public slots:
protected:
    QRectF  allign_rect;
    int regim;
};

#endif // V_REGIM_INDIKATOR_H
