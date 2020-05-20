#include "v_otcep.h"
#include "m_otcep.h"
#include "v_otceps.h"
#include "visual_screen.h"
#include "v_rc.h"
#include "qpfunction.h"

v_Otcep::v_Otcep(QObject *parent) : v_Model(parent)
{
    mOtcep=nullptr;
    target=nullptr;
    width=40;
    height=20;
    direct=MVP_Enums::left2right;
    setZValue(30);
    setVisible(false);
}

void v_Otcep::setModelObject(BaseObject *B)
{
    mOtcep=qobject_cast<m_Otcep*>(B);
    v_Model::setModelObject(mOtcep);
}

void v_Otcep::validation(ListObjStr *l) const
{
    if (!mOtcep) l->error(this,QString("Нет в медели контоллера отцепов"));
}

void v_Otcep::resetStates()
{
}

void v_Otcep::updateStates()
{
    v_Model::updateStates();


}

void v_Otcep::calculateGeometry()
{
    // находим координаты РЦ
    if (!target) return;
    //QPolygonF CP=target->centreLine();
    if (targetLine.isEmpty()) return;
    //QPFunction::sortByX(CP,direct);
    QPFunction::sortByX(targetLine,MVP_Enums::left2right);
    QPointF targetP=targetLine.first();
    if (direct==MVP_Enums::left2right) targetP=targetLine.last();
    if ((target)&&(target->TRAIN_NUM_BY_END())&&(mOtcep) && (mOtcep->RCS==mOtcep->RCF)){
        if (direct==MVP_Enums::left2right) {
            targetP=targetLine.first();
            targetP.setX(targetP.x()+width);
        }else{
            targetP=targetLine.last();
            targetP.setX(targetP.x()-width);
        }
    }


    //QPointF my_pos=target->getProxyGraphicsItem()->mapToItem(Fscreen->getProxyGraphicsItem(),targetP);
    QPointF my_pos=targetP;

    proxyGraphicsItem->setPos(my_pos);

    P.clear();
    qreal h2=height/2;
    qreal xx=h2;
    P << M[0]
            << QPointF(M[0].x()+xx,M[0].y()-h2)
            << QPointF(M[0].x()+width,M[0].y()-h2)
            << QPointF(M[0].x()+width,M[0].y()+h2)
            << QPointF(M[0].x()+xx,M[0].y()+h2);
    PP=QPFunction::makeFullPolygonFromPolyline(targetLine,h2);
    if (screen())
        PP=screen()->getProxyGraphicsItem()->mapToItem(getProxyGraphicsItem(),PP);
    lPP.clear();
    if ((mOtcep) && (mOtcep->STATE_VAGON_CNT()>1)){
        qreal p1=0;
        qreal pd=1./mOtcep->STATE_VAGON_CNT();
        for (int i=0;i<mOtcep->STATE_VAGON_CNT();i++){
            QPolygonF p=QPFunction::cutPolyline(targetLine, 1.*i*pd, 1.*i*pd+pd);
            QPolygonF PP1=QPFunction::makeFullPolygonFromPolyline(p,h2);
            PP1=screen()->getProxyGraphicsItem()->mapToItem(getProxyGraphicsItem(),PP1);
            lPP.push_back(PP1);
        }
    } else {
        lPP.push_back(PP);
    }

    textR=QRectF(0,0,width-xx,height);
    textR.moveTopLeft(QPointF(M[0].x()+xx,M[0].y()-h2));

    if (direct==MVP_Enums::left2right){
        QPFunction::reflectPLx(&P,M[0].x());
        textR.moveTopLeft(QPointF(M[0].x()-width,M[0].y()-h2));
    }

    R=P.boundingRect();
    _boundingRect=R.united(PP.boundingRect());
}

void v_Otcep::d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    QBrush B=QBrush(color);
    QPen   p=QPen(Qt::black);
    painter->setBrush(Qt::NoBrush);
    if (option->levelOfDetailFromTransform(painter->worldTransform())>FLEVELDETAIL){
        foreach (auto PP1, lPP) {
            painter->drawPolygon(PP1);
        }
    } else {
       painter->drawPolygon(PP);
    }

    painter->setPen(p);painter->setBrush(B);
    painter->drawPolygon(P);
    QFont font("Times", height-2);
    painter->setFont(font);
    painter->drawText(textR, Qt::AlignCenter,QString("%1").arg(number));
}

void v_Otcep::updateStatesFromModel()
{
    v_Model::updateStatesFromModel();
    number=0;
    if (mOtcep) {
        number=mOtcep->NUM();
        if ((!mOtcep->STATE_ENABLED())|| (mOtcep->STATE_LOCATION()==m_Otcep::locationUnknow)){
            setVisible(false);
            return;
        }
        color=Qt::white;
        if ((mOtcep->STATE_ERROR()!=0)) color=Qt::red;
    }

    // находим РЦ

    target=nullptr;
    targetLine.clear();
    if (mOtcep){
        foreach (m_RC * mrc,mOtcep->vBusyRc){
            v_RC*vrc=(qobject_cast<v_Otceps*>(parent()))->m_RC2v_RCV(mrc);
            if (vrc!=nullptr){
                if (target==nullptr) target=vrc;
                QPolygonF vrc_centreLine=vrc->centreLine();
                QPFunction::sortByX(vrc_centreLine,MVP_Enums::left2right);
                if ((mrc==mOtcep->RCS) && (mOtcep->RCS==mOtcep->RCF)) {
                    qreal p1=1;
                    qreal p2=1;
                    if ((mrc->LEN()>0)&&(mOtcep->STATE_D_RCS_XOFFSET()>0)&&(mOtcep->STATE_D_RCF_XOFFSET()>0)) {
                        p1=1.*mOtcep->STATE_D_RCS_XOFFSET()/mrc->LEN();
                        p2=1.*mOtcep->STATE_D_RCF_XOFFSET()/mrc->LEN();
                    }
                    if (p1>1) p1=1;
                    if (p2>1) p2=1;
                    if (p1<p2) p1=p2;
                    if (vrc->DIRECT_D0()==MVP_Enums::left2right)
                        vrc_centreLine=QPFunction::cutPolyline(vrc_centreLine, p1, p2); else
                        vrc_centreLine=QPFunction::cutPolyline(vrc_centreLine, 1-p1, 1-p2);
                } else
                    if (mrc==mOtcep->RCS){
                        qreal p=1;
                        if ((mrc->LEN()>0)&&(mOtcep->STATE_D_RCS_XOFFSET()>0)) p=1.*mOtcep->STATE_D_RCS_XOFFSET()/mrc->LEN();
                        if (p>1) p=1;
                        if (vrc->DIRECT_D0()==MVP_Enums::left2right)
                            vrc_centreLine=QPFunction::cutPolyline(vrc_centreLine, 0, p); else
                            vrc_centreLine=QPFunction::cutPolyline(vrc_centreLine, 1-p, 1);
                    } else
                        if (mrc==mOtcep->RCF){
                            qreal p=1;
                            if ((mrc->LEN()>0) &&(mOtcep->STATE_D_RCF_XOFFSET()>0))p=1.*mOtcep->STATE_D_RCF_XOFFSET()/mrc->LEN();
                            if (p>1) p=1;
                            if (vrc->DIRECT_D0()==MVP_Enums::left2right)
                                vrc_centreLine=QPFunction::cutPolyline(vrc_centreLine, p, 1); else
                                vrc_centreLine=QPFunction::cutPolyline(vrc_centreLine, 0, 1-p);
                        }
                foreach (QPointF targetP, vrc_centreLine) {
                    if (screen())
                        targetP=vrc-> getProxyGraphicsItem()->mapToItem(screen()->getProxyGraphicsItem(),targetP);
                    if (targetLine.indexOf(targetP)<0)
                        targetLine.push_back(targetP);
                }
            }
        }
    }

    if (target){
        setVisible(true);
        calculateGeometry();
    } else {
        setVisible(false);
    }
}
