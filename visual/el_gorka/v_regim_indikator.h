#ifndef V_REGIM_INDIKATOR_H
#define V_REGIM_INDIKATOR_H

#include "v_stat_text.h"

class v_Regim_Indikator : public v_StaticText
{
    Q_OBJECT
    DECLARECOLOR(p_regimRospusk)
    DECLARECOLOR(p_regimPausa)
    DECLARECOLOR(p_regimStop)
    DECLARECOLOR(b_regimRospusk)
    DECLARECOLOR(b_regimPausa)
    DECLARECOLOR(b_regimStop)

public:

    Q_INVOKABLE v_Regim_Indikator(v_Base *parent = nullptr);

    virtual ~v_Regim_Indikator(){}

    virtual void resetStates();
    virtual void updateStates();
    virtual QString getText();
    virtual QColor getTextColor();
    virtual QColor getRectColor();
    virtual QColor getBrusColor();
signals:

public slots:
protected:
    int regim;
};

#endif // V_REGIM_INDIKATOR_H
