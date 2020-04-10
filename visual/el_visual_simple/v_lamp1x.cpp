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

v_Lamp1x::v_Lamp1x(v_Base *parent) : v_Base(parent)
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
    FFLAGS=MVP_Enums::TTextFlag::AlignCenter;
    addCOMMON_PROPERTY("SH","TEXTH");
    calculateGeometry();
    resetStates();

}



void v_Lamp1x::setSTATE_SIGNAL(int p)
{
    if (p!=FSTATE_SIGNAL) {
        FSTATE_SIGNAL=p;
        update();
    }
}


void v_Lamp1x::d_paint(QPainter *painter, const QStyleOptionGraphicsItem *)
{
    QPen P;
    QBrush B;
    QString text=objectName();


    if (ISEDITORMODE){
        P=QPen(Qt::black);
        B=QBrush(Qt::white);
        if (FSIGNAL_1.isEmpty()){
            B=QBrush(Qt::gray);
        }
    } else{
        QColor penColor;
        QColor brushColor;
        if ((FSTATE_SIGNAL<0)||(FSTATE_SIGNAL>=16)){
            penColor=Qt::black;
            brushColor=getColor(Color_state_33);
        } else {
            penColor=getColor(FSCOLOR_P[FSTATE_SIGNAL]);
            brushColor=getColor(FSCOLOR_B[FSTATE_SIGNAL]);
            if (!FSTEXT[FSTATE_SIGNAL].isEmpty()) text=FSTEXT[FSTATE_SIGNAL];
        }
        P=QPen(penColor);
        B=QBrush(brushColor);
    }

    painter->setBrush(B);
    painter->setPen(P);
    if (P.color().isValid()) {
        if (!FROUND_RECT) painter->drawRect(allign_rect); else
                          painter->drawRoundedRect(allign_rect, allign_rect.height()/5, allign_rect.height()/5);
    }
    RTEXTH >0 ? FFONT.setPointSizeF(RTEXTH): FFONT.setPointSizeF(1);
    painter->setFont(FFONT);
    painter->drawText(allign_rect,FFLAGS,text);
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

void v_Lamp1x::calculateGeometry()
{
    allign_rect=QRectF(0,0,FSIZEALLIGNRECT.width()*gridDX(),FSIZEALLIGNRECT.height()*gridDY());
    allign_rect.moveCenter(QPointF(0,0));
    if (FSH!=0) RTEXTH=FSH; else RTEXTH=allign_rect.height()-2;
    {
        QRectF R=allign_rect;
        _boundingRect=R.adjusted(-1,-1,+1,+1);
    }
}



