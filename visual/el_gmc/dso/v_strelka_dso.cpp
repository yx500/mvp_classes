#include "v_strelka_dso.h"


#include "mcolorpalette.h"
#include "mcolorpalette.h"

#include "qpfunction.h"


#include "mvp_objectfactory.h"
REGISTERELEMENT(v_Strelka_DSO,"Стелка с ДСО ДБК","ДБК")

v_Strelka_DSO::v_Strelka_DSO(v_Base *parent ):
    v_Strelka(parent)
{
    _m_STREL_DSO_DBK=0;
}


void v_Strelka_DSO::d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    v_Strelka::d_paint(painter, option);
}

int v_Strelka_DSO::getSTATE_BUSY()
{
    if (_m_STREL_DSO_DBK) {
        if (_m_STREL_DSO_DBK->STATE_DSO_ERROR()) return 2;
        return _m_STREL_DSO_DBK->STATE_BUSY();
    }
    return 0;
}

