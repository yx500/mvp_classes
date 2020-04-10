#include "v_strelka.h"

#include "qpfunction.h"
#include "baseobjecttools.h"
#include "m_rc.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_Strelka,"Cтрелка","VISUAL Стандартные РЦ")

REGISTERCOLOR(v_Strelka,strel_text_plus,"Стрелка текст плюс",Qt::green);
REGISTERCOLOR(v_Strelka,strel_text_minus,"Стрелка текст минус",Qt::yellow);
REGISTERCOLOR(v_Strelka,strel_text_w,"Стрелка текст W",Qt::red);


REGISTERPROPERTY(v_Strelka,M1,"M1","Точка пред относительно центра","Геометрия")
REGISTERPROPERTY(v_Strelka,M2,"M2","Точка по плюсу относительно центра","Геометрия")
REGISTERPROPERTY(v_Strelka,M3,"M3","Точка по минусу относительно центра","Геометрия")
REGISTERPROPERTY(v_Strelka,MD1,"MD1","Дополн.точка  относительно точки М1","Геометрия")
REGISTERPROPERTY(v_Strelka,MD2,"MD2","Дополн.точка относительно точки М2","Геометрия")
REGISTERPROPERTY(v_Strelka,MD3,"MD3","Дополн.точка относительно точки М3","Геометрия")
REGISTERPROPERTY(v_Strelka,MSRZ1,"СРЕЗ1","Тип среза пред","Геометрия")
REGISTERPROPERTY(v_Strelka,MSRZ2,"СРЕЗ2","Тип среза плюс","Геометрия")
REGISTERPROPERTY(v_Strelka,MSRZ3,"СРЕЗ3","Тип среза минус","Геометрия")
REGISTERPROPERTY(v_Strelka,STIK1,"СТЫК1","Тип стыка пред","Стыки")
REGISTERPROPERTY(v_Strelka,STIK2,"СТЫК2","Тип стыка плюс","Стыки")
REGISTERPROPERTY(v_Strelka,STIK3,"СТЫК3","Тип стыка минус","Стыки")
REGISTERPROPERTY(v_Strelka,INVERSPOL,"ИНВЕРСИЯ ПОЛОЖЕНИЯ","Плюс сверху","Геометрия")
REGISTERPROPERTY(v_Strelka,SHOWPLUSPOL,"ПОКАЗЫВАТЬ ПЛЮС.ПОЛ.","Отображать штрих плюсового положения","Геометрия")
REGISTERPROPERTY(v_Strelka,PODSV_MARSH_M1,"М_ПОДСВЕТКА_МИН","Подсветка маршрута по минусу(кол-во стрелок)","")



v_Strelka::v_Strelka(v_Base *parent) :
    v_RC(parent)
{

    FM[1]=QPointF(10,0);
    FM[2]=QPointF(10,0);
    FM[3]=QPointF(5,5);
    FMD[3]=QPointF(5,0);
    FSTIK[0]=MVP_Enums::TStikType::stik_line;FSTIK[1]=MVP_Enums::TStikType::stik_line;FSTIK[2]=MVP_Enums::TStikType::stik_line;
    FINVERSPOL=false;
    FSHOWPLUSPOL=false;
    font=QFont("Times", 12);
    resetStates();
    calculateGeometry();
}

void v_Strelka::setModelObject(BaseObject *B)
{
    v_RC::setModelObject(B);
}

void v_Strelka::resetStates()
{
    v_RC::resetStates();
    FSTATE_POL=MVP_Enums::TStrelPol::pol_plus;
    FSTATE_BLOCK=false;
}


QPainterPath v_Strelka::shape() const
{
    return v_RC::shape();
    //    QPainterPath path;
    //    path.addPolygon(FULLPL);
    //    path.addRect(boundingRect());

    //    if (pol==m_RC::pol_plus){
    //        path.addPolygon(PLUSPL[0]);path.closeSubpath();
    //        path.addPolygon(PLUSPL[1]);path.closeSubpath();
    //    } else
    //    if (pol==m_RC::pol_minus){
    //        path.addPolygon(MINUSPL[0]);path.closeSubpath();
    //        path.addPolygon(MINUSPL[1]);path.closeSubpath();
    //    } else {
    //        path.addPolygon(FULLPL);
    //    }
    //    return path;
}

QMap<int,QPointF> v_Strelka::getMarkersInfo() const
{
    QMap<int,QPointF> m=v_RC::getMarkersInfo();

    m.insert(5,M[3]);
    if (!FMD[3].isNull())
        m.insert(6,MD[3]);
    return m;

}


void v_Strelka::moveMarker(int markerId, QPointF xyItem)
{
    v_RC::moveMarker(markerId, xyItem);
    QPointF gP=point2grid(xyItem);
    if (FINVERTDRAWX)
        gP=QPFunction::reflectPx(gP,0);
    if (FINVERTDRAWY)
        gP=QPFunction::reflectPy(gP,0);
    QPointF newM;
    QPointF oldM;
    switch (markerId){

    case 5:
        oldM=QPointF(FM[0].x()+FM[3].x(),FM[0].y()-FM[3].y());
        newM=QPointF(FM[3].x()-oldM.x()+gP.x(),FM[3].y()-gP.y()+oldM.y());
        setM3(newM);
        break            ;
    case 6:
        oldM=QPointF(FM[0].x()+FM[3].x()+FMD[3].x(),FM[0].y()-FM[3].y()+FMD[3].y());
        newM=QPointF(FMD[3].x()-oldM.x()+gP.x(),FMD[3].y()+gP.y()-oldM.y());
        setMD3(newM);
        break;
    }

}


void v_Strelka::calculateGeometry()
{
    assign_M_MD_S_MT();

    calculateP123();

    setTextBoundRect();

    qreal dd=S;

    QPolygonF PP[4];
    PP[1]=QPFunction::makerect(M[0],M[1],S+dd);
    PP[2]=QPFunction::makerect(M[0],M[2],S+dd);
    PP[3]=QPFunction::makerect(M[0],M[3],S+dd);

    z13_1=QPFunction::intersectionPointHard(PP[1].at(2),PP[1].at(3),  P[3].at(0),P[3].at(1));
    z13_2=QPFunction::intersectionPointHard(PP[1].at(2),PP[1].at(3),  P[3].at(2),P[3].at(3));

    z12_1=QPFunction::intersectionPointHard(PP[1].at(0),PP[1].at(1),  P[2].at(2),P[2].at(3));
    z12_2=QPFunction::intersectionPointHard(PP[1].at(0),PP[1].at(1),  P[2].at(0),P[2].at(1));

    z32_1=QPFunction::intersectionPointHard(PP[3].at(2),PP[3].at(3),  P[2].at(0),P[2].at(1));
    z32_2=QPFunction::intersectionPointHard(PP[3].at(2),PP[3].at(3),  P[2].at(2),P[2].at(3));
    z23_1=QPFunction::intersectionPointHard(PP[2].at(0),PP[2].at(1),  P[3].at(2),P[3].at(3));
    z23_2=QPFunction::intersectionPointHard(PP[2].at(0),PP[2].at(1),  P[3].at(0),P[3].at(1));

    // выбираем лучший срез
    QLineF srez1=QLineF(z13_1,z13_2);
    QLineF srez2=QLineF(z23_1,z23_2);
    QPointF ZZ1=z13_1;
    QPointF ZZ2=z13_2;
    if ((ZZ1.isNull())||(ZZ2.isNull())||(srez1.length()>srez2.length())){
        ZZ1=z23_2;
        ZZ2=z23_1;
    }
    QLineF srez3=QLineF(z12_1,z12_2);
    QLineF srez4=QLineF(z32_1,z32_2);
    QPointF ZZ3=z12_2;
    QPointF ZZ4=z12_1;
    if ((ZZ3.isNull())||(ZZ4.isNull())||(srez3.length()>srez4.length())){
        ZZ3=z32_1;
        ZZ4=z32_2;
    }
    // пересечения!!!
    Z2=QPFunction::intersectionPoint(P[1].at(0),P[1].at(1),  P[3].at(2),P[3].at(3));
    r13_1=QPFunction::intersectionPoint(P[1].at(2),P[1].at(3),  P[3].at(0),P[3].at(1));
    r32_1=QPFunction::intersectionPoint(P[3].at(2),P[3].at(3),  P[2].at(0),P[2].at(1));
    r21_1=QPFunction::intersectionPoint(P[2].at(2),P[2].at(3),  P[1].at(0),P[1].at(1));

    // Целая
    {

        FULLPL.clear();
        vFULLPL_CL.clear();
        if (bPD1) FULLPL <<PD[1].at(2);
        FULLPL << rD1_1 << r13_1 ;
        if ((!bPD3)&&(FSTIK[2]==MVP_Enums::TStikType::stik_0)) vFULLPL_CL.push_back(FULLPL.size());
        FULLPL << rD3_1;
        if (bPD3) {
            if (FSTIK[2]==MVP_Enums::TStikType::stik_0) vFULLPL_CL.push_back(FULLPL.size());
            FULLPL <<PD[3].at(1);FULLPL <<PD[3].at(2);
        }
        FULLPL << rD3_2 << r32_1;
        if ((!bPD2)&&(FSTIK[1]==MVP_Enums::TStikType::stik_0)) vFULLPL_CL.push_back(FULLPL.size());
        FULLPL << rD2_1;
        if (bPD2) {
            if (FSTIK[1]==MVP_Enums::TStikType::stik_0) vFULLPL_CL.push_back(FULLPL.size());
            FULLPL <<PD[2].at(1);FULLPL <<PD[2].at(2);
        }
        FULLPL << rD2_2 << r21_1;
        if ((!bPD1)&&(FSTIK[0]==MVP_Enums::TStikType::stik_0)) vFULLPL_CL.push_back(FULLPL.size());
        FULLPL << rD1_2;
        if (bPD1) {
            if (FSTIK[0]==MVP_Enums::TStikType::stik_0) vFULLPL_CL.push_back(FULLPL.size());
            FULLPL <<PD[1].at(1);
        }
    }
    // плюс это P2+P3
    {
        PLUSPL[0].clear();vPLUSPL_CL[0].clear();
        PLUSPL[1].clear();vPLUSPL_CL[1].clear();
        if (bPD1) PLUSPL[0] <<PD[1].at(2);
        PLUSPL[0] << rD1_1 << Z1 ;
        if ((!bPD2)&&(FSTIK[1]==MVP_Enums::TStikType::stik_0)) vPLUSPL_CL[0].push_back(PLUSPL[0].size());
        PLUSPL[0] << rD2_1;
        if (bPD2) {
            if (FSTIK[1]==MVP_Enums::TStikType::stik_0) vPLUSPL_CL[0].push_back(PLUSPL[0].size());
            PLUSPL[0] <<PD[2].at(1);PLUSPL[0] <<PD[2].at(2);
        }
        PLUSPL[0] << rD2_2 << r21_1;
        if ((!bPD1)&&(FSTIK[0]==MVP_Enums::TStikType::stik_0)) vPLUSPL_CL[0].push_back(PLUSPL[0].size());
        PLUSPL[0] << rD1_2;
        if (bPD1) {
            if (FSTIK[0]==MVP_Enums::TStikType::stik_0) vPLUSPL_CL[0].push_back(PLUSPL[0].size());
            PLUSPL[0] <<PD[1].at(1);
        }

        PLUSPL[1] << ZZ1 << rD3_1;
        if ((!bPD3)&&(FSTIK[2]==MVP_Enums::TStikType::stik_0)) vPLUSPL_CL[1].push_back(PLUSPL[1].size());
        if (bPD3) {
            if (FSTIK[2]==MVP_Enums::TStikType::stik_0) vPLUSPL_CL[1].push_back(FULLPL.size());
            PLUSPL[1] <<PD[3].at(1);PLUSPL[1] <<PD[3].at(2);
        }
        PLUSPL[1] << rD3_2 << ZZ2;
    }

    // минус это P3+P2
    {
        MINUSPL[0].clear();vMINUSPL_CL[0].clear();
        MINUSPL[1].clear();vMINUSPL_CL[1].clear();
        if (bPD1) MINUSPL[0] <<PD[1].at(2);
        MINUSPL[0] << rD1_1 << r13_1 ;
        if ((!bPD3)&&(FSTIK[2]==MVP_Enums::TStikType::stik_0)) vMINUSPL_CL[0].push_back(MINUSPL[0].size());
        MINUSPL[0] << rD3_1;
        if (bPD3) {
            if (FSTIK[2]==MVP_Enums::TStikType::stik_0) vMINUSPL_CL[0].push_back(MINUSPL[0].size());
            MINUSPL[0] <<PD[3].at(1);MINUSPL[0] <<PD[3].at(2);
        }
        MINUSPL[0] << rD3_2 << Z2;
        if ((!bPD1)&&(FSTIK[0]==MVP_Enums::TStikType::stik_0)) vMINUSPL_CL[0].push_back(MINUSPL[0].size());
        MINUSPL[0] << rD1_2;
        if (bPD1) {
            if (FSTIK[0]==MVP_Enums::TStikType::stik_0) vMINUSPL_CL[0].push_back(MINUSPL[0].size());
            MINUSPL[0] <<PD[1].at(1);
        }

        MINUSPL[1] << ZZ3 << rD2_1;
        if ((!bPD2)&&(FSTIK[1]==MVP_Enums::TStikType::stik_0)) vMINUSPL_CL[1].push_back(MINUSPL[1].size());
        if (bPD2) {
            if (FSTIK[1]==MVP_Enums::TStikType::stik_0) vMINUSPL_CL[1].push_back(FULLPL.size());
            MINUSPL[1] <<PD[2].at(1);MINUSPL[1] <<PD[2].at(2);
        }
        MINUSPL[1] << rD2_2 << ZZ4;
    }

    // указатель плюса
    qreal sh_ukaz=4; // ширина указателя
    // указатель плюса на минусе
    QPointF uM0=QPFunction::pointOnLine(ZZ2,P[3].at(2),dd/2);
    QPointF uM1=QPFunction::pointOnLine(ZZ2,P[3].at(2),dd/2+sh_ukaz*4);
    QPointF uM3=QPFunction::pointOnPerpendikular(uM0,P[3].at(2),sh_ukaz);
    QPointF uM2=QPFunction::pointOnPerpendikular(uM1,P[3].at(2),sh_ukaz);
    UKPOLMINUSPL.clear();
    UKPOLMINUSPL << uM0 << uM1 << uM2 << uM3;
    // указатель плюса на плюсе
    QPointF uP0=QPFunction::pointOnLine(ZZ3,P[2].at(1),dd/2);
    QPointF uP1=QPFunction::pointOnLine(ZZ3,P[2].at(1),dd/2+sh_ukaz*4);
    QPointF uP3=QPFunction::pointOnPerpendikular(uP0,P[2].at(1),-sh_ukaz);
    QPointF uP2=QPFunction::pointOnPerpendikular(uP1,P[2].at(1),-sh_ukaz);
    UKPOLPLUSPL.clear();
    UKPOLPLUSPL << uP0 << uP1 << uP2 << uP3;





    // переворот
    if (FINVERTDRAWX){
        QPFunction::reflectPLx(&FULLPL,0);
        QPFunction::reflectPLx(&PLUSPL[0],0);
        QPFunction::reflectPLx(&PLUSPL[1],0);
        QPFunction::reflectPLx(&MINUSPL[0],0);
        QPFunction::reflectPLx(&MINUSPL[1],0);

        QPFunction::reflectPLx(&UKPOLMINUSPL,0);
        QPFunction::reflectPLx(&UKPOLPLUSPL,0);

        for (int i=0;i<4;i++){
            M[i]=QPFunction::reflectPx(M[i],0);
            MD[i]=QPFunction::reflectPx(MD[i],0);
            MDs[i]=QPFunction::reflectPx(MDs[i],0);
        }
        for (int i=0;i<3;i++)
            LSTIK[i]=QPFunction::reflectLx(LSTIK[i],0);
    }
    if (FINVERTDRAWY){
        QPFunction::reflectPLy(&FULLPL,0);
        QPFunction::reflectPLy(&PLUSPL[0],0);
        QPFunction::reflectPLy(&PLUSPL[1],0);
        QPFunction::reflectPLy(&MINUSPL[0],0);
        QPFunction::reflectPLy(&MINUSPL[1],0);

        QPFunction::reflectPLy(&UKPOLMINUSPL,0);
        QPFunction::reflectPLy(&UKPOLPLUSPL,0);

        for (int i=0;i<4;i++){
            M[i]=QPFunction::reflectPy(M[i],0);
            MD[i]=QPFunction::reflectPy(MD[i],0);
            MDs[i]=QPFunction::reflectPy(MDs[i],0);
        }
        for (int i=0;i<3;i++)
            LSTIK[i]=QPFunction::reflectLy(LSTIK[i],0);
    }

    {
        QPolygonF P=FULLPL;
        P=FULLPL.united(boundrect_text);
        P=P.united(ZablokR);
        _boundingRect=P.boundingRect();
        _shape=QPainterPath();
        _shape.addPolygon(FULLPL);
    }

}

QPolygonF v_Strelka::centreLine() const
{
    MVP_Enums::TStrelPol _pol=getDrawPol();

    QPolygonF P;
    if (_pol==MVP_Enums::TStrelPol::pol_minus){
        if (!MD[1].isNull()) P<<MDs[1];
        P << M[1] << M[0] <<M[3];
        if (!MD[3].isNull()) P<<MDs[3];

    } else {
        if (!MD[1].isNull()) P<<MDs[1];
        P << M[1] << M[0] <<M[2];
        if (!MD[2].isNull()) P<<MDs[2];
    }
    //    if (FINVERTDRAWX){
    //        QPolygonF Pinvert;
    //        for (int i=P.size()-1;i>=0;i--)
    //            Pinvert << P[i];
    //        return Pinvert;
    //    }

    QPFunction::sortByX(P,FDIRECT_D0);

    return P;
}


MVP_Enums::TStrelPol v_Strelka::getDrawPol() const
{
    MVP_Enums::TStrelPol pol=FSTATE_POL;
    if (FINVERSPOL){
        if (FSTATE_POL==MVP_Enums::TStrelPol::pol_plus)  pol=MVP_Enums::TStrelPol::pol_minus; else
            if (FSTATE_POL==MVP_Enums::TStrelPol::pol_minus) pol=MVP_Enums::TStrelPol::pol_plus;
    }
    return pol;
}

void v_Strelka::d_paint(QPainter *painter, const QStyleOptionGraphicsItem * )
{
    QColor clrB1;
    QColor clrB2;
    QColor clrP1=Qt::black;
    QColor clrP2=Qt::black;
    MVP_Enums::TStrelPol pol=getDrawPol();
    QColor clrBusy=busyColor();
    clrB1=clrBusy;
    clrB2=getColor(Color_rc_free);
    if (FSTATE_IN_BLOCK_AREA) {
        clrB1=clrB1.darker(120);
        clrB2=clrB2.darker(120);
    } else {
        if ((FSTATE_BLOCK)&&(FPODSV_BLOK)){
            if (pol==MVP_Enums::TStrelPol::pol_plus) clrB2=clrB2.darker(120);
            if (pol==MVP_Enums::TStrelPol::pol_minus) clrB1=clrB1.darker(120);
            if (pol==MVP_Enums::TStrelPol::pol_w) {clrB1=clrB1.darker(120);clrB2=clrB2.darker(120);}
        }
    }


    showStrelka(painter,pol,clrB1,clrB2,clrP1,clrP2);
    if (FSTATE_SHTRIH_PLUS) showSHTRIH(painter,MVP_Enums::TStrelPol::pol_plus);
    if (FSTATE_SHTRIH_MINUS) showSHTRIH(painter,MVP_Enums::TStrelPol::pol_minus);
    showTEXT(painter);

    if (ISEDITORMODE){
        painter->setPen(QPen(QBrush(Qt::black),1,Qt::SolidLine));
        QPolygonF PC=centreLine();
        QPolygonF PPP=QPFunction::makeFullArrowPolygonFromPolyline(PC,2);
        painter->drawPolyline(PPP);
    }

}

void v_Strelka::showStrelka(QPainter *painter, MVP_Enums::TStrelPol pol,
                            QColor clrB1, QColor clrB2, QColor clrP1, QColor clrP2)
{
    painter->setPen(QPen(QBrush(Qt::black),1,Qt::SolidLine));
    QPen pen1=painter->pen();pen1.setJoinStyle(Qt::MiterJoin);pen1.setCapStyle(Qt::RoundCap);
    pen1.setColor(clrP1);
    QPen pen2=painter->pen();pen1.setJoinStyle(Qt::MiterJoin);pen1.setCapStyle(Qt::RoundCap);
    pen1.setColor(clrP2);
    QBrush brush1(clrB1);
    QBrush brush2(clrB2);

    if (pol==MVP_Enums::TStrelPol::pol_plus){
        painter->setPen(pen1);painter->setBrush(brush1);
        drawColored0PolygonF(painter,PLUSPL[0],vPLUSPL_CL[0]);
        painter->setPen(pen2);painter->setBrush(brush2);
        drawColored0PolygonF(painter,PLUSPL[1],vPLUSPL_CL[1]);



    } else
        if (pol==MVP_Enums::TStrelPol::pol_minus){
            painter->setPen(pen1);painter->setBrush(brush1);
            drawColored0PolygonF(painter,MINUSPL[0],vMINUSPL_CL[0]);
            painter->setPen(pen2);painter->setBrush(brush2);
            drawColored0PolygonF(painter,MINUSPL[1],vMINUSPL_CL[1]);
        } else {
            painter->setPen(pen1);painter->setBrush(brush1);
            drawColored0PolygonF(painter,FULLPL,vFULLPL_CL);
        }

    painter->setPen(QPen(QBrush(Qt::white),1,Qt::SolidLine));
    for (int i=0;i<3;i++)
        if (FSTIK[i]>1)
            showStik(painter,LSTIK[i],FSTIK[i]);

    if (FSHOWPLUSPOL)
        showUkPolPlus(painter);
}

void v_Strelka::showUkPolPlus(QPainter *painter)
{
    painter->setPen(QPen(QBrush(Qt::black),1,Qt::SolidLine));
    painter->setBrush(QBrush(Qt::black));
    if (!FINVERSPOL)  painter->drawPolygon(UKPOLPLUSPL); else
        painter->drawPolygon(UKPOLMINUSPL);
}

void v_Strelka::setTextBoundRect()
{
    font.setPointSize(textH);
    QFontMetrics fm(font);
    boundrect_text=fm.boundingRect(text);
    qreal W=boundrect_text.width()/2;
    qreal H=boundrect_text.height()/2;

    qreal dd=2;
    QPointF mt=QPointF(0,0);
    switch(FTEXTPOS){
    case MVP_Enums::TTextPos::tp_Custom:         mt=grid2point(FTEXTM);break;
    case MVP_Enums::TTextPos::tp_HLeftVTop:      mt=QPointF(r13_1.x()-dd-W, r13_1.y()-dd-H);break;
    case MVP_Enums::TTextPos::tp_HCenterVTop:    mt=QPointF(M[0].x()     ,  M[0].y()-S-dd-H);break;
    case MVP_Enums::TTextPos::tp_HRightVTop:     mt=QPointF(UKPOLPLUSPL[3].x()+W, UKPOLPLUSPL[3].y()-1-H);break;
    case MVP_Enums::TTextPos::tp_HLeftVCenter:   mt=QPointF(M[0].x()-S-dd-W,M[0].y()       );break;
    case MVP_Enums::TTextPos::tp_HCenterVCenter: mt=QPointF(M[0].x()     ,  M[0].y()       );break;
    case MVP_Enums::TTextPos::tp_HRightVCenter:  mt=QPointF(M[0].x()+S+dd+W,M[0].y()       );break;
    case MVP_Enums::TTextPos::tp_HLeftVBottom:   mt=QPointF(M[0].x()-S-dd-W,M[0].y()+S+dd+H);break;
    case MVP_Enums::TTextPos::tp_HCenterVBottom: mt=QPointF(M[0].x()     ,  M[0].y()+S+dd+H);break;
    case MVP_Enums::TTextPos::tp_HRightVBottom:  mt=QPointF(M[0].x()+S+dd+W,M[0].y()+S+dd+H);break;
    }
    // переворот
    if (FINVERTDRAWX) mt=QPFunction::reflectPx(mt,0);
    if (FINVERTDRAWY) mt=QPFunction::reflectPy(mt,0);
    boundrect_text.moveCenter(mt);
}

void v_Strelka::updateStatesFromModel()
{
    v_RC::updateStatesFromModel();
}

void v_Strelka::updateSTATE_IN_MARSH()
{
    bool ism=false;
    if (FPODSV_MARSH_M1==0){
        v_RC::updateSTATE_IN_MARSH();
        return;
    }
    m_RC *rc=qobject_cast<m_RC *>(modelObject());
    if (rc){
        if  (rc->STATE_POL()==MVP_Enums::pol_plus){
            v_RC::updateSTATE_IN_MARSH();
            return;
        }
        int d,nstr;
        if (FPODSV_MARSH_M1>0){
            d=0;nstr=FPODSV_MARSH_M1;
        } else {
            d=1;nstr=-FPODSV_MARSH_M1;
        }
        int cntstr=0;
        QList<m_RC *> lrc=m_RC::goGrafpolcfb(rc, d);
        for (int i=0;i<lrc.size();i++){
            if ((lrc[i]->getNextCount(d)>1)||(lrc[i]->getNextCount(1-d)>1)) cntstr++;
            if (cntstr>=nstr){
                ism=true;
                break;
            }
        }
    }
    setSTATE_IN_MARSH(ism);
}


void v_Strelka::showSHTRIH(QPainter *painter, MVP_Enums::TStrelPol pol)
{
    if ((pol==MVP_Enums::TStrelPol::pol_plus)||((FINVERSPOL)&&(pol==MVP_Enums::TStrelPol::pol_minus))){
        QBrush brush3(Qt::NoBrush);
        brush3.setStyle(Qt::DiagCrossPattern);
        painter->setPen(Qt::NoPen);painter->setBrush(brush3);
        drawColored0PolygonF(painter,PLUSPL[0],vPLUSPL_CL[0]);
    }
    if ((pol==MVP_Enums::TStrelPol::pol_minus)||((FINVERSPOL)&&(pol==MVP_Enums::TStrelPol::pol_plus))){
        QBrush brush3(Qt::NoBrush);
        brush3.setStyle(Qt::DiagCrossPattern);
        painter->setPen(Qt::NoPen);painter->setBrush(brush3);
        drawColored0PolygonF(painter,MINUSPL[0],vMINUSPL_CL[0]);
    }
}


void v_Strelka::showTEXT(QPainter *painter)
{
    MVP_Enums::TStrelPol realpol=FSTATE_POL;
    QColor clrText;
    if (realpol==MVP_Enums::TStrelPol::pol_plus){
        clrText=getColor(Color_strel_text_plus);
    } else if (realpol==MVP_Enums::TStrelPol::pol_minus){
        clrText=getColor(Color_strel_text_minus);
    } else {
        clrText=getColor(Color_strel_text_w);
    }

    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(clrText),0,Qt::SolidLine));
    int flags=Qt::AlignCenter;
    painter->setFont(font);
    painter->drawText(boundrect_text,flags,text);

}


void v_Strelka::drawBackground(QPainter *painter)
{
    v_Base::drawBackground(painter);
    if (FSTATE_BLOCK)
        showZABLOK(painter,true);
}
