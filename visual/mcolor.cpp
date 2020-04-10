#include "mcolor.h"
#include <QTimer>

#include "mvp_objectfactory.h"
REGISTERELEMENT(MColor,"Цвет","VISUAL БАЗОВЫЕ")


void MColor::setCOLOR(int i, QColor p)
{
    COLOR[i]=p;update();
}

void MColor::setALPHA(int i, int p)
{
    ALPHA[i]=p;update();
}

void MColor::setSMOOTH(int i, int p)
{
    SMOOTH[i]=p;update();
}

MColor::MColor(QObject *parent) : BaseObject(parent)
{
    COLOR[0]=QColor(Qt::black);
    COLOR[1]=QColor(Qt::transparent);
    COLOR[2]=QColor(Qt::transparent);
    COLOR[3]=QColor(Qt::transparent);
    DT[0]=DT[1]=DT[2]=DT[3]=0;
    ALPHA[0]=255;ALPHA[1]=255;ALPHA[2]=255;ALPHA[3]=255;
    currentIndex=0;
    currentColor=COLOR[0];currentColor.setAlpha(ALPHA[currentIndex]);
}

MColor::MColor(const MColor &mclr, QObject *parent): BaseObject(parent)
{
    assign(&mclr);
    currentIndex=0;
    currentColor=COLOR[0];currentColor.setAlpha(ALPHA[currentIndex]);
}

MColor::MColor(int uid, QString name, QColor clr, QObject *parent): BaseObject(parent)
{
    setUID(uid);
    setObjectName(name);
    COLOR[0]=clr;
    COLOR[1]=QColor(Qt::transparent);
    COLOR[2]=QColor(Qt::transparent);
    COLOR[3]=QColor(Qt::transparent);
    DT[0]=DT[1]=DT[2]=DT[3]=0;
    ALPHA[0]=255;ALPHA[1]=255;ALPHA[2]=255;ALPHA[3]=255;
    currentIndex=0;
    currentColor=COLOR[0];currentColor.setAlpha(ALPHA[currentIndex]);


}

void MColor::assign(const MColor *mclr)
{
    fuid=mclr->UID();
    setObjectName(mclr->objectName());
    Fidstr=mclr->idstr();
    COLOR[0]=mclr->getCOLOR1();
    COLOR[1]=mclr->getCOLOR2();
    COLOR[2]=mclr->getCOLOR3();
    COLOR[3]=mclr->getCOLOR4();
    DT[0]=mclr->getDT1();
    DT[1]=mclr->getDT2();
    DT[2]=mclr->getDT3();
    DT[3]=mclr->getDT4();
    ALPHA[0]=mclr->getALPHA1();
    ALPHA[1]=mclr->getALPHA2();
    ALPHA[2]=mclr->getALPHA3();
    ALPHA[3]=mclr->getALPHA4();
    SMOOTH[0]=mclr->getSMOOTH1();
    SMOOTH[1]=mclr->getSMOOTH2();
    SMOOTH[2]=mclr->getSMOOTH3();
    SMOOTH[3]=mclr->getSMOOTH4();
}

MColor::~MColor()
{
}

void MColor::updateAfterLoad()
{
    BaseObject::updateAfterLoad();
    if (DT[0]!=0){

    }
    currentColor=COLOR[0];currentColor.setAlpha(ALPHA[currentIndex]);
    startT=QDateTime::currentDateTime();
}

QColor MColor::getCOLOR()
{
    return currentColor;
}

bool MColor::isAnimated()
{
    if (DT[0]!=0) return true;
    return false;
}

void MColor::update()
{
    currentColor=COLOR[currentIndex];
}

void MColor::updateTimer()
{
    QColor newColor=currentColor;
    if (DT[currentIndex]>0){
        if (startT.msecsTo(QDateTime::currentDateTime())>=DT[currentIndex]){
            currentIndex++;
            if (currentIndex>3)currentIndex=0;
            if (DT[currentIndex]==0)currentIndex=0;
            newColor=COLOR[currentIndex];
            newColor.setAlpha(ALPHA[currentIndex]);
            startT=QDateTime::currentDateTime();
        } else
            if (SMOOTH[currentIndex]){
                QColor C1=COLOR[currentIndex];
                C1.setAlpha(ALPHA[currentIndex]);
                int nextIndex=currentIndex+1;
                if (nextIndex>3)nextIndex=0;
                if (DT[nextIndex]==0)nextIndex=0;
                QColor C2=COLOR[nextIndex];
                C2.setAlpha(ALPHA[nextIndex]);
                long msec=startT.msecsTo(QDateTime::currentDateTime());
                qreal p=1.*msec/DT[currentIndex];
                if (p>1) p=1;
                int r= C1.red()+(C2.red()-C1.red())*p;
                int g= C1.green()+(C2.green()-C1.green())*p;
                int b= C1.blue()+(C2.blue()-C1.blue())*p;
                int a= C1.alpha()+(C2.alpha()-C1.alpha())*p;
                newColor=QColor(r, g, b, a);
            }
        if (currentColor!=newColor){
            currentColor=newColor;
            emit changed();

        }
    }
}

