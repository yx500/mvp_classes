#include "v_strelka_gac.h"

#include "qpfunction.h"
#include "m_strel_gor_y.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_Strelka_GAC,"Cтрелка ГАЦ","VISUAL ГОРКА")

REGISTERCOLOR(v_Strelka_GAC,strel_prp_p,"Стрелка ПРП линия",Qt::yellow);
REGISTERCOLOR(v_Strelka_GAC,strel_prp_b,"Стрелка ПРП заполн",Qt::red);

v_Strelka_GAC::v_Strelka_GAC(v_Base *parent) : v_Strelka(parent)
{
    resetStates();
}

void v_Strelka_GAC::resetStates()
{
    v_Strelka::resetStates();
}

void v_Strelka_GAC::calculateGeometry()
{
    v_Strelka::calculateGeometry();
    // ПРМ ПРП
    qreal sh_pr=S*.075; // ширина стрелки
    if (sh_pr<4) sh_pr=4;
    qreal ln_pr=sh_pr*4; // длинна стрелки
    {   // ПРМ
        QPointF prm1=QPFunction::pointOnLine(z23_1,P[3].at(2),ln_pr);
        QLineF l1=QLineF(z23_1,rD3_2);
        QLineF l2=QLineF(z23_1,prm1);
        if (l2.length()>=l1.length()) prm1=rD3_2;
        QPointF prm2=QPFunction::pointOnLine(z23_1,z23_2,-sh_pr);
        PRMPL.clear();
        PRMPL<<z23_1 << prm1 << prm2;
    }
    {   // ПРП
        QPointF prp1=QPFunction::pointOnLine(z32_1,P[2].at(1),ln_pr);
        QLineF l1=QLineF(z32_1,rD2_1);
        QLineF l2=QLineF(z32_1,prp1);
        if (l2.length()>=l1.length()) prp1=rD2_1;
        QPointF prp2=QPFunction::pointOnLine(z32_1,z32_2,-sh_pr);
        PRPPL.clear();
        PRPPL<<z32_1 << prp1 << prp2;
    }
    if (FINVERTDRAWY){
        QPFunction::reflectPLy(&PRMPL,0);
        QPFunction::reflectPLy(&PRPPL,0);


    }
    if (FINVERTDRAWX){
        QPFunction::reflectPLx(&PRMPL,0);
        QPFunction::reflectPLx(&PRPPL,0);
    }

    {
        QPolygonF P=QPolygonF(v_Strelka::boundingRect());
        P=P.united(PRPPL);
        P=P.united(PRMPL);
        _boundingRect=P.boundingRect();
    }

}

void v_Strelka_GAC::d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    v_Strelka::d_paint(painter,option);

        showPRPPRM(painter);

}



void v_Strelka_GAC::updateState()
{
    v_Strelka::updateStates();
    auto strel=qobject_cast<m_Strel_Gor_Y*>(modelObject());
    if (strel!=nullptr) {
        if ((strel->STATE_PRP()==1)||(strel->STATE_UVK_PRP()==1)) dtPrOff[0].start();
        if ((strel->STATE_PRM()==1)||(strel->STATE_UVK_PRM()==1)) dtPrOff[1].start();
    }
}

const int ms_fade_prp=250;
void v_Strelka_GAC::showPRPPRM(QPainter *painter)
{
    auto strel=qobject_cast<m_Strel_Gor_Y*>(modelObject());
    if (strel==nullptr) return;
    bool prp=false;
    bool prm=false;
    if ((strel->STATE_PRP()==1)||(strel->STATE_UVK_PRP()==1)) prp=true;
    if ((strel->STATE_PRM()==1)||(strel->STATE_UVK_PRM()==1)) prm=true;

    QColor cb=getColor(Color_strel_prp_b);
    QColor cp=getColor(Color_strel_prp_p);
    if (ISEDITORMODE){
        cb=Qt::gray;
        cp=Qt::black;
        prp=true;prm=true;
    }
    QColor c2=QColor(Qt::gray);
    c2.setAlphaF(0);
    if ((prp) || ((dtPrOff[0].isValid())&&(dtPrOff[0].elapsed()<ms_fade_prp))){

        if (strel->STATE_UVK_PRP())
            painter->setPen(QPen(QBrush(Qt::red),0,Qt::SolidLine)); else
            painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));
        if (strel->STATE_PRP()){
            qreal procent=1.*dtPrOff[0].elapsed()/ms_fade_prp;
            QColor c=Color2Color(cp,c2,procent);
            c=Color2Color(cb,c2,procent);
            painter->setBrush(QBrush(c));
        } else {
             painter->setBrush(QBrush(Qt::gray));
        }
        if (!FINVERSPOL) painter->drawPolygon(PRPPL); else painter->drawPolygon(PRMPL);
    }
    if ((prm) || ((dtPrOff[1].isValid())&&(dtPrOff[1].elapsed()<ms_fade_prp))){
        if (strel->STATE_UVK_PRM())
            painter->setPen(QPen(QBrush(Qt::red),0,Qt::SolidLine)); else
            painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));
        if (strel->STATE_PRM()){
            qreal procent=1.*dtPrOff[1].elapsed()/ms_fade_prp;
            QColor c=Color2Color(cp,c2,procent);
            c=Color2Color(cb,c2,procent);
            painter->setBrush(QBrush(c));
        } else {
             painter->setBrush(QBrush(Qt::gray));
        }
        if (!FINVERSPOL) painter->drawPolygon(PRMPL); else painter->drawPolygon(PRPPL);
    }
}
