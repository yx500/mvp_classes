#include "v_dbg_dbk_arrow.h"
#include "gmc_dbk.h"

#include <qmath.h>

#include "qpfunction.h"
#include "mcolorpalette.h"
#include "mvp_system.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_dbg_DBK_Arrow,"Стрела состояния канала ДБК","VISUAL Диагностика")

//void v_dbg_DBK_Arrow::setINOUT(v_dbg_DBK_Arrow::TDBKInOut p)
//{
//    FINOUT=p;setChanel();
//    if (!FDEFCOLORS) return;
//    if (FINOUT==inDBK) {
//        setZValue(1);
//        setBRUSH_COLOR(Qt::yellow);
//        setBRUSH_ANIMATE_COLOR(Qt::red);
//        setPEN_STYLE(Qt::NoPen);
//    }else {
//        setZValue(0);
//        setBRUSH_COLOR(Qt::green);
//        setBRUSH_ANIMATE_COLOR(Qt::yellow);
//        setPEN_STYLE(Qt::SolidLine);
//        setPEN_COLOR(Qt::black);
//    }
//}

void v_dbg_DBK_Arrow::setCOMPLECTNUMBER(int p)
{
    FCOMPLECTNUMBER=p;
    FSIGNAL_STATUS.setChanelType(TDBK485PacketExchange::packetType());
    FSIGNAL_STATUS.setChanelName(TDBK485PacketExchange::packetName(FCOMPLECTNUMBER));
    FSIGNAL_STATUS.setChanelOffset(0);
}

v_dbg_DBK_Arrow::v_dbg_DBK_Arrow(v_Base *parent) : v_Arrow(parent)
{
    FSIGNAL_STATUS.clear();
    FCOLOR_33=getColor(Qt::gray);
    FBRUSH_COLOR=Qt::green;
    FBRUSH_ANIMATE_COLOR=Qt::yellow;
    FBRUSH_ANIMATE=true;
    FSH=2;
    FDBK_ADDR=0;
    FCOMPLECTNUMBER=0;
    FDBK_LINE=0;
    FDEFCOLORS=true;
    FTO_SEC=5;
    _DatagramPacket2_tick=0;
    packetRecivedTime=QDateTime();
    resetStates();
}

void v_dbg_DBK_Arrow::resetStates()
{
    FSTATE_33=false;
    FSTATE_COMPLECTNUMBER=0;
    FSTATE_DBKLINE=0;
    FSTATE_MOXA485ERROR=0;
    FSTATE_ONLINE=0;
}

void v_dbg_DBK_Arrow::updateAfterLoad()
{
    v_Arrow::updateAfterLoad();
    setCOMPLECTNUMBER(FCOMPLECTNUMBER);
    if (FSIGNAL_STATUS.getBuffer())FSIGNAL_STATUS.getBuffer()->setMsecPeriodLive(FTO_SEC*1000);
}

void v_dbg_DBK_Arrow::d_paint(QPainter *painter, const QStyleOptionGraphicsItem * )
{
    
    QBrush B;

    B=QBrush(Qt::gray,FBRUSH_STYLE);
    if (!FSTATE_33){
        if (!FSTATE_ONLINE){
            B=QBrush(Qt::lightGray,FBRUSH_STYLE);
        }else  {
            if (FSTATE_MOXA485ERROR) B=QBrush(Qt::red,FBRUSH_STYLE); else
                                     B=QBrush(Qt::green,FBRUSH_STYLE);
        }
    }

    /*if ((FBRUSH_ANIMATE)&&(proclen>0)){
        qreal xc=len_basePolyLine*(proclen)/100.;
        QPointF centr=QPFunction::pointOnPolygon(basePolyLine,xc);
        QRadialGradient gradient(centr, len_basePolyLine/10);
        gradient.setColorAt(1, FBRUSH_COLOR);
        gradient.setColorAt(0, FBRUSH_ANIMATE_COLOR);
        B=QBrush(gradient);
    }
    */

    painter->setBrush(B);
    painter->setPen(QPen(QBrush(FPEN_COLOR),0,FPEN_STYLE));
    painter->drawPolygon(arrowPolygon);

}
/*
void v_dbg_DBK_Arrow::advance(int phase)
{
    proclen=0;
    if ((!FBRUSH_ANIMATE)) return;
    if (chanel==0) return;
    if (!packetRecived) return;
    qreal ms_period=FBRUSH_ANIMATE_SPEED;
    int ms=animateT.elapsed();
    //    bool animateRestart=false;
    //    TChanelInformation I;
    //    chanel->getChanelInformation(&I);
    //    if (old_TICK!=I.tick[0]){
    //        old_TICK=I.tick[0];
    //        if (FINDICATOR!=obj){
    //            if (ms>ms_period) animateRestart=true;
    //        } else {
    //            TDBK485PacketExchange * DBK485PacketExchange=(TDBK485PacketExchange *)chanel->getChanelData();
    //            if (DBK485PacketExchange->Packet.data[FDBK_TU.data_number()]!=0)  animateRestart=true;
    //        }
    //    }

    if (phase==1){
        if (ms<=ms_period) proclen=ms/ms_period*100.; else proclen=0;

        if (old_proclen!=proclen) {
            old_proclen=proclen;
            update();
        }
        //        if ((FINOUT==inDBK)) {
        //            qreal op=0;
        //            if (proclen>0){
        //                op=qSin(M_PI*proclen/100.)*100;
        //            }
        //            setOpacity(op/100.);
        //        } else {
        //            setOpacity(1);
        //        }
    }
}
*/
void v_dbg_DBK_Arrow::updateStates()
{
    GtBuffer *chanel=FSIGNAL_STATUS.getBuffer();
    if ((chanel==nullptr)||(chanel->sost!=GtBuffer::_alive))  {
        setSTATE_33(true);
        setSTATE_COMPLECTNUMBER(0);
        setSTATE_DBKLINE(0);
        setSTATE_MOXA485ERROR(0);
        setSTATE_ONLINE(0);
        return;
    }
    auto DBK485PacketExchange=reinterpret_cast<const TDBK485PacketExchange *>( FSIGNAL_STATUS.value_data(sizeof(TDBK485PacketExchange)));
    if ((DBK485PacketExchange->complectNumber==FCOMPLECTNUMBER) &&
            (DBK485PacketExchange->dbkLine==FDBK_LINE)&&
            ((FDBK_ADDR==0)||(DBK485PacketExchange->packet_from.addrDBK==FDBK_ADDR)))
    {
        if (_DatagramPacket2_tick!=FSIGNAL_STATUS.getBuffer()->getTick()){
            _DatagramPacket2_tick=FSIGNAL_STATUS.getBuffer()->getTick();
            packetRecivedTime=FSIGNAL_STATUS.getBuffer()->dataRecivedTime();
        }

    }

    bool bonline=true;
    if (packetRecivedTime.msecsTo(QDateTime::currentDateTime())>FTO_SEC*1000) bonline=false;
    if (bonline) {
        setSTATE_33(false);
        setSTATE_COMPLECTNUMBER(DBK485PacketExchange->complectNumber);
        setSTATE_DBKLINE(DBK485PacketExchange->dbkLine);
        setSTATE_ONLINE(DBK485PacketExchange->online);
        setSTATE_MOXA485ERROR(DBK485PacketExchange->moxa485_err);

    } else {
        setSTATE_33(true);
    }
    if (FSTATE_ONLINE){
        if (FSTATE_MOXA485ERROR){
            setZValue(-10);
        } else {
            setZValue(0);
        }
    } else {
        setZValue(-10);
    }

}

void v_dbg_DBK_Arrow::calculateGeometry()
{
    //    qreal _FSH=FSH;
    //    if (FSTATE_MOXA485ERROR) {
    //        FSH=FSH*0.7;
    //        if (FSH==0) FSH=_FSH;
    //    }
    v_Arrow::calculateGeometry();
    //    FSH=_FSH;
}




