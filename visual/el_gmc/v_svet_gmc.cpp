#include "v_svet_gmc.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_Svet_GMC,"Светофор ГМЦ ДБК","VISUAL Горка ГМЦ")

v_Svet_GMC::v_Svet_GMC(v_Base *parent) : v_Svet_Gor_1(parent)
{
    FSTATE_GMC_OPEN_ENABLE=0;
    FSTATE_GMC_BLOCK=0;
    FSTATE_GMC_TU_OPEN=0;
    FSTATE_GMC_TU_CLOSE=0;
    FSTATE_GMC_ERROR=0;
    FSTATE_GMC_OPEN_CONDITION=0;

    FSTATE_DBK_ERROR0=0;
    FSTATE_DBK_OPEN0=0;
    FSTATE_DBK_OPENBLINK0=0;
    FSTATE_DBK_ERROR1=0;
    FSTATE_DBK_OPEN1=0;
    FSTATE_DBK_OPENBLINK1=0;
}

void v_Svet_GMC::drawBackground(QPainter *painter)
{
    v_Base::drawBackground(painter);
    if (FSTATE_GMC_BLOCK)
        showZABLOK(painter);

}

void v_Svet_GMC::d_paint(QPainter *painter, const QStyleOptionGraphicsItem *)
{
    QColor clrB=QColor();
    QColor clrP=Qt::black;
    if (FSTATE_DBK_OPEN0) clrB=colorOpen(FSTATE_OPEN_BLINK) ; else

        if (FSTATE_DBK_OPEN1) clrB=colorClose() ;
    if ((FSTATE_DBK_ERROR0)||(FSTATE_DBK_ERROR1)||(FSTATE_GMC_ERROR)){
        clrP=getColor(Color_svet_kra_mig);
    }

    if (clrB.isValid()){
        painter->setBrush(clrB);
    } else {
        painter->setBrush(Qt::NoBrush);
    }


    painter->setPen(QPen(QBrush(clrP),0,Qt::SolidLine));
    painter->drawLine(T[1],T[2]);
    painter->drawLine(T[0],T[4]);

    painter->drawEllipse(M[0],S,S);
    show_text(painter);
}

void v_Svet_GMC::updateStatesFromModel()
{
    v_Svet::updateStatesFromModel();

    SET_STATE_MODEL_BOOL(STATE_GMC_OPEN_ENABLE);
    SET_STATE_MODEL_BOOL(STATE_GMC_BLOCK);
    SET_STATE_MODEL_BOOL(STATE_GMC_TU_OPEN);
    SET_STATE_MODEL_BOOL(STATE_GMC_TU_CLOSE);
    SET_STATE_MODEL_BOOL(STATE_GMC_ERROR);
    SET_STATE_MODEL_BOOL(STATE_GMC_OPEN_CONDITION);

    SET_STATE_MODEL_BOOL(STATE_DBK_ERROR0);
    SET_STATE_MODEL_BOOL(STATE_DBK_OPEN0);
    SET_STATE_MODEL_BOOL(STATE_DBK_OPENBLINK0);
    SET_STATE_MODEL_BOOL(STATE_DBK_ERROR1);
    SET_STATE_MODEL_BOOL(STATE_DBK_OPEN1);
    SET_STATE_MODEL_BOOL(STATE_DBK_OPENBLINK1);

    setSTATE_OPEN(FSTATE_DBK_OPEN0);
    setSTATE_OPEN_BLINK(FSTATE_DBK_OPENBLINK0);
}

