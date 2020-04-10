#include "v_otceps.h"
#include "m_otceps.h"

#include "v_rc.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_Otceps,"Группа отцепы","VISUAL ГОРКА")

v_Otceps::v_Otceps(QObject *parent) : v_Model(parent)
{
    FDIRECT_D0=MVP_Enums::left2right;
    FSIZE=QSizeF(4,2);
    mOtceps=nullptr;
}

v_Otceps::~v_Otceps()
{
    foreach (v_Otcep*v, lOtcep) {
        delete v;
    }
    lOtcep.clear();
}

void v_Otceps::setModelObject(BaseObject *B)
{
    v_Model::setModelObject(B);
    if (modelObject()){
        mOtceps=qobject_cast<m_Otceps*>(modelObject());
        if (!mOtceps){
            QList<m_Otceps*> lm_Otceps=modelObject()->findChildren<m_Otceps*>();
            if (lm_Otceps.size()>0) mOtceps=lm_Otceps.first();
        }
    }
}

void v_Otceps::updateAfterLoad()
{
    v_Model::updateAfterLoad();
    if (mOtceps){
        QList<m_Otcep *> l=mOtceps->all_otceps();
        foreach (m_Otcep *m, l) {
            v_Otcep *v=new v_Otcep(this);
            v->setModelObject(m);
            lOtcep.push_back(v);
        }
    }
    doPropertyChanged();
}

v_RC *v_Otceps::m_RC2v_RCV(m_Base *m)
{
    if (mm2v.isEmpty()){
        if (qobject_cast<v_Base*>(parent())!=nullptr){
            QList<v_RC*> lRC=(qobject_cast<v_Base*>(parent()))->findChildrenVisual<v_RC *>();
            foreach (v_RC* vrc, lRC) {
                mm2v[vrc->modelObject()]=vrc;
            }
        }
    }
    if (mm2v.contains(m)) return mm2v[m];
    return nullptr;
}

void v_Otceps::calculateGeometry()
{
    R=QRectF(0,0,gridDX()*4,gridDY()*4);
}

void v_Otceps::d_paint(QPainter *painter, const QStyleOptionGraphicsItem *)
{
    if (ISEDITORMODE){
        QBrush B=QBrush(Qt::NoBrush);
        QPen   P=QPen(Qt::black);
        painter->setPen(P);painter->setBrush(B);
        painter->drawRect(R);
        painter->drawText(R, Qt::AlignCenter,objectName());
    }
}

void v_Otceps::updateStates()
{
    v_Base::updateStates();
    // перекидывем отображение на скриин
    if (lOtcep.isEmpty()) return;
    if (lOtcep.first()->getProxyGraphicsItem()->parentItem()==getProxyGraphicsItem()->parentItem()) return;
    foreach (v_Otcep*v, lOtcep) {
        v->getProxyGraphicsItem()->setParentItem(getProxyGraphicsItem()->parentItem());
    }
}

void v_Otceps::doPropertyChanged()
{
    v_Base::doPropertyChanged();
    foreach (v_Otcep*v, lOtcep) {
        v->width=FSIZE.width()*gridDX();
        v->height=FSIZE.height()*gridDY();
        v->direct=FDIRECT_D0;
        v->calculateGeometry();
        v->update();
    }

}
