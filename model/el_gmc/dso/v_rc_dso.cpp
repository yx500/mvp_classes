#include "v_rc_dso.h"

#include "mcolorpalette.h"
#include "mcolorpalette.h"

#include "qpfunction.h"


#include "mvp_objectfactory.h"
REGISTERELEMENT(v_RC_DSO,"РЦ с ДСО","ДБК")
v_RC_DSO::v_RC_DSO(v_Base *parent ):
    v_RC(parent)
{
    _m_RC_DSO=0;
}



void v_RC_DSO::d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    v_RC::d_paint(painter,option);
}

