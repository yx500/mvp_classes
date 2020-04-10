#include "v_dso.h"

#include "m_dso.h"

#include "qpfunction.h"


#include "mvp_objectfactory.h"
REGISTERELEMENT(v_DSO,"ДСО","VISUAL ГОРКА")
REGISTERCOLOR(v_DSO,dso_free,"ДСО свободен",Qt::lightGray );
REGISTERCOLOR(v_DSO,dso_busy,"ДСО занят",Qt::red );
REGISTERCOLOR(v_DSO,dso_error,"ДСО ошибка",Qt::red );

v_DSO::v_DSO(v_Base *parent) :
    v_Model(parent)
{
    FSH=10;FSHRC=4;
    _m_DSO=nullptr;
    lastOsyCount=0;
    dtStartShowText.restart();
    addCOMMON_PROPERTY("SHRC","SH_RC");
    addCOMMON_PROPERTY("SH","SH_SVET");
    calculateGeometry();
    resetStates();
}

void v_DSO::resetStates()
{
    v_Model::resetStates();
    FSTATE_SRAB=0;
    FSTATE_OSY_COUNT=0;
    FSTATE_ERROR=0;
}

void v_DSO::updateAfterLoad()
{
    v_Model::updateAfterLoad();
    _m_DSO=qobject_cast<m_DSO*>(modelObject());
}

void v_DSO::setSH(int p)
{
    if (p>=0){
        if (FSH!=p){
            FSH=p;
            doPropertyChanged();
        }
    }
}

void v_DSO::setSHRC(int p)
{
    if (p>=0){
        if (FSHRC!=p){
            FSHRC=p;
            doPropertyChanged();
        }
    }
}


void v_DSO::calculateGeometry()
{
    // а глянем что нам предлагают
    S=FSH;
    SRC=FSHRC;

    M[0]=QPointF(0,0);
    QPointF T[4];
    // V
    T[0]=QPointF(M[0].x(),M[0].y()-SRC);
    T[1]=QPointF(T[0].x()-S/2.,T[0].y()-S);
    T[2]=QPointF(T[0].x()+S/2.,T[0].y()-S);
    T[3]=QPointF(T[0].x()     ,T[0].y()-S);

    mt=QPointF(T[3].x()     ,T[3].y()-5);

    P[0].clear();P[1].clear();
    PP.clear();

    PP << T[0] << T[1] <<T[2];
    P[0] << T[0] << T[1] <<T[3];
    P[1] << T[0] << T[3] <<T[2];

    // переворот
    if (FINVERTDRAWX){
        QPFunction::reflectPLx(&PP,M[0].x());
        QPFunction::reflectPLx(&P[0],M[0].x());
        QPFunction::reflectPLx(&P[1],M[0].x());
    }
    if (FINVERTDRAWY){
        QPFunction::reflectPLy(&PP,M[0].y());
        QPFunction::reflectPLy(&P[0],M[0].y());
        QPFunction::reflectPLy(&P[1],M[0].y());
        mt=QPFunction::reflectPy(mt,M[0].y());
    }
    _boundingRect=PP.boundingRect();
    if (!boundrect_text.isEmpty()) _boundingRect=_boundingRect.united(boundrect_text);
    _boundingRect.adjust(-5,-5,+5,+5);
}

void v_DSO::slotModelStateChanged(QObject *O)
{
    Q_UNUSED(O);
    if (_m_DSO){
        if (FSTATE_OSY_COUNT!=_m_DSO->STATE_OSY_COUNT())
            dtStartShowText.restart();
    }
    v_Model::slotModelStateChanged(O);
}

void v_DSO::d_paint(QPainter *painter, const QStyleOptionGraphicsItem * option)
{
    
    QColor clrB=QColor();
    if (!_m_DSO){
        clrB=getColor(Color_modelNotAccepted);
        painter->setPen(QPen(QBrush(clrB),0,Qt::SolidLine));
        painter->setBrush(clrB);
        painter->drawPolygon(PP);
    } else {
        if (FSTATE_33) {
            clrB=getColor(Color_state_33);
            painter->setBrush(clrB);
            painter->setPen(QPen(QBrush(clrB),0,Qt::SolidLine));
            painter->drawPolygon(PP);
        } else {
            clrB=getColor(Color_dso_free);
            painter->setBrush(clrB);
            if (FSTATE_ERROR==1) painter->setPen(QPen(QBrush(getColor(Color_dso_busy)),0,Qt::SolidLine)); else
                painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));

            painter->drawPolygon(PP);
            if ((FSTATE_SRAB==1)||(FSTATE_SRAB==3)) {
                clrB=getColor(Color_dso_busy);painter->setBrush(clrB);
                painter->setPen(QPen(QBrush(clrB),0,Qt::SolidLine));
                painter->drawPolygon(P[0]);
            }
            if ((FSTATE_SRAB==2)||(FSTATE_SRAB==3)) {
                clrB=getColor(Color_dso_busy);painter->setBrush(clrB);
                painter->setPen(QPen(QBrush(clrB),0,Qt::SolidLine));
                painter->drawPolygon(P[1]);
            }
            bool btext=false;
            QColor clr=Qt::black;

            if (option->levelOfDetailFromTransform(painter->worldTransform())>FLEVELDETAIL){
                btext=true;
            }
            if (!btext){
                qint64 msShowTime=1000;
                qint64 ms=dtStartShowText.elapsed();
                if (ms<msShowTime){
                    btext=true;
                    int alpha=255.*(1.*(msShowTime-ms)/msShowTime);
                    clr.setAlpha(alpha);
                }
            }
            if (btext){
                painter->setPen(QPen(QBrush(clr),0,Qt::SolidLine));
                showText(painter);
            }
        }
    }
}

void v_DSO::updateStatesFromModel()
{
    v_Model::updateStatesFromModel();
    if (_m_DSO){
        setSTATE_OSY_COUNT(_m_DSO->STATE_OSY_COUNT());
        setSTATE_SRAB(_m_DSO->STATE_SRAB());
        setSTATE_ERROR(_m_DSO->STATE_ERROR());
    } else {
        setSTATE_OSY_COUNT(0);
        setSTATE_SRAB(0);
        setSTATE_ERROR(0);
    }

}
void v_DSO::showText(QPainter *painter)
{
    boundrect_text=QRectF();
    QFont font("Times", 8);
    painter->setFont(font);
    QString text=QString::number(FSTATE_OSY_COUNT);
    if (ISEDITORMODE)
        text=objectName();
    QRectF RT=painter->fontMetrics().boundingRect(text);
    RT.adjust(-1,-1,+1,+1);
    int flags=Qt::AlignCenter;
    RT.moveCenter(mt);
    painter->drawText(RT,flags,text,&boundrect_text);
}
