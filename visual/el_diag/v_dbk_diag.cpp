#include "v_dbk_diag.h"

#include "mcolorpalette.h"

#include <qmath.h>

#include "qpfunction.h"
#include "mcolorpalette.h"
#include "mvp_system.h"
#include "mvp_objectfactory.h"
REGISTERELEMENT(v_DBK_Diag,"Состояние ДБК","VISUAL Диагностика")

v_DBK_Diag::v_DBK_Diag(v_Base *parent) : v_StaticText(parent)
{

    FSIGNAL_STATUS2.clear();
    FSIGNAL_STATUS3.clear();
    FSIGNAL_STATUS4.clear();
    FDBK_ADDR=0;
    FSIZEALLIGNRECT=QSizeF(8,6);
    memset(&DBK485PacketExchange,0,sizeof(DBK485PacketExchange));
    FTO_SEC=5;
    packetRecivedTime=QDateTime();

    for (int i=0;i<4;i++) _DatagramPacket2_tick[i]=0;

    FSIGNAL_STATUS1.clear();
    FSIGNAL_STATUS1.setChanelType(TDBK485PacketExchange::packetType());
    FSIGNAL_STATUS1.setChanelName(TDBK485PacketExchange::packetName(TGMC_ComplectNumber::gmc_1a));
    FSIGNAL_STATUS1.setChanelOffset(0);
    FSIGNAL_STATUS2.clear();
    FSIGNAL_STATUS2.setChanelType(TDBK485PacketExchange::packetType());
    FSIGNAL_STATUS2.setChanelName(TDBK485PacketExchange::packetName(TGMC_ComplectNumber::gmc_1b));
    FSIGNAL_STATUS2.setChanelOffset(0);
    FSIGNAL_STATUS3.clear();
    FSIGNAL_STATUS3.setChanelType(TDBK485PacketExchange::packetType());
    FSIGNAL_STATUS3.setChanelName(TDBK485PacketExchange::packetName(TGMC_ComplectNumber::gmc_2a));
    FSIGNAL_STATUS3.setChanelOffset(0);
    FSIGNAL_STATUS4.clear();
    FSIGNAL_STATUS4.setChanelType(TDBK485PacketExchange::packetType());
    FSIGNAL_STATUS4.setChanelName(TDBK485PacketExchange::packetName(TGMC_ComplectNumber::gmc_2b));
    FSIGNAL_STATUS4.setChanelOffset(0);

    SIGNAL_STATUS[0]=&FSIGNAL_STATUS1;
    SIGNAL_STATUS[1]=&FSIGNAL_STATUS2;
    SIGNAL_STATUS[2]=&FSIGNAL_STATUS3;
    SIGNAL_STATUS[3]=&FSIGNAL_STATUS4;
    resetStates();
    calculateGeometry();

}

void v_DBK_Diag::resetStates()
{
    FSTATE_33=false;
}


void v_DBK_Diag::d_paint(QPainter *painter, const QStyleOptionGraphicsItem * )
{
    QBrush B;
    QString text=objectName();
    allign_rect.moveCenter(M[0]);


    if ((ISEDITORMODE)||(!FSTATE_33)){
        B=QBrush(Qt::green);
    } else {
        B=QBrush(Qt::gray);
    }
    painter->setBrush(B);
    painter->setPen(Qt::black);
    painter->drawRect(allign_rect);

    bool err[8];
    for (int i=0;i<8;i++) err[i]=false;
    quint8 b=DBK485PacketExchange.packet_from.diagnostic;
    if (b==0xFF){
        //ok=true;
    } else{
        err[0]=b & 0x01;
        err[1]=b & 0x02;
        err[2]=b & 0x04;
        err[3]=b & 0x08;
        err[4]=b & 0x10;
        err[5]=b & 0x20;
        err[6]=b & 0x40;
        err[7]=b & 0x80;
    }
    qreal sx=allign_rect.width()/8.;
    qreal sy=allign_rect.height()/4.;
    // аддркс тип
    if (FSH==0) FFONT.setPointSizeF(12); else FFONT.setPointSizeF(FSH);
    painter->setFont(FFONT);
    painter->setPen(QPen(QBrush(FCOLOR_TEXT),1,Qt::SolidLine));
    int flags=Qt::AlignTop | Qt::AlignHCenter |Qt::TextWordWrap;
    painter->drawText(allign_rect,flags,text,&boundrect_text);

    // цикловая
    qreal errsx=allign_rect.width()/2/8;

    FFONT.setPointSizeF(errsx*2-4);
    painter->setFont(FFONT);

    QRectF tickR=QRectF(0,0,sx*4,sy);
    tickR.moveCenter(QPointF(0-sx*2+2,0+sy/2));
    flags=Qt::AlignLeft;
    painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));
    painter->drawText(tickR,flags,QString("%1").arg(DBK485PacketExchange.packet_from.tick));

    // ошибки
    painter->setPen(Qt::black);
    for (int i=0;i<8;i++){
        if (!err[i]) painter->setBrush(QBrush(Qt::NoBrush)); else
            painter->setBrush(QBrush(Qt::red));
        QRectF R1=QRectF(0,0,errsx,errsx);
        R1.moveCenter(QPointF(0-errsx/2+errsx*i,sy/2));
        painter->drawRect(R1);
    }
    // данные
    painter->setPen(Qt::black);
    qreal datasx=allign_rect.width()/7;
    FFONT.setPointSizeF(datasx-4);
    painter->setFont(FFONT);
    for (int i=0;i<6;i++){
        QString stD;
        if (DBK485PacketExchange.packet_from.data[i]!=0){
            painter->setBrush(QBrush(Qt::yellow));
            stD=QString("%1").arg(DBK485PacketExchange.packet_from.data[i],2,16,QLatin1Char('0'));
        } else {
            painter->setBrush(QBrush(Qt::gray));
            stD=" ";
        }
        QRectF R1=QRectF(0,0,datasx,datasx);
        R1.moveBottomLeft(QPointF(allign_rect.left()+datasx/2+datasx*i,allign_rect.bottom()-2));
        painter->drawRect(R1);
        painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));
        flags=Qt::AlignCenter;
        painter->drawText(R1,flags,stD);
    }




}

void v_DBK_Diag::updateAfterLoad()
{
    v_StaticText::updateAfterLoad();
    for (int i=0;i<4;i++)
        SIGNAL_STATUS[i]->getBuffer()->setMsecPeriodLive(FTO_SEC*1000);

}

void v_DBK_Diag::updateStates()
{
    // находим нужный канал
    bool bonline=false;

    for (int i=0;i<4;i++){
        if (SIGNAL_STATUS[i]->getBuffer()) continue;
        if (SIGNAL_STATUS[i]->is33())  continue;
        auto DBK485PacketExchange=reinterpret_cast<const TDBK485PacketExchange *>(SIGNAL_STATUS[i]->value_data(sizeof(TDBK485PacketExchange)));
        if (DBK485PacketExchange->online==0) continue;
        if (DBK485PacketExchange->packet_from.addrDBK!=FDBK_ADDR) continue;
        if (_DatagramPacket2_tick[i]==SIGNAL_STATUS[i]->getBuffer()->getTick())continue;
        bonline=true;
        _DatagramPacket2_tick[i]=SIGNAL_STATUS[i]->getBuffer()->getTick();
        packetRecivedTime=SIGNAL_STATUS[i]->getBuffer()->dataRecivedTime();
        //memcpy(&DBK485PacketExchange,chanel->getChanelData(),sizeof(DBK485PacketExchange)); ??

        break;
    }
    setSTATE_33(!bonline);
}



