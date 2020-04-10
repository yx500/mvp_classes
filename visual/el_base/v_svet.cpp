#include "v_svet.h"
#include "qpfunction.h"

#include "v_rc.h"

#include "mvp_objectfactory.h"
//REGISTERELEMENT(v_Svet,"Cветофор","VISUAL Стандартные")

REGISTERPROPERTY(v_Svet,SH,"РАЗМЕР","Размер светофора","")

REGISTERCOLOR(v_Svet,svet_zel,"Светофор зелёный",Qt::green);
REGISTERCOLOR(v_Svet,svet_bel,"Светофор белый",Qt::white);
REGISTERCOLOR(v_Svet,svet_kra,"Светофор красный",Qt::red);
REGISTERCOLOR(v_Svet,svet_jelt,"Светофор жёлтый",Qt::yellow);
REGISTERCOLOR(v_Svet,svet_siniy,"Светофор синий",Qt::blue);
REGISTERCOLOR2(v_Svet,svet_zel_mig,"Светофор зелёный миг.",Qt::green,QColor(0,0,0,0));
REGISTERCOLOR2(v_Svet,svet_bel_mig,"Светофор белый миг.",Qt::white,QColor(0,0,0,0));
REGISTERCOLOR2(v_Svet,svet_kra_mig,"Светофор красный миг.",Qt::red,QColor(0,0,0,0));
REGISTERCOLOR2(v_Svet,svet_jelt_mig,"Светофор жёлтый миг.",Qt::yellow,QColor(0,0,0,0));
REGISTERCOLOR2(v_Svet,svet_siniy_mig,"Светофор синий миг.",Qt::blue,QColor(0,0,0,0));

//const VisualGroupProperty_int v_Svet::groupProp_SHSVET("SHSVET","РАЗМЕР СВЕТОФОРА",0,10);

v_Svet::v_Svet(v_Base *parent) :
    v_Model(parent)
{
    FSH=10;
    FTEXT="Ч";
    FMANEVROV=false;
    font=QFont("Times", 12);
    addCOMMON_PROPERTY("SH","SH_SVET");
    calculateGeometry();
}



void v_Svet::setSH(int p)
{
    if (p>=0){
        if (FSH!=p){
            FSH=p;
            doPropertyChanged();
        }
    }
}




void v_Svet::showZABLOK(QPainter *painter)
{
    QColor clr=getColor(v_RC::Color_rc_blok);
    qreal r=gridDX()*4;
    if (r<gridDY()*3) r=gridDY()*4;
    if (r<S*4) r=S*4;

    ZablokR=QRectF(0,0,r,r);
    ZablokR.moveCenter(M[0]);
    painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));
    painter->setBrush(clr);
    painter->drawRect(ZablokR);
}

void v_Svet::updateStatesFromModel()
{
    v_Model::updateStatesFromModel();
}


void v_Svet::calculateGeometry_T()
{
    //       1
    //    RT 0  3 MSV
    //       2

    S=FSH;//viewProperty_int(groupProp_SHSVET,FSH);
    M[0]=QPointF(0,0);
    T[0]=M[0];
    T[3]=QPointF(M[0].x()+S*2,M[0].y());
    if (MANEVROV())
        T[3]=T[0];
    T[1]=QPointF(T[0].x(),T[0].y()-S);
    T[2]=QPointF(T[0].x(),T[0].y()+S);
    RSV.setSize(QSizeF(S*2,S*2));
    RSV.moveCenter(QPointF(T[3].x()+S,T[3].y()));

}


void v_Svet::calculateGeometry()
{

    calculateGeometry_T();
    font.setPointSize(S*2-1);


    FULLP.clear();
    FULLP << T[0] << T[1] << T[2] << T[3];
    FULLP=FULLP.united(QPolygonF(RSV));
    boundrect_text=QFontMetrics(font).boundingRect(FTEXT);
    boundrect_text.moveCenter(QPointF(T[0].x()-4-boundrect_text.width()/2,T[0].y()));

    // переворот
    if (FINVERTDRAWX){
        QPFunction::reflectRectx(&boundrect_text,0);
        QPFunction::reflectPLx(&FULLP,0);
        for (int i=0;i<4;i++){
            T[i]=QPFunction::reflectPx(T[i],0);
        }
        QPFunction::reflectRectx(&RSV,0);
        boundrect_text.moveCenter(QPointF(T[0].x()+4+boundrect_text.width()/2,T[0].y()));
    }

    {
        QPolygonF P;
        P =FULLP.united(boundrect_text);
        _boundingRect=P.boundingRect();
    }


}

void v_Svet::show_text(QPainter *painter)
{
    painter->setFont(font);
    painter->setBrush(Qt::NoBrush);
    int flags=Qt::AlignCenter;
    painter->drawText(boundrect_text,flags,FTEXT);
}


