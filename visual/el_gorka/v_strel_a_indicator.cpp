#include "v_strel_a_indicator.h"

#include "baseobjecttools.h"
#include "qpfunction.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_Strel_A_Indicator,"Знак АВТОМАТ","VISUAL ГОРКА")


REGISTERPROPERTY(v_Strel_A_Indicator,SIZE,"РАЗМЕР","Размер знака АВТОМАТ","Автомат")
REGISTERCOLOR(v_Strel_A_Indicator,strel_auto,"Стрелка АВТОМАТ",Qt::yellow);

v_Strel_A_Indicator::v_Strel_A_Indicator(v_Base *parent) : v_Base(parent)
{
    FTARGET.clear();
    strel=nullptr;
    FSIZE=QSizeF(2,1);
    calculateGeometry();
}

void v_Strel_A_Indicator::setTARGET(ObjectLink p)
{
    if (FTARGET!=p){
        FTARGET=p;
        strel=qobject_cast<m_Base*>(reLink(this,FTARGET));
        calculateGeometry();
    }
}


void v_Strel_A_Indicator::updateAfterLoad()
{
    v_Base::updateAfterLoad();
    if (FTARGET.id()!=0){
        strel=qobject_cast<m_Base*>(reLink(this,FTARGET));
        if (!strel)
            qCritical() << objectName() << "Ошибочная ссылка TARGET" <<endl ;
    }
    if(strel!=nullptr){
        connect(strel,SIGNAL(stateChanged(QObject*)),this,SLOT(slotModelStateChanged(QObject*)));
    }
}

void v_Strel_A_Indicator::calculateGeometry()
{
    // Стрела автомата
    M[0]=QPoint(0,0);
    AUTOPL.clear();
    QPointF A0=QPoint(0,0);
    qreal szw=gridDX()*FSIZE.width();
    qreal szh=gridDY()*FSIZE.height();
    AUTOPL << QPointF(A0.x(),A0.y()-szh/2) <<
              QPointF(A0.x()+szw/2,A0.y()-szh/2) <<
              QPointF(A0.x()+szw/2,A0.y()-szh/2-szh/2) <<
              QPointF(A0.x()+szw,A0.y()) <<
              QPointF(A0.x()+szw/2,A0.y()+szh/2+szh/2) <<
              QPointF(A0.x()+szw/2,A0.y()+szh/2) <<
              QPointF(A0.x(),A0.y()+szh/2);
    // переворот
    if (FINVERTDRAWX){
        QPFunction::reflectPLx(&AUTOPL,0);

    }
    _boundingRect=AUTOPL.boundingRect();
}

void v_Strel_A_Indicator::updateStates()
{
    if (strel) {
        QVariant V=strel->property("STATE_A");
        if (V.isValid()) {
            FSTATE_A=V.toBool();
        }
    }
}

void v_Strel_A_Indicator::slotModelStateChanged(QObject *)
{
    updateStates();
    calculateGeometry();
    update();
}


void v_Strel_A_Indicator::d_paint(QPainter *painter, const QStyleOptionGraphicsItem *)
{
    QColor cb=getColor(Color_strel_auto);
    if (ISEDITORMODE){
        cb=Qt::gray;
        if (!strel) cb=getColor(Color_modelNotAccepted);
    } else {
        if (!FSTATE_A) return;
    }

    painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));
    painter->setBrush(QBrush(cb));
    painter->drawPolygon(AUTOPL);
}
