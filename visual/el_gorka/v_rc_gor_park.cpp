#include "v_rc_gor_park.h"

#include <math.h>
#include "qpfunction.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_RC_Gor_Park,"РЦ путь парка","VISUAL ГОРКА")

//const VisualGroupProperty_int v_RC_Gor_Park::groupProp_SHRCKZP("SHRCKZP","ШИРИНА РЦ ПУТЬ ПАРКА",0,10);
//const VisualGroupProperty_int v_RC_Gor_Park::groupProp_USL_VAGON("USL_VAGON","ВЫВОД В УВ",2,0);


v_RC_Gor_Park::v_RC_Gor_Park(v_Base *parent) : v_RC(parent)
{
    FSTIK[0]=MVP_Enums::TStikType::stik_no_line;
    FSTIK[1]=MVP_Enums::TStikType::stik_no_line;
    FSTIKW=1;
    FTEXT_SVOB_XY=QPointF(0,0);
    FTEXT_SVOB_HEIGHT=0;
    FUSL_VAGON=false;
    FSHRCKZP=10;
    FWIDTH=10;
    FMASHTAB_MAX_LEN=0;
    k_mashtab=1;
    width_abs=0;width_mashtab=0;
    isUSL_VAGON=false;
    addCOMMON_PROPERTY("SHRCKZP","SH_RC_KZP");
    addCOMMON_PROPERTY("USL_VAGON","USL_VAGON");
    animateK=0;
    font=QFont("Times",1);
    resetStates();
}

void v_RC_Gor_Park::resetStates()
{
    FSTATE_KZP_D=0;
    FSTATE_KZP_E=0;
    FSTATE_KZP_V=0;
    FSTATE_KZP_P=0;
}


void v_RC_Gor_Park::updateAfterLoad()
{
    v_RC::updateAfterLoad();

}

void v_RC_Gor_Park::calculateGeometry()
{
    isUSL_VAGON=FUSL_VAGON;

    if ((len_rc>0)&&(FMASHTAB_MAX_LEN>0)){
        k_mashtab=1.*len_rc/FMASHTAB_MAX_LEN;
    } else {
        k_mashtab=1;
    }
    width_abs=FWIDTH*gridDX();
    width_mashtab=width_abs*k_mashtab;

    M[0]=grid2point(FM[0]);
    M[1]=QPointF(FM[0].x()+width_mashtab,FM[0].y());

    S=FSHRCKZP;//viewProperty_int(groupProp_SHRCKZP,FSH);
    FULLPL.clear();
    vFULLPL_CL.clear();
    FULLPL << QPointF(0,-S) << QPointF(width_mashtab,-S) << QPointF(width_mashtab,S) << QPointF(0,S) ;

    int th=FTEXT_SVOB_HEIGHT*gridDY();
    if (FTEXT_SVOB_HEIGHT==0) th=S*2;
    int tw=th*4;
    text_sv_rect=QRectF(0,0,tw,th);
    text_sv_rect.moveCenter(QPointF(tw/2+2.,0));
    if ((2*S-2)>0)
        font.setPointSizeF(2*S-2);


    // переворот
    if (FINVERTDRAWX){
        QPFunction::reflectPLx(&FULLPL,0);
        text_sv_rect=QRectF(0,0,tw,th);
        text_sv_rect.moveCenter(QPointF(-tw/2-2.,0));

        for (int i=0;i<4;i++){
            M[i]=QPFunction::reflectPx(M[i],0);
        }
    }
    if (FINVERTDRAWY){
        QPFunction::reflectPLy(&FULLPL,0);
        for (int i=0;i<4;i++){
            M[i]=QPFunction::reflectPy(M[i],0);
        }
    }

    {
        QPolygonF P=FULLPL;
        P=FULLPL.united(text_sv_rect);
        _boundingRect=P.boundingRect();
        _boundingRect.adjust(-2,-2,2,2);
    }

}

QPolygonF v_RC_Gor_Park::centreLine() const
{
    QPolygonF P;
    P << M[1] << M[0];
    QPFunction::sortByX(P,FDIRECT_D0);
    return P;
}

void v_RC_Gor_Park::d_paint(QPainter *painter, const QStyleOptionGraphicsItem * )
{

    // определяем коффициент свободности
    qreal k_free=1;
    if ((FSTATE_KZP_D>=0) && (len_rc>0)) {
        if (FSTATE_KZP_D>=len_rc) k_free=1; else
            k_free=1.*FSTATE_KZP_D/len_rc;
    }

    QColor clrFree=getColor(Color_rc_free);
    QColor clrBusy=getColor(Color_rc_busy);

    if ((FSTATE_33)||(FSTATE_KZP_E!=0)) {
        clrFree=getColor(Color_state_33);
        clrBusy=getColor(Color_state_33);
    }

    if (!modelObject()){
        k_free=1;
        clrFree=Qt::gray;
        clrBusy=Qt::gray;
    }



    //if (FSTATE_KZP_V!=0) clrBusy=clrBusy.lighter();

    painter->setPen(QPen(QBrush(Qt::black),1,Qt::SolidLine));
    QPen pen1=painter->pen();
    pen1.setJoinStyle(Qt::MiterJoin);pen1.setCapStyle(Qt::RoundCap);
    painter->setPen(pen1);painter->setBrush(clrFree);
    painter->drawPolygon(FULLPL);

    if (k_free!=1){
        qreal k_busy=1-k_free;
        QRectF R=QRectF(M[0].x()+width_mashtab*k_free,M[0].y()-S,width_mashtab*k_busy,S*2);
        int dp=painter->pen().width();
        R.adjust(dp,dp,-dp,-dp);
        QPolygonF FULLPL_B=QPolygonF(R);
        if (FINVERTDRAWX) {
            QPFunction::reflectPLx(&FULLPL_B,0);
        }

        painter->setPen(QPen(QBrush(clrFree),0,Qt::SolidLine));

        if (STATE_KZP_V()!=0){
            QRectF R=FULLPL_B.boundingRect();
            qreal xx=R.left()+R.width()*animateK;
            if (FINVERTDRAWX) xx=R.left()+R.width()*(1.-animateK);
            qreal yy=R.center().y();
            qreal dd=R.width()/4;
            QLinearGradient  gradient(xx-dd,yy,xx+dd,yy);
            gradient.setColorAt(0, clrBusy.darker(110));
            gradient.setColorAt(1, clrBusy.lighter(110));
            gradient.setSpread(QGradient::RepeatSpread);
            QBrush B=QBrush(gradient);
            painter->setBrush(B);
            //painter->drawLine(xx-dd,yy-R.height()/2,xx-dd,yy+R.height()/2);
            //painter->drawLine(xx+dd,yy-R.height()/2,xx+dd,yy+R.height()/2);
        } else {
            painter->setBrush(QBrush(clrBusy));
        }
        painter->drawPolygon(FULLPL_B);





    }
    // техт
    int flags=Qt::AlignLeft | Qt::AlignVCenter;
    if (FINVERTDRAWX) flags=Qt::AlignRight | Qt::AlignVCenter;

    painter->setFont(font);
    //painter->drawRect(text_sv_rect);
    painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));
    painter->drawText(text_sv_rect,flags,valueString());


    //    QPolygonF cLine=centreLine();
    //    qreal kbusy=0;
    //    if (len>0) kbusy=1.*val/len;
    //    QPolygonF busyL=QPFunction::cutPolyline(cLine,kbusy,1);
    //    QPolygonF busyPL=QPFunction::makeFullPolygonFromPolyline(busyL,S);
    //    clrP=Qt::black;
    //    if (val<len){
    //    clrB=getColor(Color_rc_busy);
    //    painter->setPen(QPen(QBrush(clrP),0,Qt::SolidLine));
    //    painter->setBrush(QBrush(clrB));
    //    painter->drawPolygon(busyPL);
    //    }

    if (ISEDITORMODE){
        painter->setPen(QPen(QBrush(Qt::black),1,Qt::SolidLine));
        QPolygonF PC=centreLine();
        QPolygonF PPP=QPFunction::makeFullArrowPolygonFromPolyline(PC,2);
        painter->drawPolyline(PPP);

    }

}

void v_RC_Gor_Park::advance(int phase)
{
    if (!phase) return;
    if (FSTATE_KZP_V!=0){
        if (!animateT.isValid()) animateT.restart();
        long ms=animateT.elapsed();
        int min_peiod=1000;
        int max_peiod=5000;
        qreal V=fabs(FSTATE_KZP_V);
        if (V>20) V=20;
        int peiod=min_peiod+1.*(20-V)/20*(max_peiod-min_peiod);
        animateK=1.*(ms%peiod)/peiod;
        if (FSTATE_KZP_V<0) animateK=(1.-animateK);
        update();
    } else {
        v_RC::advance(phase);
    }
}

void v_RC_Gor_Park::updateStatesFromModel()
{
    v_RC::updateStatesFromModel();
    len_rc=modelPropertyInt("LEN",0);
}

QString v_RC_Gor_Park::valueString() const
{
    if (!modelObject()) return "";
    if (isUSL_VAGON)
        return QString::number((int)(FSTATE_KZP_D/14.5)); else
        return QString::number(FSTATE_KZP_D);
}

