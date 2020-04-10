#include "mvp_scene.h"

#include <QGraphicsSceneMouseEvent>
#include <QCoreApplication>
#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QLineEdit>
#include <QToolButton>

#include "qpfunction.h"
#include "v_model.h"
#include <qmath.h>

#include "groupbaseobjects.h"
#include "baseobjecttools.h"
#include "mcolorpalette.h"

#include "v_widget.h"
#include "visualrootgroup.h"


MVP_Scene::MVP_Scene(QObject *parent):
    QGraphicsScene(parent)
{

    FVisual_Screen=nullptr;

    FDrawMode.isEditor=false;
    timerAnimate=new QTimer(this);
    timerInvalidate=new QTimer(this);
    timerUpdateState=new QTimer(this);
    timerAnimate->setInterval(100);
    timerInvalidate->setInterval(1000);
    timerUpdateState->setInterval(1000);
    connect(timerAnimate, SIGNAL(timeout()), this, SLOT(slotAnimate()));
    connect(timerInvalidate, SIGNAL(timeout()), this, SLOT(slotInvalidate()));
    connect(timerUpdateState, SIGNAL(timeout()), this, SLOT(slotUpdateState()));
    MColorPalette::instance()->updateAfterLoad();
    setBackgroundBrush(QBrush(MColorPalette::instance()->color(v_Base::Color_FON)));
}

MVP_Scene::~MVP_Scene()
{
    removeAllItems();
}

void MVP_Scene::setVisual_Screen(Visual_Screen *p)
{
    timerAnimate->stop();
    timerInvalidate->stop();
    timerUpdateState->stop();
    removeAllItems();
    FVisual_Screen=p;
    if (FVisual_Screen) {
        FVisual_Screen->activate();
        addItem(FVisual_Screen->getProxyGraphicsItem());
        addWigets();
        timerAnimate->start();
        timerInvalidate->start();
        timerUpdateState->start();
    }
}


void MVP_Scene::addWigets()
{
    if (FVisual_Screen)    {
        QList <v_Widget *> lw=FVisual_Screen->findChildren<v_Widget *>();
        foreach (v_Widget *vW,lw){
            QWidget *W=getWidget(vW->TYPE(),vW->idstr(),vW->PROPERTYES());
            if (W){
                vW->PW->setWidget(W);
                QGraphicsProxyWidget *PW=vW->PW;
                //QGraphicsProxyWidget *PW=addWidget(W);
                //                QString prop=vW->PROPERTYES();
                //                QStringList lprops=prop.split(";");
                //                foreach (QString propkv, container) {

                //                }


                PW->setParentItem(vW->getProxyGraphicsItem());
                QRectF rw=PW->boundingRect();
                rw.moveCenter(QPointF(0,0));
                PW->setPos(rw.left(),rw.top());
                if (vW->SCALETOSIZE()){
                    QRectF ri=vW->boundingRect();
                    qreal mx=1.*ri.width()/rw.width();
                    qreal my=1.*ri.height()/rw.height();
                    qreal mm=mx;
                    if (mm>my) mm=my;
                    PW->setScale(mm);
                    rw=PW->boundingRect();
                    rw.moveCenter(QPointF(0,0));
                    PW->setPos(rw.left()*mm,rw.top()*mm);
                }

            }
        }
    }
}

QWidget *MVP_Scene::getWidget(QString type,QString idstr,QString propertyes)
{
    QWidget *W=nullptr;
    if (type=="QToolButton"){
        W=new QToolButton();
    }
    if (type=="QLineEdit"){
        W=new QLineEdit();
    }
    Q_UNUSED(idstr);Q_UNUSED(propertyes);
//    QLineEdit *numberEdit = new QLineEdit;
//    return numberEdit;
    return W;
}

QList<v_Base *> MVP_Scene::selectedV()
{
    QList<v_Base *> l;
    QList<QGraphicsItem*>s=selectedItems();
    foreach (QGraphicsItem *item,s){
        ProxyGraphicsItem * PV=qgraphicsitem_cast<ProxyGraphicsItem *>(item);
        if (PV!=nullptr)
                l.push_back(PV->getvBase());
     }
    return l;
}

QList<v_Base *> MVP_Scene::itemsV(const QPointF &pos, Qt::ItemSelectionMode mode, Qt::SortOrder order, const QTransform &deviceTransform) const
{
    QList<v_Base *> l;
    QList<QGraphicsItem*>s=items(pos, mode, order, deviceTransform);
    foreach (QGraphicsItem *item,s){
        ProxyGraphicsItem * PV=qgraphicsitem_cast<ProxyGraphicsItem *>(item);
        if (PV!=nullptr)
                l.push_back(PV->getvBase());
     }
    return l;
}

QList<v_Base *> MVP_Scene::itemsV(Qt::ItemSelectionMode mode, Qt::SortOrder order) const
{
    Q_UNUSED(mode)
    QList<v_Base *> l;
    QList<QGraphicsItem*>s=items(order);
    foreach (QGraphicsItem *item,s){
        ProxyGraphicsItem * PV=qgraphicsitem_cast<ProxyGraphicsItem *>(item);
        if (PV!=nullptr)
                l.push_back(PV->getvBase());
     }
    return l;
}

bool MVP_Scene::isMyItem(QObject *O)
{
    v_Base * V=qobject_cast<v_Base *>(O);
    if (V){
        if (items().indexOf(V->getProxyGraphicsItem())>=0) return true;
    }
    return false;
}

v_Base *MVP_Scene::ItemByBaseModelElement(QObject *B)
{
    if (B==nullptr) return nullptr;
    foreach (QGraphicsItem *item,items()){
        ProxyGraphicsItem * PV=qgraphicsitem_cast<ProxyGraphicsItem *>(item);
        if (PV){
            v_Model * vm=qobject_cast<v_Model *>(PV->getvBase());
            if ((vm!=nullptr)&&(vm->modelObject()==B)) return vm;
        }
     }
    return nullptr;
}

void MVP_Scene::removeAllItems()
{
    foreach (QGraphicsItem* I, items()) {
        removeItem(I);
    }
}

void MVP_Scene::setDrawModeisEditor(bool isEditor)
{
    if (FVisual_Screen){
        FVisual_Screen->setSceneEditorMode(isEditor);
        FVisual_Screen->reCalculateGeometry();
        FVisual_Screen->getProxyGraphicsItem()->update();
    }
    if (isEditor) {
        foreach (QGraphicsItem *I, items()) {
            I->setVisible(true);
        }
    }
    FDrawMode.isEditor=isEditor;


}

void MVP_Scene::drawBackground(QPainter *painter, const QRectF &rect)
{
    drawFon(painter, rect);
    QList<QGraphicsItem *> l=items(rect,Qt::IntersectsItemBoundingRect);
    foreach (QGraphicsItem *item, l) {
        ProxyGraphicsItem * pv=qgraphicsitem_cast<ProxyGraphicsItem *>(item);
        if (pv){
            painter->save();
            painter->setMatrix(pv->sceneMatrix(), true);
            painter->setMatrix(pv->matrix(), true);
            pv->drawBackground(painter);
            customDrawBackground(painter,pv);
            painter->restore();
        }
    }

}

void MVP_Scene::drawForeground(QPainter *painter, const QRectF &rect)
{
    QList<QGraphicsItem *> l=items(rect,Qt::IntersectsItemBoundingRect);
    foreach (QGraphicsItem *item, l) {
        ProxyGraphicsItem * pv=qgraphicsitem_cast<ProxyGraphicsItem *>(item);
        if (pv){
            painter->save();
            painter->setMatrix(pv->sceneMatrix(), true);
            painter->setMatrix(pv->matrix(), true);
            pv->drawForeground(painter);
            customDrawForeground(painter,pv);
            painter->restore();
        }
    }
}

void MVP_Scene::drawFon(QPainter *painter, const QRectF &rect)
{
    QGraphicsScene::drawBackground(painter, rect);
}

void MVP_Scene::customDrawForeground(QPainter *, QGraphicsItem *)
{

}

void MVP_Scene::customDrawBackground(QPainter *, QGraphicsItem *)
{

}







void MVP_Scene::slotInvalidate()
{
    // медленная прорисовка на случай не отрабатывания update
    invalidate();
}

void MVP_Scene::slotAnimate()
{
            advance();
}

void MVP_Scene::slotUpdateState()
{
//    // обновляем свойства от моделей
//    // медленное обновление на случай не отрабатывания увязки модель-> отображение

//    foreach (QGraphicsItem *item,items()){
//        ProxyGraphicsItem * PV=qgraphicsitem_cast<ProxyGraphicsItem *>(item);
//        if (PV){
//            v_Base* v=PV->getvBase();
//            if (v) v->updateStates();
//            v_Model * vm=qobject_cast<v_Model *>(v);
//            if (vm) vm->updateStatesFromModel();
//        }
//     }
}


int MVP_Scene::updateStateInterval() const
{
    if (timerUpdateState->isActive())
        return timerUpdateState->interval();
    return 0;
}
void MVP_Scene::setUpdateStateInterval(int p)
{
    if (p==0) {
        timerUpdateState->stop();
    } else {
        timerUpdateState->start(p);
    }
}

