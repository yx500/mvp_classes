#include "v_lamp1x.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_Lamp1x,"Лампочка1зн","VISUAL Простые элементы")



REGISTERPROPERTY(v_Lamp1x,SCOLOR_B0,"CLRS_B__0_0000","Цвет заполн 0","Цвет заполн")
REGISTERPROPERTY(v_Lamp1x,SCOLOR_B1,"CLRS_B__1_0001","Цвет заполн 0","Цвет заполн")

REGISTERPROPERTY(v_Lamp1x,SCOLOR_P0, "CLRS_P__0_0000","Цвет букв 0","Цвет текст")
REGISTERPROPERTY(v_Lamp1x,SCOLOR_P1, "CLRS_P__1_0001","Цвет букв 1","Цвет текст")

REGISTERPROPERTY(v_Lamp1x,STEXT_0, "TXTS__0_0000","Текст 0","Текст")
REGISTERPROPERTY(v_Lamp1x,STEXT_1, "TXTS__1_0001","Текст 1","Текст")


void v_Lamp1x::setSCOLOR_P(int i, QString &p)
{
    if (FSCOLOR_P[i]!=p){
        FSCOLOR_P[i]=p;
        doStateChanged();
    }
}

void v_Lamp1x::setSCOLOR_B(int i, QString &p)
{
    if (FSCOLOR_B[i]!=p){
        FSCOLOR_B[i]=p;
        doStateChanged();
    }
}

void v_Lamp1x::setTEXT_(int i, QString &p)
{
    if (FSTEXT[i]!=p){
        FSTEXT[i]=p;
        doStateChanged();
    }
}

v_Lamp1x::v_Lamp1x(v_Base *parent) : v_StaticText(parent)
{
    FFONT=QFont("Times", 12-1);

    for (int i=0;i<16;i++){
        FSCOLOR_P[i]="";
        FSCOLOR_B[i]="";
        FSTEXT[i]="";
    }
    FSCOLOR_P[0]="black";
    FSCOLOR_P[1]="black";
    FSCOLOR_B[0]="gray";
    FSCOLOR_B[1]="red";


    FSIGNAL_1.clear();
    FSTATE_SIGNAL=0;
    FSIZEALLIGNRECT=QSizeF(4,2);

    calculateGeometry();
    resetStates();

}



void v_Lamp1x::setSTATE_SIGNAL(int p)
{
    if (p<0) p=0;
    if (p>=16) p=15;
    if (p!=FSTATE_SIGNAL) {
        FSTATE_SIGNAL=p;
        update();
    }
}




void v_Lamp1x::updateStates()
{
    v_Base::updateStates();
    if (FSIGNAL_1.is33()) setSTATE_SIGNAL(-1); else
        setSTATE_SIGNAL(FSIGNAL_1.value_1bit());
}

void v_Lamp1x::resetStates()
{
    v_Base::resetStates();
    FSTATE_SIGNAL=0;
}



QString v_Lamp1x::getText()
{
    if (!FSTEXT[FSTATE_SIGNAL].isEmpty()) return FSTEXT[FSTATE_SIGNAL];
    return objectName();

}

QColor v_Lamp1x::getTextColor()
{
    if (!FSCOLOR_P[FSTATE_SIGNAL].isEmpty()) return getColor(FSCOLOR_P[FSTATE_SIGNAL]);
    return FCOLOR_TEXT;
}

QColor v_Lamp1x::getRectColor()
{
    return Qt::black;
}

QColor v_Lamp1x::getBrusColor()
{
    if (!FSCOLOR_B[FSTATE_SIGNAL].isEmpty()) return getColor(FSCOLOR_B[FSTATE_SIGNAL]);
    return FCOLOR_BRUSH;
}



