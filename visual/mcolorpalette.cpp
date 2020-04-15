#include "mcolorpalette.h"
#include <QDebug>
#include <qtimer.h>
#include <QFileInfo>
#include "qobject2xml.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(MColorPalette,"Палитра цветов","VISUAL БАЗОВЫЕ")

static int _idcounter=0;
MColorPalette::MColorPalette(QObject *parent) : BaseObject(parent)
{
    setObjectName("Текущая палитра");
    timer=nullptr;
    FexistsUnLoadedColors=false;
    timer_ms_elapsed=0;

}

MColorPalette::~MColorPalette()
{
    if (timer) delete timer;
}

int MColorPalette::registerColor(QString idstr, QString name, Qt::GlobalColor color)
{
    name.replace('"',"");

    QList <MColor*> l=findChildren<MColor*>();
    foreach (MColor*mclr, l) {
        if (mclr->idstr()==idstr)
            return mclr->id();
    }
//    if (m_UID2MColor.contains(idstr)){
//        qCritical() << "цвет с uid "<< idstr << " уже зарегестрирован("<< idstr << m_UID2MColor[idstr]->objectName() << ")";
//        return m_UID2MColor[idstr];
//    }
    MColor *newMColor=new MColor(this);
    newMColor->setParent(this);
    newMColor->setCOLOR1(QColor(color));
    newMColor->setObjectName(name);
    newMColor->setidstr(idstr);
    newMColor->setUID(++_idcounter);
    m_UID2MColor[newMColor->UID()]=newMColor;
    m_IDSTR2MColor[newMColor->idstr()]=newMColor;
    FexistsUnLoadedColors=true;
    return newMColor->UID();
}

int MColorPalette::registerColor(QString idstr, QString name, Qt::GlobalColor color1,QColor color2)
{

    int uid=registerColor(idstr, name, color1);
    MColor *newMColor=getMColor(uid);
    newMColor->setCOLOR2(QColor(color2));
    newMColor->setDT1(500);
    newMColor->setDT2(500);
    return newMColor->UID();
}


MColor* MColorPalette::getMColor(int uid) const
{
    if (!m_UID2MColor.contains(uid)){
        qCritical() << "запрос отсутвуещего цвета " << uid;
        return m_UID2MColor[1];
    }
    MColor*mc=m_UID2MColor[uid];
    return mc;
}

MColor *MColorPalette::getMColor_idstr(QString idstr)
{
    if (!m_IDSTR2MColor.contains(idstr)){
        registerColor(idstr, QString("незарегестрированный цвет %1").arg(idstr), Qt::blue);
    }
    MColor*mc=m_IDSTR2MColor[idstr];
    return mc;
}

QColor MColorPalette::color(int uid) const
{
    return getMColor(uid)->getCOLOR();
}

QColor MColorPalette::color(QString idstr)
{
    return getMColor_idstr(idstr)->getCOLOR();
}

bool MColorPalette::existsColor(QString idstr) const
{
    if (m_IDSTR2MColor.contains(idstr)) return true;
    return false;
}


//void MColorPalette::assignPalette0()
//{
//    MColorPalette *palette=instance();
//    // проставляем uid

//    QList <MColor*> l=findChildren<MColor*>();
//    foreach (MColor*mclr, l) {
//        mclr->setUID(0);
//    }
//    foreach (MColor*mclr, l) {
//        QList <MColor*> l2=palette->findChildren<MColor*>(mclr->objectName());
//        if (l2.size()>0)
//            mclr->setUID(l2[0]->UID());
//    }
//    // добавляум отстающие
//    l=palette->findChildren<MColor*>();
//    foreach (MColor*mclr, l) {
//        QList <MColor*> l2=findChildren<MColor*>(mclr->objectName());
//        if (l2.size()==0){
//            MColor *newMColor=new MColor(*mclr,this);
//            newMColor->setParent(this);
//        }
//    }
//    // заносим в мапу
//    m_UID2MColor.clear();
//    l=findChildren<MColor*>();
//    foreach (MColor*mclr, l) {
//        m_UID2MColor[mclr->UID()]=mclr;
//    }
//}


void MColorPalette::updateAfterLoad()
{
    BaseObject::updateAfterLoad();
    QList <MColor*> l=findChildren<MColor*>();
    foreach (MColor*mclr, l) {
        mclr->updateAfterLoad();
    }
    if (!timer){
        timer=new QTimer();
        connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
        timer->start(24);
        timerStart.start();
    }
}

bool MColorPalette::isCanAddObject(QObject *) const
{
    return false;
}

void MColorPalette::addObject(QObject *)
{

}

void MColorPalette::load(QString fn)
{
    setXMLfile(fn);
    if (QFileInfo(fn).exists()){
        QObject2XML XML(MVP_ObjectFactory::instance());
        QObject *O=XML.readFile(fn);
        MColorPalette *P=qobject_cast<MColorPalette *> (O);
        if (P){
            QList<MColor *> l=P->findChildren<MColor *>();
            foreach (MColor *mclr,l) {
                MColor *mymclr=getMColor_idstr(mclr->idstr());
                if (mymclr) {
                    mymclr->assign(mclr);
                }
            }
        }
        FexistsUnLoadedColors=false;

    }
}

QString MColorPalette::rgbString(QColor c)
{
    return QString("#%1%2%3").arg(c.red(),2,16,QLatin1Char('0')).arg(c.green(),2,16,QLatin1Char('0')).arg(c.blue(),2,16,QLatin1Char('0'));
}

qreal MColorPalette::animateK(int ms_peroid) const
{
    if (timer_ms_elapsed==0) return 0;
    if (ms_peroid==0) return 1;
    return 1.*((timer_ms_elapsed % ms_peroid)+1) /ms_peroid;
}

void MColorPalette::updateTimer()
{
    QList <MColor*> l=findChildren<MColor*>();
    foreach (MColor*mclr, l) {
        mclr->updateTimer();
    }
    timer_ms_elapsed=timerStart.elapsed();
}


QColor Color2Color(QColor c1,QColor c2,qreal procent)
{
    if (procent>1) procent=1;
    int r1,g1,b1,a1,r2,g2,b2,a2;
    c1.getRgb(&r1,&g1,&b1,&a1);
    c2.getRgb(&r2,&g2,&b2,&a2);
    r1=r1+(r2-r1)*procent;
    g1=g1+(g2-g1)*procent;
    b1=b1+(b2-b1)*procent;
    a1=a1+(a2-a1)*procent;
    return QColor(r1,g1,b1,a1);
}

QColor mvpColor(QString idstr)
{
    return MColorPalette::instance()->color(idstr);

}
