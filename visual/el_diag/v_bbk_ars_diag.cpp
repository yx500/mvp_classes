#include "v_bbk_ars_diag.h"

#include "uart_packet.h"
#include <iostream>
#include <string>
#include <unistd.h>

#include "gtbuffer.h"
#include "qpfunction.h"
#include "mcolorpalette.h"

//#include <QProcess>
#include "mvp_system.h"
#include "mvp_objectfactory.h"
REGISTERELEMENT(v_BBK_ARS_Diag,"Диагностика ББК АРС","VISUAL Диагностика")


v_BBK_ARS_Diag::v_BBK_ARS_Diag(v_Base *parent):v_StaticText(parent)
{
    FISSHOWRECT=true;
    FSIZEALLIGNRECT=QSizeF(10,6);
    chanel_main=nullptr;
    chanel_debug=nullptr;
    FTO=1000;
    tick_debug=0;tick_main=0;
    tick_time.start();
    FIP.clear();
//    ping = new QProcess();
//    connect(ping, SIGNAL(readyReadStandardOutput()), SLOT(readResult()));
//    ping->start(QString("ping %1").arg("192.168.1.200"));

}

//void v_BBK_ARS_Diag::readResult()
//{
//    QProcess *ping = qobject_cast<QProcess *>(sender());
//    if (!ping)
//        return;
//    QString res = ping->readAllStandardOutput();
//    if (!res.contains('%'))
//        return;
//    const int percent = res.mid(res.indexOf('('), res.indexOf(')')).section('%', 0, 0).remove('(').toInt();
//    if (percent == 100) {
//        qDebug() << "host not found.";
//    } else {
//        qDebug() << "host found.";
//    }
//}

inline double TPVal(uint16_t w)
{
    return (!w || w == 0xffff) ?  0. : (1000000./70./w);
}
QString v_BBK_ARS_Diag::getText(bool &error)
{
    error=false;
    QString s="---";
    QDateTime t1,t2;
    if (chanel_debug)
        t1=chanel_debug->dataRecivedTime();
    if (chanel_main)
        t2=chanel_main->dataRecivedTime();
    if ((t1>t2) &&(chanel_debug)){
        s=QString::fromLocal8Bit(chanel_debug->A);
        // сравниваем аддресс с ип
        if (s.contains("addr")){
            QStringList sl=s.split(';');
            foreach(QString ss,sl){
                if (ss.contains("addr")){
                    QString addrS=ss.section('=',1,1);
                    QString ipS=FIP.section(".",3,3);
                    // последние цифры addr
                    if (addrS.length()>0){
                        int addr = addrS.toInt();
                        addr += 200;
                        QString saddr = QString::number(addr);
                        if (ipS!=saddr)
                            error=true;
    //                    if (addrS.length()<=ipS.length()){
    //                        if (ipS.right(addrS.length())==addrS) bincorrectAddr=false;
    //                    }
                    }
                }
            }
        }
    } else
        if (chanel_main) {
            const tSoftOut *op  = reinterpret_cast<const tSoftOut*>(chanel_main->A.data());
            if (op){
                static std::vector<char> buf(256,0);
                snprintf(buf.data(), buf.size(),
                         "addr=%u;"
                         "V=%4.1f  V2=%4.1f;st=0x%02hhx;rj=0x%02hhx;"
                         "cnf=0x%04hx;s=0x%02hhx;t=0x%02hhx;F=%4d;A=%6.1f"
                         , int(op->block8.b0_addr)
                         , TPVal(op->block8.w1_V), TPVal(op->block8.w3_V2), (op->block8.b5_st), (op->block8.b6_rj)
                         , op->conf, op->sigs, op->tp_out, op->F, op->A/10.
                         );
                s=QString(buf.data());
            }
        }
    return s;
}


void v_BBK_ARS_Diag::setIP(QString p)
{
    if (p!=FIP){
        FIP=p;
        chanel_main=SignalDescription::getBuffer(33,"bbb");
        chanel_debug=SignalDescription::getBuffer(9999,FIP);
        doPropertyChanged();
    }
}

void v_BBK_ARS_Diag::advance(int phase)
{
    Q_UNUSED(phase);
    if (chanel_debug){
        qlonglong newtick=chanel_debug->getTick();
        if (newtick!=tick_debug){
            tick_debug=newtick;
            tick_time.restart();
        }
    }
//    if (chanel_main){
//        TChanelInformation info;
//        chanel_main->getChanelInformation(QHostAddress(FIP),&info);
//        qlonglong newtick=info.tick[ssReal];
//        if (newtick!=tick_main){
//            tick_main=newtick;
//            tick_time.restart();
//        }
//    }
}

void v_BBK_ARS_Diag::updateAfterLoad()
{
    v_StaticText::updateAfterLoad();
    setIP(FIP);
}


void v_BBK_ARS_Diag::d_paint(QPainter *painter, const QStyleOptionGraphicsItem * )
{
    
    painter->setBrush(Qt::NoBrush);

    bool error;
    QString s=getText(error);

    qreal procent=100;
    if (FTO>0)
        procent=1.*tick_time.elapsed()/(FTO*2);
    QColor c1=QColor(Qt::green);
    QColor c2=QColor(Qt::gray);

    if (error) c1=QColor(Qt::red);

    QColor c=Color2Color(c1,c2,procent);
    painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));



    QBrush brush=QBrush(c);
    painter->setBrush(brush);
    painter->drawRect(allign_rect);



    s=s.replace(";","\r\n");
    FFONT.setPointSizeF(RTEXTH);
    painter->setFont(FFONT);
    painter->setPen(QPen(QBrush(FCOLOR_TEXT),1,Qt::SolidLine));
    int flags=FFLAGS;
    painter->drawText(allign_rect,flags,s,&boundrect_text);

}
