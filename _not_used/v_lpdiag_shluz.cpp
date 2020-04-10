#include "v_lpdiag_shluz.h"

#include "qpfunction.h"
#include "v_rootlpdiag.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_LPDiag_Shluz,"ШЛЮЗ","Диагностика ЛП")


v_LPDiag_Shluz::v_LPDiag_Shluz(v_Base *parent):
    v_LPDiag(parent)
{
    FNUM=0;

}

QRectF v_LPDiag_Shluz::boundingRect() const
{
    QPolygonF P=QPolygonF(lpR);
    //P=P.united(boundrect_text);
    return P.boundingRect();
}

void v_LPDiag_Shluz::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    painter->setPen(QPen(QBrush(Qt::black),1,Qt::SolidLine));
    painter->setBrush(QBrush(Qt::lightGray));
    painter->drawRect(lpR);
    QString addrtext=QString("Ш%1").arg(FNUM+1);

//    // имя
    // аддресс
    QFont font("Times", lpR.height()/4-1);
    font.setPointSizeF(FTEXTADDRHEIGHT);
    painter->setFont(font);
    painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));
    QRectF RT=painter->fontMetrics().boundingRect(addrtext);
    int flags=Qt::AlignCenter;
    RT.moveCenter(QPointF(0,0));
    painter->drawText(RT,flags,addrtext,&boundrect_text);


//    painter->setBrush(QBrush(Qt::green));
//    for (int i=0;i<2;i++){
//        QList<QPolygonF> l=getInOutPolygons(i,out);
//        for (int j=0;j<l.size();j++){
//            painter->drawEllipse(l[j].back(),3,3);

//        }
//    }
//    painter->setPen(Qt::NoPen);;
    painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));
    QSizeF sznamelink=QSizeF(lpR.width()/4, lpR.height()/4);
    font.setPointSizeF(FTEXTADDRHEIGHT-2);
    painter->setFont(font);
    for (int i=0;i<2;i++){
        QRectF RT=QRectF(QPointF(0,0),sznamelink);
        if (i==0) RT.moveCenter(QPointF( lpR.width()/4 ,-lpR.height()/4));
        if (i==1) RT.moveCenter(QPointF( lpR.width()/4 , lpR.height()/4));
        QString namelink=getNameLink(i);
        int flags=Qt::AlignCenter;
        painter->drawRect(RT);
        painter->drawText(RT,flags,namelink);
    }

}

void v_LPDiag_Shluz::calculateGeometry()
{
    addr_krug=FADDR_KRUG;
    QSizeF szr=QSizeF(w_lp(),h_lp()/2);
    lpR=QRectF(QPointF(-szr.width()/2.,-szr.height()/2.),szr);
    if (rootLPDiag()!=0)
        rootLPDiag()->updateLines(FADDR_KRUG);
}

QString v_LPDiag_Shluz::getNameLink(int n)
{
    if (n==0) return "1";
    if (n==1) return "2";
    return "";
}




int v_LPDiag_Shluz::isAddrExists(int addr)
{
    if (FNUM==0){
        if (addr==0x41) return 1;
        if (addr==0x81) return 0;
    }
    if (FNUM==1){
        if (addr==0x42) return 0;
        if (addr==0x82) return 1;
    }
    return -1;
}

QList<QPolygonF> v_LPDiag_Shluz::getInOutPolygons(int n, v_LPDiag::Tinout inout)
{
    qreal sx=10;

    QList<QPolygonF> lpoly;
    if (inout==in) return lpoly;
    QPolygonF poly1;
    poly1.clear();
    QPolygonF poly0;
    qreal yc=0+h_lp()/4;
    if (FNUM==0) yc=0+h_lp()/4; else yc=0-h_lp()/4;
    qreal yc0;
    qreal x0,x1;
    qreal W=w_lp();
    x0=W/2+1; x1=x0+sx;
    qreal sy_inout=-5;
    if (n==0){
        yc0=0+sy_inout;
    }
    if (n==1){
        yc0=0-sy_inout;x1+=5;
    }

    poly0.push_back(QPointF(x0,yc0));poly0.push_back(QPointF(x1,yc0));
    lpoly.push_back(poly0);
    for (int i=0;i<4;i++){
        poly1.clear();poly1 << poly0;
        poly1.push_back(QPointF(x1,YY(yc,0,i+1,inout)));lpoly.push_back(poly1);
    }
    for (int i=0;i<4;i++){
        poly1.clear();poly1 << poly0;
        poly1.push_back(QPointF(x1,YY(yc,1,i+1,inout)));lpoly.push_back(poly1);
    }
    return lpoly;
}

