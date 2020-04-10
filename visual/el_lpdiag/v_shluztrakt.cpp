#include "v_shluztrakt.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_ShluzTrakt,"ШЛЮЗ ТРАКТ","Диагностика ЛП")

v_ShluzTrakt::v_ShluzTrakt(v_Base *parent) : v_LPDiag(parent)
{
    setNORDER(0);
}

void v_ShluzTrakt::setNORDER(int p)
{
    if (FNORDER!=p){
        FNORDER=p;
        if (p==0){
            FADDR00=0;
            FADDR01=0x41;
            FADDR10=0;
            FADDR11=0x81;
        } else {
            FADDR00=0;
            FADDR01=0x42;
            FADDR10=0;
            FADDR11=0x82;
        }
        addr[0]=FADDR00;
        addr[1]=FADDR01;
        addr[2]=FADDR10;
        addr[3]=FADDR11;
        emit propertyChanged(this);
    }
}

void v_ShluzTrakt::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));
    painter->setBrush(QBrush(Qt::lightGray));
    painter->drawRect(lpR);
    QString addrtext="0000";
    QString nametext="ЛП";
    addrtext=QString("%1").arg(ADDR(),4,16,QLatin1Char('0'));
    addrtext=addrtext.toUpper();
    nametext=objectName();

    // аддресс
    QFont font("Times");
    FTEXTADDRHEIGHT>0 ? font.setPointSizeF(FTEXTADDRHEIGHT):font.setPointSizeF(1);
    painter->setFont(font);
    painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));
    QRectF RT=painter->fontMetrics().boundingRect(addrtext);
    RT.adjust(-1,0,1,0);
    int flags=Qt::AlignCenter;
    RT.moveCenter(QPointF(0,0));
    painter->drawText(RT,flags,addrtext,&boundrect_text);
    // имя
    qreal namefontsz=FTEXTNAMEHEIGHT;
    if (FTEXTNAMEHEIGHT==0){
        namefontsz=12;
    }
    font.setPointSizeF(namefontsz);
    painter->setFont(font);
    RT=painter->fontMetrics().boundingRect(nametext);
    RT.adjust(-1,0,1,0);
    RT.moveCenter(QPointF(0,0));
    RT.moveBottom(-lpR.height()/2);
    painter->drawText(RT,flags,nametext,&boundrect_text);

    QSizeF sznamelink=QSizeF(lpR.width()/4, lpR.height()/4);
    FTEXTADDRHEIGHT-2>0 ? font.setPointSizeF(FTEXTADDRHEIGHT-2): font.setPointSizeF(1);
    painter->setFont(font);
    for (int i=0;i<4;i++){
        RT=QRectF(QPointF(0,0),sznamelink);
        if (i==0) RT.moveCenter(QPointF(-lpR.width()/4 ,-lpR.height()/4));
        if (i==1) RT.moveCenter(QPointF( lpR.width()/4 ,-lpR.height()/4));
        if (i==2) RT.moveCenter(QPointF(-lpR.width()/4 , lpR.height()/4));
        if (i==3) RT.moveCenter(QPointF( lpR.width()/4 , lpR.height()/4));
        QString namelink=getNameLink(i);
        int flags=Qt::AlignCenter;
        painter->drawText(RT,flags,namelink);
    }

}

void v_ShluzTrakt::calculateGeometry()
{


    QSizeF szr=QSizeF(w_lp(),h_lp());

    lpR=QRectF(QPointF(-szr.width()/2.,-szr.height()/2.),szr);
    if (boundrect_text.isEmpty()){
        qreal namefontsz=FTEXTNAMEHEIGHT;
        if (FTEXTNAMEHEIGHT==0)
            namefontsz=12;
        boundrect_text=QRectF(QPointF(-lpR.width()/2,-lpR.height()/2-namefontsz),QSizeF(namefontsz*10,namefontsz));
    }
    _boundingRect=lpR;
    _boundingRect=_boundingRect.united(boundrect_text);
}
