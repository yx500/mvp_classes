#include "v_svet_gor_1.h"

#include "qpfunction.h"
#include "mcolorpalette.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_Svet_Gor_1,"Cветофор 1зн","VISUAL ГОРКА")

v_Svet_Gor_1::v_Svet_Gor_1(v_Base *parent) : v_Svet(parent)
{
    FCOLOR_OPEN=clr_green;
    FCOLOR_CLOSE=clr_fon;
    resetStates();
}

void v_Svet_Gor_1::resetStates()
{
    v_Svet::resetStates();
    FSTATE_OPEN=false;
    FSTATE_OPEN_BLINK=false;
}


QColor v_Svet_Gor_1::colorOpen(bool blink)
{
    switch (FCOLOR_OPEN){
    case clr_green: if (blink) return getColor(v_Svet::Color_svet_zel_mig); else return getColor(v_Svet::Color_svet_zel);
    case clr_white: if (blink) return getColor(v_Svet::Color_svet_bel_mig); else return getColor(v_Svet::Color_svet_bel);break;
    case clr_yellow:if (blink) return getColor(v_Svet::Color_svet_jelt_mig); else return getColor(v_Svet::Color_svet_jelt);
    }
    return getColor(Color_svet_zel);
}

QColor v_Svet_Gor_1::colorClose()
{
    switch (FCOLOR_CLOSE){
    case clr_red: return getColor(Color_svet_kra);
    case clr_fon: return QColor(0,0,0,0);
    case clr_blue:return getColor(Color_svet_siniy);
    }
    return QColor(0,0,0,0);
}


void v_Svet_Gor_1::d_paint(QPainter *painter, const QStyleOptionGraphicsItem *)
{
    QColor clrB=QColor();
    if (FSTATE_OPEN) clrB=colorOpen(FSTATE_OPEN_BLINK) ; else
                                    clrB=colorClose() ;
    painter->setPen(QPen(QBrush(Qt::black),1,Qt::SolidLine));
    painter->drawLine(T[1],T[2]);
    if (!MANEVROV()) painter->drawLine(T[0],T[3]);
    painter->setBrush(clrB);
    painter->drawEllipse(RSV);
    show_text(painter);
}

void v_Svet_Gor_1::updateStatesFromModel()
{
    v_Svet::updateStatesFromModel();
    SET_STATE_MODEL_BOOL(STATE_OPEN);
    SET_STATE_MODEL_BOOL(STATE_OPEN_BLINK);
}


