#ifndef V_SVET_GMC_H
#define V_SVET_GMC_H

#include "v_svet_gor_1.h"

class v_Svet_GMC : public v_Svet_Gor_1
{
    Q_OBJECT
    public:
    MYSTATE(bool, STATE_GMC_OPEN_ENABLE)
    MYSTATE(bool, STATE_GMC_BLOCK)
    MYSTATE(bool, STATE_GMC_TU_OPEN)
    MYSTATE(bool, STATE_GMC_TU_CLOSE)
    MYSTATE(bool, STATE_GMC_ERROR)
    MYSTATE(bool, STATE_GMC_OPEN_CONDITION)

    MYSTATE(bool, STATE_DBK_OPEN0)
    MYSTATE(bool, STATE_DBK_OPENBLINK0)
    MYSTATE(bool, STATE_DBK_ERROR0)
    MYSTATE(bool, STATE_DBK_OPEN1)
    MYSTATE(bool, STATE_DBK_OPENBLINK1)
    MYSTATE(bool, STATE_DBK_ERROR1)
public:
    Q_INVOKABLE v_Svet_GMC(v_Base *parent = 0);
    virtual ~v_Svet_GMC(){}

    virtual void drawBackground(QPainter * painter) ;

protected:
    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void updateStatesFromModel();


signals:

public slots:
};

#endif // V_SVET_GMC_H
