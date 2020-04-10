#ifndef V_STRELKA_DSO_H
#define V_STRELKA_DSO_H

#include "v_strelka.h"
#include "m_strel_dso_dbk.h"


class v_Strelka_DSO : public v_Strelka
{
    Q_OBJECT
    USEMODEL(m_STREL_DSO_DBK)

public:
    Q_INVOKABLE v_Strelka_DSO(v_Base *parent = 0);
    virtual ~v_Strelka_DSO(){}
    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual int getSTATE_BUSY();
protected:
};

#endif // V_STRELKA_DSO_H
