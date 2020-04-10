#include "v_lpdiag.h"
#include <QTimer>
#include "mvp_system.h"
#include "qpfunction.h"
#include "qnxdlp.h"
#include "visual_screen.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_LPDiag,"ЛП ТРАКТ","Диагностика ЛП")

const qreal lwidth=1;

qreal v_LPDiag::w_lp()
{
    qreal W=FLPSIZE.width();
    if (W==0){
        W=80;
    }
    return W;
}

qreal v_LPDiag::h_lp()
{
    qreal H=FLPSIZE.height();
    if (H==0){
        H=120;
    }
    return H;
}


void v_LPDiag::setCHANELNAME(QString p)
{

    if (FCHANELNAME!=p){
        FCHANELNAME=p;
        if (FCHANELNAME!=""){
            QString FCHANELNAME8=FCHANELNAME+"8";
            // инициализируем каналы
            chanels13[0]=SignalDescription::getBuffer(13,FCHANELNAME);  chanels13[0]->setMsecPeriodLive(150000);
            chanels13[1]=SignalDescription::getBuffer(13,FCHANELNAME8); chanels13[1]->setMsecPeriodLive(150000);
            chanels14[0]=SignalDescription::getBuffer(14,FCHANELNAME);  chanels14[0]->setMsecPeriodLive(150000);
            chanels14[1]=SignalDescription::getBuffer(14,FCHANELNAME8); chanels14[1]->setMsecPeriodLive(150000);
            chanels15[0]=SignalDescription::getBuffer(15,FCHANELNAME);  chanels15[0]->setMsecPeriodLive(150000);
            chanels15[1]=SignalDescription::getBuffer(15,FCHANELNAME8); chanels15[1]->setMsecPeriodLive(150000);
            emit propertyChanged(this);
        }
    }
}

void v_LPDiag::setNORDER(int p)
{
    if (FNORDER!=p){
        FNORDER=p;
        if (FNORDER>=0){
            int N=((0x00f0 & FNORDER*16)>> 3)+ 3;
            FADDR00=(0x40 | N);
            FADDR01=(0x80 | N);
            FADDR10=(0x40 | N)+1;
            FADDR11=(0x80 | N)+1;
            //        FADDR00=67+FNORDER*2;
            //        FADDR01=68+FNORDER*2;
            //        FADDR10=131+FNORDER*2;
            //        FADDR11=132+FNORDER*2;
            if (p==0){
                FADDR00=0x41;
                FADDR01=0x42;
                FADDR10=0x81;
                FADDR11=0x82;

            }
        }
        addr[0]=FADDR00;
        addr[1]=FADDR01;
        addr[2]=FADDR10;
        addr[3]=FADDR11;

        emit propertyChanged(this);
    }
}

v_LPDiag::v_LPDiag(v_Base *parent):
    v_Base(parent)
{
    chanels13[0]=nullptr;
    chanels13[1]=nullptr;
    chanels14[0]=nullptr;
    chanels14[1]=nullptr;
    chanels15[0]=nullptr;
    chanels15[1]=nullptr;
    FLPSIZE=QSizeF(80,120);
    FTEXTNAMEHEIGHT=0;
    FTEXTADDRHEIGHT=8;
    FPACKET_RECIVED15=0;
    FANIMATE=false;
    for (int i=0;i<4;i++){
        proclen[i]=0;
        addr_not_found[i]=false;
        addr_is_new[i]=0;
        last_addr[i]=0;
        last_addr_in[i]=0;
        arrows[0].clear();
    }
    M13_S[0]=0;M13_S[1]=0;
    boundrect_text=QRectF();

    //timerAnimateNewPacket = new QTimer();
    //timerAnimateNewAddr = new QTimer();
    timeAnimateNewPacket=QTime();
    timeAnimateNewAddr=QTime();

//    connect(timerAnimateNewPacket, SIGNAL(timeout()), this, SLOT(animateNewPacket()));
//    connect(timerAnimateNewAddr, SIGNAL(timeout()), this, SLOT(animateNewAddr()));
}

v_LPDiag::~v_LPDiag()
{
//    delete timerAnimateNewPacket;
//    delete timerAnimateNewAddr;
}

void v_LPDiag::updateStates()
{
    if (!FCHANELNAME.isEmpty()){
        auto  QMsg0=reinterpret_cast<tQMsg *>(chanels15[0]->A.data());
        //if (result==ISignalsManager::soChanelDie) setSTATE_33(true); else setSTATE_33(false);
        auto  KPMask0=reinterpret_cast<tKPMask *>(&QMsg0->LPY);

        auto  QMsg8=reinterpret_cast<tQMsg *>(chanels15[1]->A.data());
        auto  KPMask8=reinterpret_cast<tKPMask *>(&QMsg8->LPY);

        setSTATE_ADDR_IN00(KPMask0->AddrUz[0]);
        setSTATE_ADDR_IN01(KPMask0->AddrUz[1]);


        setSTATE_ADDR_IN10(KPMask8->AddrUz[0]);
        setSTATE_ADDR_IN11(KPMask8->AddrUz[1]);
        M13_S[0]=chanels13[0]->sost;
        M13_S[1]=chanels13[1]->sost;
        M15_S[0]=chanels15[0]->sost;
        M15_S[1]=chanels15[1]->sost;
    }




//    TChanelInformation I;
//    int _M13_S;
//    int _M15_S;
//    bool redraw=false;
//    QString FCHANELNAME,FCHANELNAME8;
//    if (mLP){
//        FCHANELNAME=mLP->CHANELNAME();
//        FCHANELNAME8=mLP->CHANELNAME()+"8";
//    }
//    for (int i=0;i<2;i++){
//        if (i==0) SManager->getChanelInformation(13,FCHANELNAME,&I);
//        if (i==1) SManager->getChanelInformation(13,FCHANELNAME8,&I);
//        if (I.tick[I.signalSource]==0) {
//            _M13_S=0;
//        } else {
//            if (I.alive) _M13_S=1; else _M13_S=-1;
//        }
//        if (i==0) SManager->getChanelInformation(15,FCHANELNAME,&I);
//        if (i==1) SManager->getChanelInformation(15,FCHANELNAME8,&I);
//        if (I.tick[I.signalSource]==0) {
//            _M15_S=0;
//        } else {
//            if (I.alive) _M15_S=1; else _M15_S=-1;
//        }
//        if (M13_S[i]!=_M13_S) redraw=true;
//        if (M15_S[i]!=_M15_S) redraw=true;
//        M13_S[i]=_M13_S;
//        M15_S[i]=_M15_S;
//    }
//    bool redraw=true;
//    if (redraw) {
//        calculateGeometry();
//        emit geometryChange(this);
//    }

}








QPainterPath v_LPDiag::shape() const
{
    QPainterPath P;
    P.addRect(lpR);
    return P;
}

void v_LPDiag::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));
        painter->setBrush(QBrush(Qt::lightGray));
        painter->drawRect(lpR);
        QString addrtext="0000";
        QString nametext="ЛП";
        addrtext=QString("%1").arg(ADDR(),4,16,QLatin1Char('0'));
        addrtext=addrtext.toUpper();
        nametext=objectName();
        // мастера
        for (int i=0;i<2;i++){
            if (M13_S[i]==0) painter->setBrush(QBrush(Qt::gray));
            if (M13_S[i]==-1) painter->setBrush(QBrush(Qt::red));
            if (M13_S[i]==1) painter->setBrush(QBrush(Qt::green));
            qreal y=-lpR.height()/4;
            if (i==1) y=+lpR.height()/4;
            painter->drawEllipse(QPointF(0,y),lpR.width()/4,lpR.width()/4.);
        }
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
        if (FTEXTNAMEHEIGHT<=0){
            namefontsz=12;
        }
        font.setPointSizeF(namefontsz);
        painter->setFont(font);
        RT=painter->fontMetrics().boundingRect(nametext);
        RT.adjust(-1,0,1,0);
        RT.moveCenter(QPointF(0,0));
        RT.moveBottom(-lpR.height()/2);
        painter->drawText(RT,flags,nametext,&boundrect_text);


//        painter->setBrush(QBrush(Qt::yellow));
//        for (int i=0;i<4;i++){
//            QList<QPolygonF> l=getInOutPolygons(i,in);
//            for (int j=0;j<l.size();j++){
//                painter->drawEllipse(l[j].back(),3,3);

//            }
//        }
//        painter->setBrush(QBrush(Qt::green));
//        for (int i=0;i<2;i++){
//            QList<QPolygonF> l=getInOutPolygons(i,out);
//            for (int j=0;j<l.size();j++){
//                painter->drawEllipse(l[j].back(),3,3);

//            }
//        }
        QSizeF sznamelink=QSizeF(lpR.width()/4, lpR.height()/4);
        FTEXTADDRHEIGHT-2 >0 ? font.setPointSizeF(FTEXTADDRHEIGHT-2):font.setPointSizeF(1);
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



        int _M15_S;
        for (int i=0;i<4;i++){
            if ((i==0)||(i==1)) _M15_S=M15_S[0];else _M15_S=M15_S[1];
            if (lines[i].size()>0){
                if (addr_is_new[i]>0){
                    painter->setPen(QPen(QBrush(QColor(55+addr_is_new[i]*2,55+addr_is_new[i]*2,55+addr_is_new[i]*2)),0,Qt::SolidLine));
                    //painter->setPen(QPen(QColor(55+addr_is_new[i]*2,255,255)));
                } else {
                    painter->setPen(Qt::NoPen);;
                }

                if (addr_not_found[i]){
                    painter->setBrush(QBrush(Qt::red));
                } else if (_M15_S==-1){
                    painter->setBrush(QBrush(Qt::gray));
                } else {
                    if (proclen[i]>0){
                        qreal len=QPFunction::lenPolyline(lines[i]);
                        qreal xc=len*proclen[i];
                        QPointF centr=QPFunction::pointOnPolygon(lines[i],xc);
                        QRadialGradient gradient(centr, len/10);
                        gradient.setColorAt(0, Qt::yellow);
                        gradient.setColorAt(1, Qt::green);
                        QBrush brush(gradient);
                        painter->setBrush(brush);
                    } else {
                        painter->setBrush(QBrush(Qt::green));
                    }
                }
//                if (option->state & PaintStyle::State_Selected){
//                    painter->setPen(QPen(QBrush(Qt::yellow),0,Qt::SolidLine));
//                }
                painter->drawPolygon(arrows[i]);
                //painter->setPen(QPen(QBrush(Qt::red),1,Qt::SolidLine));
                //painter->drawPolyline(lines[i]);
            }
        }



}

void v_LPDiag::calculateGeometry()
{
    addr_in[0]=FSTATE_ADDR_IN00;
    addr_in[1]=FSTATE_ADDR_IN01;
    addr_in[2]=FSTATE_ADDR_IN10;
    addr_in[3]=FSTATE_ADDR_IN11;

    QSizeF szr=QSizeF(w_lp(),h_lp());

    lpR=QRectF(QPointF(-szr.width()/2.,-szr.height()/2.),szr);
    if (boundrect_text.isEmpty()){
        qreal namefontsz=FTEXTNAMEHEIGHT;
        if (FTEXTNAMEHEIGHT==0)
            namefontsz=12;
        boundrect_text=QRectF(QPointF(-lpR.width()/2,-lpR.height()/2-namefontsz),QSizeF(namefontsz*10,namefontsz));
    }
    clearLines();
    makeLines();
    _boundingRect=lpR;

    if (!arrows[0].isEmpty())_boundingRect=_boundingRect.united(arrows[0].boundingRect());
    if (!arrows[1].isEmpty())_boundingRect=_boundingRect.united(arrows[1].boundingRect());
    if (!arrows[2].isEmpty())_boundingRect=_boundingRect.united(arrows[2].boundingRect());
    if (!arrows[3].isEmpty())_boundingRect=_boundingRect.united(arrows[3].boundingRect());

    _boundingRect=_boundingRect.united(boundrect_text);
    //setCacheMode(ItemCoordinateCache);
    prepareGeometryChange();

}




qreal v_LPDiag::YY(qreal yc,int ud,int np,int inout)
{
    qreal sy_inout=5;
    qreal H=h_lp();
    qreal sy1=10;
    qreal ddy=(H/2)/14;
    if (inout==out){
        sy_inout=-5;
    }
    if (ud==0){
        if (np==0) return yc-H/4+sy_inout;
        return yc-H/2-sy1-(np-1)*ddy;//-sy_inout;
    }else {
        if (np==0) return yc+H/4+sy_inout;
        return yc+H/2+sy1+(np-1)*ddy;//+sy_inout;
    }
    return 0;
}
static int UD[4]={0,0,1,1};
QList<QPolygonF> v_LPDiag::getInOutPolygons(int n, v_LPDiag::Tinout inout,int ud1,int nord)
{
    qreal sx=10;
    if (inout==in) sx=15;
    QList<QPolygonF> lpoly;
    QPolygonF poly1;
    poly1.clear();
    QPolygonF poly0;
    qreal yc=0;
    qreal yc0;
    qreal x0 = 0.0,x1 = 0.0;
    qreal W=w_lp();
    int ud = 0;
    int inout0=inout;
    if (n==0){
        ud=0;
        x0=-W/2-1; x1=x0-sx;
    }
    if (n==1){
        inout0=1-inout;
        ud=0;
        x0=W/2+1; x1=x0+sx;
    }
    if (n==2){
        ud=1;
        x0=-W/2-1; x1=x0-sx;
    }
    if (n==3){
        inout0=1-inout;
        ud=1;
        x0=W/2+1; x1=x0+sx;
    }
    yc0=YY(yc,ud,0,inout0);
    poly0.push_back(QPointF(x0,yc0));poly0.push_back(QPointF(x1,yc0));
    lpoly.push_back(poly0);
    if (ud1==0){
            poly1.clear();poly1 << poly0;
            poly1.push_back(QPointF(x1,YY(yc,0,nord+1,inout)));lpoly.push_back(poly1);
    }
    if (ud1==1){
            poly1.clear();poly1 << poly0;
            poly1.push_back(QPointF(x1,YY(yc,1,nord+1,inout)));lpoly.push_back(poly1);
        }
    return lpoly;
}

void v_LPDiag::clearLines()
{
    for (int i=0;i<4;i++){
        lines[i].clear();
        linesG[i].clear();
    }
}
void v_LPDiag::makeLines()
{
    int n2;
    for (int i=0;i<4;i++){
        lines[i].clear();
        addr_not_found[i]=false;
        if (addr_in[i]==0) continue;
        v_LPDiag *LP2=getLPDiag(FADDR_KRUG,addr_in[i],n2);
        if (LP2){
            addr_not_found[i]=false;
            lines[i]=makeLine(i, LP2, n2);
            arrows[i]=QPFunction::makeFullArrowPolygonFromPolyline(lines[i],lwidth,0,0,QPFunction::arrow_none,QPFunction::arrow_normal);
            //linesG[i]=polygon2root(lines[i]);
        } else {
            addr_not_found[i]=true;
            QList<QPolygonF> pl=getInOutPolygons(i,in,0,0);
            if (pl.size()>0)lines[i]=pl[0];
            arrows[i]=QPFunction::makeFullArrowPolygonFromPolyline(lines[i],lwidth,0,0,QPFunction::arrow_normal,QPFunction::arrow_none);
            //linesG[i]=polygon2root(lines[i]);
        }
    }
}
qreal animateInterval=1000;
void v_LPDiag::advance(int phase)
{
    bool new_proclen=false;
    if (phase==1){
//        if ((timeAnimateNewPacket.isValid())&&(timeAnimateNewPacket.elapsed()<animateInterval)){
//            new_proclen=(1.*timeAnimateNewPacket.elapsed()/animateInterval)*100;
//            if (new_proclen>100) new_proclen=100;
//        }
        qreal _proclen[4];
        _proclen[0]=1;
        if (chanels15[0]->timeDataRecived.isValid()) _proclen[0]=chanels15[0]->timeDataRecived.msecsTo(QDateTime::currentDateTime())/animateInterval;
        if (_proclen[0]>1) _proclen[0]=0;
        _proclen[1]=_proclen[0];
        _proclen[2]=1;
        if (chanels15[1]->timeDataRecived.isValid()) _proclen[2]=chanels15[1]->timeDataRecived.msecsTo(QDateTime::currentDateTime())/animateInterval;
        if (_proclen[2]>1) _proclen[2]=0;
        _proclen[3]=_proclen[2];

        for (int i=0;i<4;i++){
            addr_is_new[i]-=5;
            if (addr_is_new[i]<0) addr_is_new[i]=0;
            if (proclen[i]!=_proclen[i]) new_proclen=true;
            proclen[i]=_proclen[i];
        }

        if (new_proclen){
            prepareGeometryChange();
        }
    }
}





//void v_LPDiag::animateNewPacket()
//{
//    proclen=timerAnimateNewPacket->t remainingTime()/(animateInterval/100);
//    if (proclen>=100) {
//        timerAnimateNewPacket->stop();
//        proclen=0;
//    }
//    prepareGeometryChange();
//}

//void v_LPDiag::animateNewAddr()
//{
//    bool stop=true;
//    for (int i=0;i<4;i++){
//        addr_is_new[i]-=5;
//        if (addr_is_new[i]<0) addr_is_new[i]=0; else stop=false;
//    }
//    if (stop) {
//        timerAnimateNewAddr->stop();
//    }
//    prepareGeometryChange();
//}





int v_LPDiag::isAddrExists(int addr)
{
    if (addr==this->addr[0]) return 0;
    if (addr==this->addr[1]) return 1;
    if (addr==this->addr[2]) return 2;
    if (addr==this->addr[3]) return 3;
    return -1;
}

QString v_LPDiag::getNameLink(int n)
{
//    if (n==0) return "О1";
//    if (n==1) return "О2";
//    if (n==2) return "Р1";
//    if (n==3) return "Р2";
    if (n==0) return QString::number(this->addr[0],16);
    if (n==1) return QString::number(this->addr[1],16);
    if (n==2) return QString::number(this->addr[2],16);
    if (n==3) return QString::number(this->addr[3],16);
    return "";
}

v_LPDiag *v_LPDiag::getLPDiag(int addkrug, int addr, int &n) const
{
    if (parent()){
        QList <v_LPDiag*> l=parent()->findChildren<v_LPDiag*>();
        for (int i=0;i<l.size();i++){
            if (l[i]->ADDR_KRUG()!=addkrug) continue;
            n=l[i]->isAddrExists(addr);
            if (n>=0){
                return l[i];
            }
        }
    }
    return nullptr;
}

bool v_LPDiag::isIntersect(int addr_krug, QPolygonF &scr_p1)
{
    if (parent()){
        //QPolygonF scr_p1=this->getProxyGraphicsItem()->mapToItem(Fscreen->getProxyGraphicsItem(),p);
        QList <v_LPDiag*> l=parent()->findChildren<v_LPDiag*>();
        for (int il=0;il<l.size();il++){
            if (l[il]->ADDR_KRUG()!=addr_krug) continue;
            //if (l[il]->ADDR()==ADDR()) continue;
            QPolygonF scr_lpR2=l[il]->getProxyGraphicsItem()->mapToItem( screen()->getProxyGraphicsItem(),l[il]->lpR);
            if (QPFunction::isPolylineIntersectPolygon(scr_p1,scr_lpR2)){
                return true;
            }
        }
    }
    return false;
}

//int v_LPDiag::sovpad(int addr_krug, QPolygonF &PL1)
//{
//    int r=0;
//    if (visualElements()){
//        QList <v_LPDiag*> l=visualElements()->findChildren<v_LPDiag*>();

//        for (int i=0;i<l.size();i++){
//            if (l[i]->addr_krug!=addr_krug) continue;
//            for (int j=0;j<4;j++){
//                QPolygonF PL2=l[i]->linesG[j];
//                for (int t1=0;t1<PL1.size();t1++){
//                    for (int t2=0;t2<PL2.size()-1;t2++){
//                        if (QPFunction::isPointOnLine(PL2[t2],PL2[t2+1],PL1[t1])) r++;
//                    }
//                }
//                for (int t1=0;t1<PL2.size();t1++){
//                    for (int t2=0;t2<PL1.size()-1;t2++){
//                        if (QPFunction::isPointOnLine(PL1[t2],PL1[t2+1],PL2[t1])) r++;
//                    }
//                }
//            }
//        }
//    }
//    return r;
//}



QPolygonF v_LPDiag::makeLine(int n1, v_LPDiag *LP2, int n2)
{
    QPolygonF minp;
    QList<QPolygonF> lout=LP2->getInOutPolygons(n2,out,UD[n1],FNORDER);
    if ((lout.size()==0)) return minp;
    QList<QPolygonF> lin=getInOutPolygons(n1,in,UD[n1],FNORDER);
    if ((lin.size()==0)) return minp;

    // Ищем самый короткий и не пересека
    {
        QPolygonF scr_outp2=LP2->getProxyGraphicsItem()->mapToItem(screen()->getProxyGraphicsItem(),lout[0]);
        QPolygonF scr_inp1=this->getProxyGraphicsItem()->mapToItem(screen()->getProxyGraphicsItem(),lin[0]);
        QPolygonF scr_p;
        scr_p << scr_outp2;
        for (int t=scr_inp1.size()-1;t>=0;t--)
            scr_p << scr_inp1[t];

        if (!isIntersect(FADDR_KRUG,scr_p)) {
            return screen()->getProxyGraphicsItem()->mapToItem(this->getProxyGraphicsItem(),scr_p);
        }

    }
    // иначе берем 2ой
    {
        QPolygonF scr_outp2=LP2->getProxyGraphicsItem()->mapToItem(screen()->getProxyGraphicsItem(),lout[1]);
        QPolygonF scr_inp1=this->getProxyGraphicsItem()->mapToItem(screen()->getProxyGraphicsItem(),lin[1]);
        QPolygonF scr_p;
        scr_p << scr_outp2;
        for (int t=scr_inp1.size()-1;t>=0;t--)
            scr_p << scr_inp1[t];

        return screen()->getProxyGraphicsItem()->mapToItem(this->getProxyGraphicsItem(),scr_p);

    }


//    if (minp.empty()){
//        for (int i=1;i<lout.size();i++){
//            QPolygonF outp=polygonO2O(LP2,lout[i],this);
//            for (int j=1;j<lin.size();j++){
//                QPolygonF inp=lin[j];
//                // если не первые то только по прямой
//                if ((outp.back().y()!=inp.back().y())) continue;
//                QPolygonF p;
//                p << outp;
//                for (int t=inp.size()-1;t>=0;t--)
//                    p << inp[t];
//                QPolygonF pg=polygon2root(p);
//                if (rootLP->isIntersect(addr_krug,pg)) continue;
//                qreal len1=p.size();//QPFunction::lenPolyline(p);
//                int sovp= rootLP->sovpad(addr_krug,pg);
//                if (minp.isEmpty()){
//                    minp=p;minlen=len1;minsovp=sovp;
//                } else
//                    if ((sovp<minsovp) || ((sovp==minsovp)&&(minlen>len1))){
//                        minp=p;minlen=len1;minsovp=sovp;
//                    }
//            }
//        }
//    }
//    if (minp.isEmpty()){
//        // рисуем напрямую
//        QPolygonF _outp=lout[0];
//        QPolygonF inp=lin[0];
//        QPolygonF outp;
//        for (int t=0;t<_outp.size();t++){
//            QPointF GP=LP2->point2Parent(_outp[t],rootLP);
//            outp.push_back(this->parent2point(GP,rootLP));
//        }
//        QPolygonF p;
//        p << outp;
//        for (int t=inp.size()-1;t>=0;t--)
//            p << inp[t];
//        minp=p;
//    }
    return minp;
}

//QPolygonF v_LPDiag::polygon2root(QPolygonF &PL)
//{
//    QPolygonF P;
//    v_RootLPDiag *rootLP=rootLPDiag();
//    if (rootLP==0) return P;
//    for (int t=0;t<PL.size();t++){
//        P.push_back(point2Parent(PL[t],rootLP));
//    }
//    return P;
//}

//QPolygonF v_LPDiag::polygonFromRoot(QPolygonF &PL)
//{
//    QPolygonF P;
//    v_RootLPDiag *rootLP=rootLPDiag();
//    if (rootLP==0) return P;
//    for (int t=0;t<PL.size();t++){
//        P.push_back(parent2point(PL[t],rootLP));
//    }
//    return P;
//}

//QPolygonF v_LPDiag::polygonO2O(v_LPDiag* LP1,QPolygonF &PL,v_LPDiag* LP2)
//{
//    QPolygonF rootPl=LP1->polygon2root(PL);
//    return LP2->polygonFromRoot(rootPl);
//}

qreal v_LPDiag::getFontPointSize4rect(QPainter *painter,QString &text, QRectF R, qreal maxSize)
{
    QRectF RT=painter->fontMetrics().boundingRect(text);
    qreal mx=R.width()/RT.width();
    qreal my=R.height()/RT.height();
    qreal mm=mx;
    if (mm>my) mm=my;
    qreal sz=painter->font().pointSizeF();
    sz=sz*mm;
    if (sz>maxSize) sz=maxSize;
    return sz;
}



