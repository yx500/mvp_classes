#include "v_rc.h"

#include "mcolorpalette.h"

#include "qpfunction.h"
#include "baseobjecttools.h"
#include "m_rc.h"


#include "mvp_objectfactory.h"
REGISTERELEMENT(v_RC,"РЦ","VISUAL Стандартные РЦ")

REGISTERPROPERTY(v_RC,M1,"M1","Точка начала относительно цента","Геометрия")
REGISTERPROPERTY(v_RC,M2,"M2","Точка конца относительно цента","Геометрия")
REGISTERPROPERTY(v_RC,MD1,"MD1","Дополн.точка относительно точки М1","Геометрия")
REGISTERPROPERTY(v_RC,MD2,"MD1","Дополн.точка относительно точки М2","Геометрия")
REGISTERPROPERTY(v_RC,MSRZ1,"СРЕЗ1","Тип среза начала","Геометрия")
REGISTERPROPERTY(v_RC,MSRZ2,"СРЕЗ2","Тип среза конца","Геометрия")
REGISTERPROPERTY(v_RC,STIK1,"СТЫК1","Тип стыка","Стыки")
REGISTERPROPERTY(v_RC,STIK2,"СТЫК2","Тип стыка","Стыки")
REGISTERPROPERTY(v_RC,STIKW,"СТЫКШ","Ширина стыка","Стыки")
REGISTERPROPERTY(v_RC,SH,"ШИРИНА","Ширина РЦ","Геометрия")
REGISTERPROPERTY(v_RC,TEXT,"ТЕКСТ","Выводимф текст","Текст")
REGISTERPROPERTY(v_RC,TEXTM,"XY","Точка вывода текста","Текст")
REGISTERPROPERTY(v_RC,TEXTPOS,"ПОЗИЦИЯ","Выбор позиции текста","Текст")
REGISTERPROPERTY(v_RC,TEXTH,"ВЫСОТА","Высота шрифта","Текст")
REGISTERPROPERTY(v_RC,PODSV_MARSH,"М_ПОДСВЕТКА","Подсветка маршрута(кол-во стрелок)","Доп.подсветка")
REGISTERPROPERTY(v_RC,PODSV_BLOK,"БЛОК_ПОДСВЕТКА","Подсветка блок зоны(направление дл блока)","Доп.подсветка")

REGISTERCOLOR(v_RC,rc_busy,"РЦ занята",Qt::red);
REGISTERCOLOR(v_RC,rc_free,"РЦ свободна",Qt::white);
REGISTERCOLOR(v_RC,rc_in_marsh,"РЦ в маршруте",Qt::yellow);
REGISTERCOLOR(v_RC,rc_busy_unknow,"РЦ занятие не определено",Qt::gray);
REGISTERCOLOR(v_RC,rc_blok,"Блокировка",Qt::blue);
REGISTERCOLOR(v_RC,rc_blok_tu,"Блокировка ТУ",Qt::blue);


v_RC::v_RC(v_Base *parent) :
    v_Model(parent),metaProperty_STATE_BUSY()
{
    for (int i=0;i<4;i++){
        FM[i]=QPointF(0,0);
        FMD[i]=QPointF(0,0);
    }
    FM[1]=QPointF(10,0);
    FM[2]=QPointF(10,0);
    FSTIKW=1;
    FSTIK[0]=MVP_Enums::TStikType::stik_line;FSTIK[1]=MVP_Enums::TStikType::stik_line;
    FMSRZ[0]=MVP_Enums::TRCSrezType::srez_n;FMSRZ[1]=MVP_Enums::TRCSrezType::srez_n;FMSRZ[2]=MVP_Enums::TRCSrezType::srez_n;

    FSH=4;
    FTEXTH=12;
    FTEXTM=QPointF(0,0);
    FTEXTPOS=MVP_Enums::TTextPos::tp_HLeftVTop;
    FPODSV_MARSH=0;
    FPODSV_BLOK=-1;
    FSTATE_IN_BLOCK_AREA=0;
    FDIRECT_D0=MVP_Enums::left2right;
    FTRAIN_NUM_BY_END=false;
    ZablokR.setSize(QSize(0,0));
    addCOMMON_PROPERTY("SH","SH_RC");
    addCOMMON_PROPERTY("TEXTH","TEXTH_STREL");
    addCOMMON_PROPERTY("STIKW","STIKW");
    resetStates();
    calculateGeometry();
}

void v_RC::setModelObject(BaseObject *B)
{
    v_Model::setModelObject(B);
    metaProperty_STATE_BUSY=getMetaProperty(modelObject(),"STATE_BUSY");
}

void v_RC::resetStates()
{
    v_Model::resetStates();
    FSTATE_SHTRIH_PLUS=false;
    FSTATE_SHTRIH_MINUS=false;
    FSTATE_BUSY=MVP_Enums::TRCBusy::free;
    FSTATE_IN_MARSH=false;

}

void v_RC::setM(QPointF *F, QPointF p,TCheckP xc,TCheckP yc)
{
    if ((xc==tcp_onlyGreat0)&&(p.x()<=0)) return;
    if ((xc==tcp_onlyGreat_0)&&(p.x()<0)) return;
    if ((yc==tcp_onlyGreat0)&&(p.y()<=0)) return;
    if ((yc==tcp_onlyGreat_0)&&(p.y()<0)) return;
    if (*F!=p){
        *F=p;
        calculateGeometry();
        emit propertyChanged(this);
        emit geometryChange(this);
    }
}

void v_RC::setSTIK(int i, MVP_Enums::TStikType p)
{
    if (FSTIK[i]!=p){
        FSTIK[i]=p;
        calculateGeometry();
        emit propertyChanged(this);
        emit geometryChange(this);
    }
}

void v_RC::setMSRZ(int i, MVP_Enums::TRCSrezType p)
{
    if (p>MVP_Enums::TRCSrezType::srez_h) return;
    if (FMSRZ[i]!=p){
        FMSRZ[i]=p;
        calculateGeometry();
        emit propertyChanged(this);
        emit geometryChange(this);
    }
}


void v_RC::setSH(int p)
{
    if (p>=0){
        if (FSH!=p){
            FSH=p;
            doPropertyChanged();
        }
    }
}

void v_RC::setTEXT(QString p)
{
    if (Ftext!=p){
        Ftext=p;
        text=Ftext;
        //        if (Ftext.contains("$")){
        //            text=modelPropertyesStr(Ftext);
        //        }
        calculateGeometry();
        emit propertyChanged(this);
        emit geometryChange(this);
    }
}


QMap<int,QPointF> v_RC::getMarkersInfo() const
{
    QMap<int,QPointF> m;
    m.insert(1,M[1]);
    m.insert(2,M[2]);
    if (!FMD[1].isNull()) m.insert(3,MD[1]);
    if (!FMD[2].isNull()) m.insert(4,MD[2]);
    return m;
}

void v_RC::moveMarker(int markerId, QPointF xyItem)
{
    QPointF gP=point2grid(xyItem);
    if (FINVERTDRAWX)
        gP=QPFunction::reflectPx(gP,0);
    if (FINVERTDRAWY)
        gP=QPFunction::reflectPy(gP,0);
    QPointF newM;
    QPointF oldM;
    switch (markerId){
    case 1:
        oldM=QPointF(FM[0].x()-FM[1].x(),FM[0].y()+FM[1].y());
        newM=QPointF(FM[1].x()+oldM.x()-gP.x(),FM[1].y()+gP.y()-oldM.y());
        setM1(newM);
        break;
    case 2:
        oldM=QPointF(FM[0].x()+FM[2].x(),FM[0].y()+FM[2].y());
        newM=QPointF(FM[2].x()-oldM.x()+gP.x(),FM[2].y()+gP.y()-oldM.y());
        setM2(newM);
        break;

    case 3:
        oldM=QPointF(FM[0].x()-FM[1].x()-FMD[1].x(),FM[0].y()+FM[1].y()+FMD[1].y());
        newM=QPointF(FMD[1].x()+oldM.x()-gP.x(),FMD[1].y()+gP.y()-oldM.y());
        setMD1(newM);
        break;
    case 4:
        oldM=QPointF(FM[0].x()+FM[2].x()+FMD[2].x(),FM[0].y()+FM[2].y()+FMD[2].y());
        newM=QPointF(FMD[2].x()-oldM.x()+gP.x(),FMD[2].y()+gP.y()-oldM.y());
        setMD2(newM);
        break;

    }

}

void v_RC::updateStates()
{
    v_Model::updateStates();
    // так как подсветка зависит от других
    if (FPODSV_BLOK!=0) updateSTATE_IN_BLOCK_AREA();
}



void v_RC::assign_M_MD_S_MT()
{
    S=FSH;//viewProperty_int(groupProp_SHRC,FSH);
    textH=FTEXTH;//viewProperty_int(groupProp_TEXTH_STREL,FTEXTH);
    stik_width=FSTIKW;//viewProperty_int(groupProp_STIKW,FSTIKW);

    // для уобства
    for (int i=0;i<4;i++){
        M[i]=grid2point(FM[i]);
        MD[i]=grid2point(FMD[i]);
    }
}



void v_RC::calculateP123()
{
    M[1]=QPointF(M[0].x()-M[1].x(),M[0].y()+M[1].y());
    M[2]=QPointF(M[0].x()+M[2].x(),M[0].y()+M[2].y());
    M[3]=QPointF(M[0].x()+M[3].x(),M[0].y()-M[3].y());

    bPD1=false;
    bPD2=false;
    bPD3=false;

    if (!FMD[1].isNull()){
        P[1]=QPFunction::makerect(QPFunction::pointOnLine(M[0],M[1],-S),QPFunction::pointOnLine(M[1],M[0],-S),S);
        bPD1=true;
        MD[1]=QPointF(M[1].x()-MD[1].x(),M[1].y()+MD[1].y());
        MDs[1]=MD[1];
        if (FSTIK[0]>0) {
            MDs[1]=QPFunction::pointOnLine(MD[1],M[1],StikD(FSTIK[0]));
            LSTIK[0]=QLineF(QPFunction::pointOnPerpendikular(MD[1],M[1],-S),
                    QPFunction::pointOnPerpendikular(MD[1],M[1],+S));
        }
        PD[1]=QPFunction::makerect(QPFunction::pointOnLine(M[1],MD[1],-S),MDs[1],S);
        rD1_1=QPFunction::intersectionPoint(P[1].at(2),P[1].at(3),  PD[1].at(2),PD[1].at(3));
        rD1_2=QPFunction::intersectionPoint(P[1].at(1),P[1].at(0),  PD[1].at(1),PD[1].at(0));
    } else {
        MD[1]=QPointF();
        QPointF M1s=M[1];
        if (FSTIK[0]>0) M1s=QPFunction::pointOnLine(M[1],M[0],StikD(FSTIK[0]));
        P[1]=QPFunction::makerect(QPFunction::pointOnLine(M[0],M[1],-S),QPFunction::pointOnLine(M1s,M[0],0),S);
        switch (FMSRZ[0]) {
        case MVP_Enums::TRCSrezType::srez_n:
        {
            QPolygonF P1=QPFunction::makerect(QPFunction::pointOnLine(M[0],M[1],-S),QPFunction::pointOnLine(M1s,M[0],0),S);
            rD1_1=P1.at(2);
            rD1_2=P1.at(1);
            LSTIK[0]=QLineF(QPFunction::pointOnPerpendikular(M[1],M[0],-S),
                    QPFunction::pointOnPerpendikular(M[1],M[0],+S));
        }
            break;
        case MVP_Enums::TRCSrezType::srez_v:
            rD1_1=QPFunction::intersectionPoint(P[1].at(2),P[1].at(3),  QPointF(M1s.x(),M1s.y()-S),QPointF(M1s.x(),M1s.y()+S));
            rD1_2=QPFunction::intersectionPoint(P[1].at(1),P[1].at(0),  QPointF(M1s.x(),M1s.y()-S),QPointF(M1s.x(),M1s.y()+S));
            LSTIK[0]=QLineF(QPFunction::intersectionPoint(P[1].at(2),P[1].at(3),  QPointF(M[1].x(),M[1].y()-S),QPointF(M[1].x(),M[1].y()+S)),
                    QPFunction::intersectionPoint(P[1].at(1),P[1].at(0),  QPointF(M[1].x(),M[1].y()-S),QPointF(M[1].x(),M[1].y()+S)));
            break;
        case MVP_Enums::TRCSrezType::srez_h:
            rD1_1=QPFunction::intersectionPoint(P[1].at(2),P[1].at(3),  QPointF(M1s.x()-S,M1s.y()),QPointF(M1s.x()+S,M1s.y()));
            rD1_2=QPFunction::intersectionPoint(P[1].at(1),P[1].at(0),  QPointF(M1s.x()-S,M1s.y()),QPointF(M1s.x()+S,M1s.y()));
            LSTIK[0]=QLineF(QPFunction::intersectionPoint(P[1].at(2),P[1].at(3),  QPointF(M[1].x()-S,M[1].y()),QPointF(M[1].x()+S,M[1].y())),
                    QPFunction::intersectionPoint(P[1].at(1),P[1].at(0),  QPointF(M[1].x()-S,M[1].y()),QPointF(M[1].x()+S,M[1].y())));
            break;
        }
    }
    if (!FMD[2].isNull()){
        bPD2=true;
        P[2]=QPFunction::makerect(QPFunction::pointOnLine(M[0],M[2],-S),QPFunction::pointOnLine(M[2],M[0],-S),S);
        MD[2]=QPointF(M[2].x()+MD[2].x(),M[2].y()+MD[2].y());
        MDs[2]=MD[2];
        if (FSTIK[1]>0) {
            MDs[2]=QPFunction::pointOnLine(MD[2],M[2],StikD(FSTIK[1]));
            LSTIK[1]=QLineF(QPFunction::pointOnPerpendikular(MD[2],M[2],-S),
                    QPFunction::pointOnPerpendikular(MD[2],M[2],+S));
        }
        PD[2]=QPFunction::makerect(QPFunction::pointOnLine(M[2],MD[2],-S),MDs[2],S);
        rD2_1=QPFunction::intersectionPoint(P[2].at(0),P[2].at(1),  PD[2].at(0),PD[2].at(1));
        rD2_2=QPFunction::intersectionPoint(P[2].at(3),P[2].at(2),  PD[2].at(3),PD[2].at(2));
    } else{
        MD[2]=QPointF();
        QPointF M2s=M[2];
        if (FSTIK[1]>0) M2s=QPFunction::pointOnLine(M[2],M[0],StikD(FSTIK[1]));
        P[2]=QPFunction::makerect(QPFunction::pointOnLine(M[0],M[2],-S),QPFunction::pointOnLine(M2s,M[0],0),S);
        switch (FMSRZ[1]) {
        case MVP_Enums::TRCSrezType::srez_n:
        {
            QPolygonF P2=QPFunction::makerect(QPFunction::pointOnLine(M[0],M[2],-S),QPFunction::pointOnLine(M2s,M[0],0),S);
            rD2_1=P2.at(1);
            rD2_2=P2.at(2);
            LSTIK[1]=QLineF(QPFunction::pointOnPerpendikular(M[2],M[0],-S),
                    QPFunction::pointOnPerpendikular(M[2],M[0],+S));
        }
            break;
        case MVP_Enums::TRCSrezType::srez_v:
            rD2_1=QPFunction::intersectionPoint(P[2].at(0),P[2].at(1),  QPointF(M2s.x(),M2s.y()-S),QPointF(M2s.x(),M2s.y()+S));
            rD2_2=QPFunction::intersectionPoint(P[2].at(3),P[2].at(2),  QPointF(M2s.x(),M2s.y()-S),QPointF(M2s.x(),M2s.y()+S));
            LSTIK[1]=QLineF(QPFunction::intersectionPoint(P[2].at(2),P[2].at(3),  QPointF(M[2].x(),M[2].y()-S),QPointF(M[2].x(),M[2].y()+S)),
                    QPFunction::intersectionPoint(P[2].at(1),P[2].at(0),  QPointF(M[2].x(),M[2].y()-S),QPointF(M[2].x(),M[2].y()+S)));
            break;
        case MVP_Enums::TRCSrezType::srez_h:
            rD2_1=QPFunction::intersectionPoint(P[2].at(0),P[2].at(1),  QPointF(M2s.x()-S,M2s.y()),QPointF(M2s.x()+S,M2s.y()));
            rD2_2=QPFunction::intersectionPoint(P[2].at(3),P[2].at(2),  QPointF(M2s.x()-S,M2s.y()),QPointF(M2s.x()+S,M2s.y()));
            LSTIK[1]=QLineF(QPFunction::intersectionPoint(P[2].at(2),P[2].at(3),  QPointF(M[2].x()-S,M[2].y()),QPointF(M[2].x()+S,M[2].y())),
                    QPFunction::intersectionPoint(P[2].at(1),P[2].at(0),  QPointF(M[2].x()-S,M[2].y()),QPointF(M[2].x()+S,M[2].y())));
            break;
        }
    }
    if (!FMD[3].isNull()){
        bPD3=true;
        P[3]=QPFunction::makerect(QPFunction::pointOnLine(M[0],M[3],-S),QPFunction::pointOnLine(M[3],M[0],-S),S);
        MD[3]=QPointF(M[3].x()+MD[3].x(),M[3].y()+MD[3].y());
        MDs[3]=MD[3];
        if (FSTIK[2]>0) {
            MDs[3]=QPFunction::pointOnLine(MD[3],M[3],StikD(FSTIK[2]));
            LSTIK[2]=QLineF(QPFunction::pointOnPerpendikular(MD[3],M[3],-S),
                    QPFunction::pointOnPerpendikular(MD[3],M[3],+S));
        }
        PD[3]=QPFunction::makerect(QPFunction::pointOnLine(M[3],MD[3],-S),MDs[3],S);
        rD3_1=QPFunction::intersectionPoint(P[3].at(0),P[3].at(1),  PD[3].at(0),PD[3].at(1));
        rD3_2=QPFunction::intersectionPoint(P[3].at(3),P[3].at(2),  PD[3].at(3),PD[3].at(2));
    } else{
        MD[3]=QPointF();
        QPointF M3s=M[3];
        if (FSTIK[2]>0) M3s=QPFunction::pointOnLine(M[3],M[0],StikD(FSTIK[2]));

        P[3]=QPFunction::makerect(QPFunction::pointOnLine(M[0],M[3],-S),QPFunction::pointOnLine(M3s,M[0],0),S);

        switch (FMSRZ[2]) {
        case MVP_Enums::TRCSrezType::srez_n:
        {
            QPolygonF P3=QPFunction::makerect(QPFunction::pointOnLine(M[0],M[3],-S),QPFunction::pointOnLine(M3s,M[0],0),S);
            rD3_1=P3.at(1);
            rD3_2=P3.at(2);
            LSTIK[2]=QLineF(QPFunction::pointOnPerpendikular(M[3],M[0],-S),
                    QPFunction::pointOnPerpendikular(M[3],M[0],+S));
        }
            break;
        case MVP_Enums::TRCSrezType::srez_v:
            rD3_1=QPFunction::intersectionPoint(P[3].at(0),P[3].at(1),  QPointF(M3s.x(),M3s.y()-S),QPointF(M3s.x(),M3s.y()+S));
            rD3_2=QPFunction::intersectionPoint(P[3].at(3),P[3].at(2),  QPointF(M3s.x(),M3s.y()-S),QPointF(M3s.x(),M3s.y()+S));
            LSTIK[2]=QLineF(QPFunction::intersectionPoint(P[3].at(2),P[3].at(3),  QPointF(M[3].x(),M[3].y()-S),QPointF(M[3].x(),M[3].y()+S)),
                    QPFunction::intersectionPoint(P[3].at(1),P[3].at(0),  QPointF(M[3].x(),M[3].y()-S),QPointF(M[3].x(),M[3].y()+S)));
            break;
        case MVP_Enums::TRCSrezType::srez_h:
            rD3_1=QPFunction::intersectionPoint(P[3].at(0),P[3].at(1),  QPointF(M3s.x()-S,M3s.y()),QPointF(M3s.x()+S,M3s.y()));
            rD3_2=QPFunction::intersectionPoint(P[3].at(3),P[3].at(2),  QPointF(M3s.x()-S,M3s.y()),QPointF(M3s.x()+S,M3s.y()));
            LSTIK[2]=QLineF(QPFunction::intersectionPoint(P[3].at(2),P[3].at(3),  QPointF(M[3].x()-S,M[3].y()),QPointF(M[3].x()+S,M[3].y())),
                    QPFunction::intersectionPoint(P[3].at(1),P[3].at(0),  QPointF(M[3].x()-S,M[3].y()),QPointF(M[3].x()+S,M[3].y())));


            break;
        }
    }
    // пересечения!!!
    Z1=QPFunction::intersectionPoint(P[1].at(2),P[1].at(3),  P[2].at(0),P[2].at(1));
    r21_1=QPFunction::intersectionPoint(P[2].at(2),P[2].at(3),  P[1].at(0),P[1].at(1));
}



void v_RC::calculateGeometry()
{
    assign_M_MD_S_MT();

    calculateP123();

    // Целая
    {

        FULLPL.clear();
        vFULLPL_CL.clear();
        if (bPD1) FULLPL <<PD[1].at(2);
        FULLPL << rD1_1 << Z1 ;
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

    // переворот
    if (FINVERTDRAWX){
        QPFunction::reflectPLx(&FULLPL,0);

        for (int i=0;i<4;i++){
            M[i]=QPFunction::reflectPx(M[i],0);
            MD[i]=QPFunction::reflectPx(MD[i],0);
        }
        for (int i=0;i<3;i++)
            LSTIK[i]=QPFunction::reflectLx(LSTIK[i],0);
    }
    if (FINVERTDRAWY){
        QPFunction::reflectPLy(&FULLPL,0);
        for (int i=0;i<4;i++){
            M[i]=QPFunction::reflectPy(M[i],0);
            MD[i]=QPFunction::reflectPy(MD[i],0);
        }
        for (int i=0;i<3;i++)
            LSTIK[i]=QPFunction::reflectLy(LSTIK[i],0);
    }

    // текст
    setTextBoundRect();

    _boundingRect=FULLPL.boundingRect().united(boundrect_text);


}

QPolygonF v_RC::centreLine() const
{
    QPolygonF P;
    if (!MD[1].isNull()) P<<MDs[1];
    P << M[1] << M[0] <<M[2];
    if (!MD[2].isNull()) P<<MDs[2];
    QPFunction::sortByX(P,FDIRECT_D0);
    return P;
}


QColor v_RC::busyColor()
{
    QColor clr=getColor(Color_modelNotAccepted);
    if (FSTATE_33){
        clr=getColor(Color_state_33);
    } else {
        if (FSTATE_BUSY==MVP_Enums::TRCBusy::free) {
            clr=getColor(Color_rc_free);
            if (FSTATE_IN_MARSH) clr=getColor(Color_rc_in_marsh);
        }

        if (FSTATE_BUSY==MVP_Enums::TRCBusy::busy_not_accepted) clr=getColor(Color_modelNotAccepted);
        if (FSTATE_BUSY==MVP_Enums::TRCBusy::busy) clr=getColor(Color_rc_busy);
        if (FSTATE_BUSY==MVP_Enums::TRCBusy::busy_unknow) clr=getColor(Color_rc_busy_unknow);

    }

    return clr;

}

void v_RC::updateStatesFromModel()
{
    v_Model::updateStatesFromModel();
//    if (metaProperty_STATE_BUSY.isValid()){
//        QVariant V=metaProperty_STATE_BUSY.read(modelObject());
//        if (V.isValid()) setSTATE_BUSY(V.value<MVP_Enums::TRCBusy>()); else
//            setSTATE_BUSY(MVP_Enums::TRCBusy::busy_unknow);
//    } else {
//        setSTATE_BUSY(MVP_Enums::TRCBusy::busy_unknow);
//    }
    if (FPODSV_MARSH!=0) updateSTATE_IN_MARSH();
    if (FPODSV_BLOK!=0) updateSTATE_IN_BLOCK_AREA();
}

void v_RC::updateSTATE_IN_MARSH()
{
    bool ism=false;
    int d,nstr;
    if (FPODSV_MARSH>0){
        d=0;nstr=FPODSV_MARSH;
    } else {
        d=1;nstr=-FPODSV_MARSH;
    }
    m_RC *rc=qobject_cast<m_RC *>(modelObject());
    if (rc){
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

void v_RC::updateSTATE_IN_BLOCK_AREA()
{
    // Смотрим есть ли блокированные стрелки до нас
    bool inblock=false;

    if (FPODSV_BLOK!=0) {
        int d=0;
        if (FPODSV_BLOK<0) d=1;
        m_RC *rc=qobject_cast<m_RC *>(modelObject());
        int cnt=0;
        while(rc){
            // идем по положениям пока не встретим блок
            if (cnt>100) break;
            m_RC *rcNext=rc->getNextRC(d,rc->STATE_POL());
            if (!rcNext) break;
            if (rcNext->STATE_BLOCK()){
                if (rcNext->getNextRC(1-d,rcNext->STATE_POL())!=rc){
                    inblock=true;
                    break;
                }
            }
            rc=rcNext;
        }
    }
    setSTATE_IN_BLOCK_AREA(inblock);
}



void v_RC::d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    QColor clrP=Qt::black;
    QColor clrB=busyColor();
    if (FSTATE_IN_BLOCK_AREA) clrB=clrB.darker(120);
    showStrelka(painter,clrB,clrP);
    showTEXT(painter);
    if (option->levelOfDetailFromTransform(painter->worldTransform())>FLEVELDETAIL){
        QRectF R=boundingRect();
        QFont font("Times", 1);
        QPFunction::setFontSizeInRect(font,R,objectName(),5);
        painter->setFont(font);
        painter->setPen(Qt::gray);
        painter->drawText(R,Qt::AlignCenter,objectName());
    }

    if (ISEDITORMODE){
        painter->setPen(QPen(QBrush(Qt::black),1,Qt::SolidLine));
        QPolygonF PC=centreLine();
        QPolygonF PPP=QPFunction::makeFullArrowPolygonFromPolyline(PC,2);
        painter->drawPolyline(PPP);

    }

}



void v_RC::showStik(QPainter *painter, QLineF L, MVP_Enums::TStikType t)
{
    if (t==MVP_Enums::TStikType::stik_0) return;
    if (t==MVP_Enums::TStikType::stik_line){
        painter->setBrush(QBrush(Qt::NoBrush));
        painter->setPen(QPen(Qt::black, 1));
        painter->drawLine(L);
    }
    if (t==MVP_Enums::TStikType::stik_negab){
        painter->setBrush(QBrush(Qt::NoBrush));
        painter->setPen(QPen(Qt::black, 1));
        painter->drawLine(L);
        qreal r=L.length()/2.;
        painter->drawEllipse(QPointF((L.p1().x()+L.p2().x())/2.,(L.p1().y()+L.p2().y())/2.),r,r);
    }

}

qreal v_RC::StikD(int t)
{
    switch (t){
    case MVP_Enums::TStikType::stik_line:
    case MVP_Enums::TStikType::stik_no_line:
    case MVP_Enums::TStikType::stik_negab:
        return stik_width;
    }
    return 0;
}

void v_RC::drawColoredPolygonF(QPainter *painter, QPolygonF &P, QColor brushColor,QVector<QColor> &vC, qreal penWidth) const
{
    if (P.size()<3) return;
    if (P.size()!=vC.size()) return;

    // сначала рисуем все без пена
    painter->save();
    painter->setBrush(QBrush(brushColor));
    painter->setPen(QPen(QBrush(brushColor),penWidth));
    painter->drawPolygon(P);
    // а теперь с первой точки
    QColor cc=Qt::black;
    painter->setPen(QPen(QBrush(cc),penWidth));
    for (int i=0;i<P.size()-1;i++){
        if (vC[i]==brushColor) continue;
        if (cc!=vC[i]){
            cc=vC[i];
            painter->setPen(cc);
        }
        painter->drawLine(P.at(i),P.at(i+1));
    }
    // соеденияем последнюю с первой
    if (cc!=vC.last()){
        cc=vC.last();
        painter->setPen(cc);
    }
    painter->drawLine(P.last(),P.first());
    painter->restore();
}

void v_RC::drawColored0PolygonF(QPainter *painter, QPolygonF &P, QVector<int> &vCL) const
{
    if (P.size()<3) return;
    if (vCL.size()==0){
        painter->drawPolygon(P);
        return;
    }
    // сначала рисуем все брашой
    painter->save();
    painter->setPen(painter->brush().color());
    painter->setPen(Qt::NoPen);
    painter->drawPolygon(P);
    painter->restore();
    // а теперь с первой точки
    bool notDraw=false;
    for (int i=0;i<P.size()-1;i++){
        notDraw=false;
        for (int j=0;j<vCL.size();j++){
            if (i==vCL[j]) {notDraw=true;break;}
        }
        if (notDraw) continue;
        painter->drawLine(P.at(i),P.at(i+1));
    }
    // соеденияем последнюю с первой
    notDraw=false;
    for (int j=0;j<vCL.size();j++){
        if (P.size()-1==vCL[j]) {notDraw=true;break;}
    }
    if (!notDraw) painter->drawLine(P.last(),P.first());
}

void v_RC::setTextBoundRect()
{
    QFont font("Times", textH);
    QFontMetrics fm(font);
    boundrect_text=fm.boundingRect(text);
    qreal W=boundrect_text.width()/2;
    qreal H=boundrect_text.height()/2;

    qreal dd=2;
    QPointF mt=QPointF(0,0);
    switch(FTEXTPOS){
    case MVP_Enums::TTextPos::tp_Custom:         mt=grid2point(FTEXTM);break;
    case MVP_Enums::TTextPos::tp_HLeftVTop:      mt=QPointF(M[0].x()-S-dd-W,M[0].y()-S-dd-H);break;
    case MVP_Enums::TTextPos::tp_HCenterVTop:    mt=QPointF(M[0].x()     ,  M[0].y()-S-dd-H);break;
    case MVP_Enums::TTextPos::tp_HRightVTop:     mt=QPointF(M[0].x()+S+dd+W,M[0].y()-S-dd-H);break;
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

void v_RC::showTEXT(QPainter *painter)
{
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));
    QFont font("Times", textH);
    int flags=Qt::AlignCenter;
    painter->setFont(font);
    painter->drawText(boundrect_text,flags,text);
}

void v_RC::showZABLOK(QPainter *painter, bool tu)
{
    QColor clr=getColor(Color_rc_blok);
    if (tu) clr=getColor(Color_rc_blok_tu);
    qreal r=gridDX()*4;
    if (r<gridDY()*3) r=gridDY()*4;
    if (r<S*4) r=S*4;

    ZablokR=QRectF(0,0,r,r);
    ZablokR.moveCenter(M[0]);
    painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));
    painter->setBrush(clr);
    painter->drawRect(ZablokR);
}

void v_RC::showSHTRIH(QPainter *painter, MVP_Enums::TStrelPol pol)
{
    Q_UNUSED(pol);
    QBrush brush3(Qt::NoBrush);
    brush3.setStyle(Qt::DiagCrossPattern);
    painter->setPen(Qt::NoPen);painter->setBrush(brush3);
    drawColored0PolygonF(painter,FULLPL,vFULLPL_CL);
}

void v_RC::showStrelka(QPainter *painter, QColor clrB, QColor clrP)
{
    painter->setPen(QPen(QBrush(clrP),1,Qt::SolidLine));
    QPen pen1=painter->pen();
    pen1.setJoinStyle(Qt::MiterJoin);pen1.setCapStyle(Qt::RoundCap);
    painter->setPen(pen1);

    painter->setBrush(clrB);
    drawColored0PolygonF(painter,FULLPL,vFULLPL_CL);

    painter->setPen(QPen(QBrush(Qt::white),1,Qt::SolidLine));
    for (int i=0;i<2;i++)
        if (FSTIK[i]>1)
            showStik(painter,LSTIK[i],FSTIK[i]);
    if (FSTATE_SHTRIH_PLUS) showSHTRIH(painter,MVP_Enums::TStrelPol::pol_plus);

}




