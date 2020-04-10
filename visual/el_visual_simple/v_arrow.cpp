#include "v_arrow.h"
#include "qpfunction.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_Arrow,"Cтрела","VISUAL Простые элементы")



v_Arrow::v_Arrow(v_Base *parent) : v_Base(parent)
{
    M[0]=QPointF(0,0);
    FSH=10;
    FARR_LENGTH=0;
    FARR_WIDTH=0;
    FARR_0=false;
    FARR_1=true;
    FPEN_COLOR=Qt::black;
    FPEN_STYLE=Qt::SolidLine;
    FBRUSH_STYLE=Qt::SolidPattern;
    FBRUSH_ANIMATE_SPEED=1000;
    animateT.restart();
    setPOINTS("10,10;5,-5");
    calculateGeometry();
}

void v_Arrow::setPOINTS(QString p)
{
    if (FPOINTS!=p){
        FPOINTS=p;
        pointsOffset.clear();
        QStringList sl=FPOINTS.split(";");
        for (int i=0;i<sl.size();i++){
            QStringList  slp=sl[i].split(",");
            if (slp.size()!=2) continue;
            QPointF P=QPointF(slp[0].toDouble(),slp[1].toDouble());
            pointsOffset.push_back(P);
        }
        doPropertyChanged();
    }
}

QPainterPath v_Arrow::shape() const
{
    QPainterPath path;
    path.addPolygon(arrowPolygon);
    return path;
}

void v_Arrow::d_paint(QPainter *painter, const QStyleOptionGraphicsItem * )
{
    
    QBrush B;


    if ((FBRUSH_ANIMATE)&&(proclen>0)){
            qreal xc=len_basePolyLine*(proclen)/100.;
            QPointF centr=QPFunction::pointOnPolygon(basePolyLine,xc);
            QRadialGradient gradient(centr, len_basePolyLine/10);
            gradient.setColorAt(1, FBRUSH_COLOR);
            gradient.setColorAt(0, FBRUSH_ANIMATE_COLOR);
            B=QBrush(gradient);
    } else {
        B=QBrush(FBRUSH_COLOR,FBRUSH_STYLE);
    }
    painter->setBrush(B);
    painter->setPen(QPen(QBrush(FPEN_COLOR),0,FPEN_STYLE));
    painter->drawPolygon(arrowPolygon);

}




void v_Arrow::calculateGeometry()
{
    basePolyLine.clear();
    arrowPolygon.clear();
    basePolyLine.push_back(M[0]);

    for (int i=0;i<pointsOffset.size();i++){
        basePolyLine.push_back(QPointF(basePolyLine.last().x()+pointsOffset[i].x()*gridDX(),basePolyLine.last().y()+pointsOffset[i].y()*gridDY()));
    }
    QPFunction::TArrowType at0=QPFunction::arrow_none;
    QPFunction::TArrowType at1=QPFunction::arrow_none;
    if (FARR_0) at0=QPFunction::arrow_normal;
    if (FARR_1) at1=QPFunction::arrow_normal;

    if (basePolyLine.size()>=1)
        arrowPolygon=QPFunction::makeFullArrowPolygonFromPolyline(basePolyLine,FSH,FARR_LENGTH,FARR_WIDTH,at0,at1);
    len_basePolyLine=QPFunction::lenPolyline(basePolyLine);

    {
        QRectF R=arrowPolygon.boundingRect();
        _boundingRect=R.adjusted(-1,-1,+1,+1);
    }
}

QMap<int,QPointF> v_Arrow::getMarkersInfo() const
{
    QMap<int,QPointF> m;
    for (int i=1;i<basePolyLine.size();i++){
        m.insert(i,basePolyLine[i]);
    }
    return m;
}

void v_Arrow::moveMarker(int markerId, QPointF xyItem)
{
    //QPointF gP=point2grid(xyItem);
    basePolyLine[markerId]=xyItem;
    QPointF dP=QPointF(basePolyLine[markerId].x()-basePolyLine[markerId-1].x(),basePolyLine[markerId].y()-basePolyLine[markerId-1].y());
    dP=point2grid(dP);
    pointsOffset[markerId-1]=dP;
    // вернем в строку
    QString p;
    for (int i=0;i<pointsOffset.size();i++){
        if (!p.isEmpty()) p=p+";";
        p=p+QString("%1,%2").arg(qreal2str(pointsOffset[i].x())).arg(qreal2str(pointsOffset[i].y()));
    }
    setPOINTS(p);
    update();

}

void v_Arrow::advance(int phase)
{
    if (!FBRUSH_ANIMATE) return;
    qreal ms_period=FBRUSH_ANIMATE_SPEED;
    if (phase==0){
        int ms=animateT.elapsed();
        proclen=ms/ms_period*100.;
        if (ms>=ms_period) animateT.restart();
        if (old_proclen!=proclen) {
            old_proclen=proclen;
            update();
        }
    }
}

QString v_Arrow::qreal2str(qreal r)
{
    if ((int)r==r) return QString::number((int)r);
    return QString::number((double)r,'g',2);
}
