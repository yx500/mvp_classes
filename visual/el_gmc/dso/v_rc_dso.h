#ifndef V_RC_DSO_H
#define V_RC_DSO_H

#include "v_rc.h"
#include "m_rc_dso.h"


class v_RC_DSO : public v_RC
{
    Q_OBJECT
    USEMODEL(m_RC_DSO)
public:
    Q_INVOKABLE v_RC_DSO(v_Base *parent = 0);
    virtual ~v_RC_DSO(){}
    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
private:

};

#endif // V_RC_DSO_H
