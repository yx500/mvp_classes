#include "v_lamp4s.h"

#include "baseobjecttools.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_Lamp4s,"Лампочка4зн_сост","VISUAL Простые элементы")


void v_Lamp4s::setCOLOR_TEXT_16(QVariantHash &p)
{
    if (FCOLOR_TEXT_H!=p){
        FCOLOR_TEXT_H=p;
        for (int i =0;i<15;i++){
            QString k=QString::number(i,2);
            while (k.length()<4) k="0"+k;
            FCOLOR_TEXT_16[i]=FCOLOR_TEXT_H[k].toString();
        }
        doPropertyChanged();
    }
}

void v_Lamp4s::setCOLOR_RECT_16(QVariantHash &p)
{
    if (FCOLOR_RECT_H!=p){
        FCOLOR_RECT_H=p;
        for (int i =0;i<15;i++){
            QString k=QString::number(i,2);
            while (k.length()<4) k="0"+k;
            FCOLOR_RECT_16[i]=FCOLOR_RECT_H[k].toString();
        }
        doPropertyChanged();
    }
}

void v_Lamp4s::setCOLOR_BRUSH_16(QVariantHash &p)
{
    if (FCOLOR_BRUSH_H!=p){
        FCOLOR_BRUSH_H=p;
        for (int i =0;i<15;i++){
            QString k=QString::number(i,2);
            while (k.length()<4) k="0"+k;
            FCOLOR_BRUSH_16[i]=FCOLOR_BRUSH_H[k].toString();
        }
        doPropertyChanged();
    }
}

v_Lamp4s::v_Lamp4s(v_Base *parent) : v_StaticText(parent)
{
    for (int i =0;i<15;i++){
        FCOLOR_TEXT_16[i]="";
        FCOLOR_RECT_16[i]="";
        FCOLOR_BRUSH_16[i]="";
        QString k=QString::number(i,2);
        while (k.length()<4) k="0"+k;
        FCOLOR_TEXT_H[k]="";
        FCOLOR_RECT_H[k]="";
        FCOLOR_BRUSH_H[k]="";
    }
    for (int i=0;i<4;i++) B[i]=nullptr;
}

void v_Lamp4s::updateAfterLoad()
{
    v_StaticText::updateAfterLoad();


    FLNKMODEL2.linkObj( superFindObjectById(this,FLNKMODEL2.id()));
    FLNKMODEL3.linkObj( superFindObjectById(this,FLNKMODEL3.id()));
    FLNKMODEL4.linkObj( superFindObjectById(this,FLNKMODEL4.id()));
    B[0]=FLNKMODEL.baseObject();
    B[1]=FLNKMODEL2.baseObject();
    B[2]=FLNKMODEL3.baseObject();
    B[3]=FLNKMODEL4.baseObject();
    S[0]=FSTATE1;
    S[1]=FSTATE2;
    S[2]=FSTATE3;
    S[3]=FSTATE4;

}

void v_Lamp4s::updateStates()
{
    v_StaticText::updateStates();

}

void v_Lamp4s::updateStatesFromModel()
{
    FSTATE_SIGNAL=0;
    int s[4];
    for (int i=0;i<4;i++){
        if (B[i]==nullptr) s[i]=0; else
        {
            if (B[i]->property(qPrintable(S[i])).isValid()) s[i]=B[i]->property(qPrintable(S[i])).toInt();
        }
    }
    FSTATE_SIGNAL=s[3]*8+s[2]*4+s[1]*2+s[0];
}

QColor v_Lamp4s::getTextColor()
{
       if (FCOLOR_TEXT_16[FSTATE_SIGNAL].isEmpty()) return FCOLOR_TEXT;
       return getColor(FCOLOR_TEXT_16[FSTATE_SIGNAL]);
}

QColor v_Lamp4s::getRectColor()
{
    if (FCOLOR_TEXT_16[FSTATE_SIGNAL].isEmpty()) return FCOLOR_RECT;
    return getColor(FCOLOR_TEXT_16[FSTATE_SIGNAL]);
}

QColor v_Lamp4s::getBrusColor()
{
    if (FCOLOR_BRUSH_16[FSTATE_SIGNAL].isEmpty()) return FCOLOR_BRUSH;
    return getColor(FCOLOR_BRUSH_16[FSTATE_SIGNAL]);
}
