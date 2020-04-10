#include "v_ip_text.h"


#include "qpfunction.h"
#include "mcolorpalette.h"
#include "mvp_system.h"
#include "mvp_objectfactory.h"
REGISTERELEMENT(v_IP_Text,"IP текст","VISUAL Диагностика")

void v_IP_Text::setIPSIGNAL(SignalDescription p)
{
    if (FIPSIGNAL!=p){
        FIPSIGNAL=p;
        FIPSIGNAL.getBuffer()->setMsecPeriodLive(FTO*1000);
        doPropertyChanged();
    }
}

v_IP_Text::v_IP_Text(v_Base *parent) : v_StaticText(parent)
{
    FISSHOWRECT=true;
    FSIZEALLIGNRECT=QSizeF(10,6);
    FIPSIGNAL.clear();
    FTO=1000;
    tick=0;
    tick_time.start();
}


void v_IP_Text::d_paint(QPainter *painter, const QStyleOptionGraphicsItem * )
{
    
    painter->setBrush(Qt::NoBrush);

    QString s="---";
    if (FIPSIGNAL.getBuffer()){
        s=QString::fromLocal8Bit(FIPSIGNAL.getBuffer()->A);
    }
    qreal procent=100;
    if (FTO>0)
        procent=1.*tick_time.elapsed()/(FTO*2);
    QColor c1=QColor(Qt::green);
    QColor c2=QColor(Qt::gray);

    // сравниваем аддресс с ип
    bool correctAddr=true;
    if (s.contains("addr")){
        QStringList sl=s.split(';');
        foreach(QString ss,sl){
            if (ss.contains("addr")){
                QString addrS=ss.section('=',1,1);
                QString ipS=FIPSIGNAL.chanelName().section(".",3,3);
                // последние цифры addr
                if (addrS.length()>0){
                    int addr = addrS.toInt();
                    addr += 200;
                    QString saddr = QString::number(addr);
                    if (ipS!=saddr)
                        correctAddr=false;
//                    if (addrS.length()<=ipS.length()){
//                        if (ipS.right(addrS.length())==addrS) bincorrectAddr=false;
//                    }
                }
            }
        }
    }

    if (!correctAddr) c1=QColor(Qt::red);

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

void v_IP_Text::advance(int phase)
{
    Q_UNUSED(phase);
    if (FIPSIGNAL.getBuffer()){
        quint64 newtick=FIPSIGNAL.getBuffer()->getTick();
        if (newtick!=tick){
            tick=newtick;
            tick_time.restart();
        }
    }
}

void v_IP_Text::updateAfterLoad()
{
    v_StaticText::updateAfterLoad();
    setIPSIGNAL(FIPSIGNAL);
    FIPSIGNAL.getBuffer()->setMsecPeriodLive(FTO*1000);

}

