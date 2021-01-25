#include "v_regim_indikator.h"

REGISTERCOLOR(v_Regim_Indikator,p_regimRospusk,"Режим РОСПУСК текст",Qt::black);
REGISTERCOLOR(v_Regim_Indikator,b_regimRospusk,"Режим РОСПУСК фон",Qt::green);
REGISTERCOLOR(v_Regim_Indikator,p_regimPausa,"Режим ПАУЗА текст",Qt::black);
REGISTERCOLOR(v_Regim_Indikator,b_regimPausa,"Режим ПАУЗА фон",Qt::yellow);
REGISTERCOLOR(v_Regim_Indikator,p_regimStop,"Режим СТОП текст",Qt::white);
REGISTERCOLOR(v_Regim_Indikator,b_regimStop,"Режим СТОП фон",Qt::red);

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_Regim_Indikator,"Режим индикатор","VISUAL ГОРКА")
v_Regim_Indikator::v_Regim_Indikator(v_Base *parent) : v_StaticText(parent)
{
    regim=-1;
}



void v_Regim_Indikator::resetStates()
{
    regim=-1;
}



void v_Regim_Indikator::updateStates()
{
    if (modelObject()!=nullptr){
        regim=modelObject()->property("STATE_REGIM").toInt();
        if (modelObject()->property("STATE_33")==1) regim=33;
    }
}

QString v_Regim_Indikator::getText()
{
    switch (regim) {
    case 0: return "СТОП";
    case 1: return "РОСПУСК";
    case 2: return "ПАУЗА";
    default:return  "-----";
    }
}

QColor v_Regim_Indikator::getTextColor()
{
    switch (regim) {
    case 0: return getColor(Color_p_regimStop);
    case 1: return (Color_p_regimRospusk);
    case 2: return getColor(Color_p_regimPausa);
    default:return  Qt::gray;
    }
}

QColor v_Regim_Indikator::getRectColor()
{
    return  Qt::black;
}

QColor v_Regim_Indikator::getBrusColor()
{
    switch (regim) {
    case 0: return getColor(Color_b_regimStop);
    case 1: return getColor(Color_b_regimRospusk);
    case 2: return getColor(Color_b_regimPausa);
        case 33: return getColor(Color_state_33);
    default:return  Qt::gray;
    }
}
