#include "v_svet_gor_4n.h"

#include "qpfunction.h"
#include "mcolorpalette.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_Svet_Gor_4N,"Cветофор 4знН","VISUAL ГОРКА")

v_Svet_Gor_4N::v_Svet_Gor_4N(v_Base *parent) : v_Svet(parent)
{
    FTEXT="Г";
    resetStates();
}

void v_Svet_Gor_4N::resetStates()
{
    v_Svet::resetStates();
    FSTATE_K=false;
    FSTATE_J=false;
    FSTATE_Z=false;
    FSTATE_B=false;
    FSTATE_NAZAD=false;
}

void v_Svet_Gor_4N::calculateGeometry()
{

    //       1
    //    RT 0 TN M4
    //       2

    v_Svet::calculateGeometry_T();
    font.setPointSize(S*2-1);

    NAZADR.setSize(QSizeF(S*2,S*2));
    NAZADR.moveCenter(QPointF(T[0].x()+1+NAZADR.width()/2,T[0].y()));
    for (int i=0;i<4;i++){
        RSV4[i].setSize(QSizeF(S*2,S*2));
        RSV4[i].moveCenter(QPointF(T[0].x()+1+NAZADR.width()+1+(i*S*2)+S,T[0].y()));
    }
    boundrect_text=QFontMetrics(font).boundingRect(FTEXT);
    boundrect_text.moveCenter(QPointF(T[0].x()-4-boundrect_text.width()/2,T[0].y()));
    FULLP.clear();
    FULLP << T[0] << T[1] << T[2] << T[3];
    for (int i=0;i<4;i++)
        FULLP=FULLP.united(QPolygonF(RSV4[i]));

    if (FINVERTDRAWX){
        QPFunction::reflectPLx(&FULLP,0);
        for (int i=0;i<4;i++){
            T[i]=QPFunction::reflectPx(T[i],0);
        }
        QPFunction::reflectRectx(&boundrect_text,0);
        QPFunction::reflectRectx(&NAZADR,0);
        for (int i=0;i<4;i++)
            QPFunction::reflectRectx(&RSV4[i],0);
    }
    {
        QPolygonF P;
        P =FULLP.united(boundrect_text);
        P =P.united(NAZADR);
        _boundingRect=P.boundingRect();
    }


}

void v_Svet_Gor_4N::d_paint(QPainter *painter, const QStyleOptionGraphicsItem *)
{
    if (FSTATE_33){
        for (int i=0;i<4;i++) clrB[i]=getColor(Color_state_33);
    } else {
        if (FSTATE_B) clrB[0]=getColor(Color_svet_bel); else clrB[0]=QColor();
        if (FSTATE_Z) clrB[1]=getColor(Color_svet_zel); else clrB[1]=QColor();
        if (FSTATE_K) clrB[2]=getColor(Color_svet_kra); else clrB[2]=QColor();
        if (FSTATE_J) clrB[3]=getColor(Color_svet_jelt); else clrB[3]=QColor();
    }

    painter->setPen(QPen(QBrush(Qt::black),1,Qt::SolidLine));
    //painter->drawLine(T[1],T[2]);
    if (FSTATE_NAZAD) painter->setBrush(getColor(Color_svet_bel)); else painter->setBrush(Qt::NoBrush);
    painter->drawRect(NAZADR);
    if (FSTATE_NAZAD) {
        QFont font("Times", NAZADR.height()-1);
        painter->setFont(font);
        painter->setBrush(Qt::NoBrush);
        int flags=Qt::AlignCenter;
        painter->drawText(NAZADR,flags,"H");
    }

    for (int i=0;i<4;i++){
        if (clrB[i].isValid()) painter->setBrush(clrB[i]); else painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(RSV4[i]);
    }

    show_text(painter);
}

void v_Svet_Gor_4N::updateStatesFromModel()
{
    v_Svet::updateStatesFromModel();
}
