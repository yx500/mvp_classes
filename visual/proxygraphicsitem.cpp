#include <QGraphicsScene>
#include "proxygraphicsitem.h"
#include "v_base.h"

ProxyGraphicsItem::ProxyGraphicsItem(v_Base *vBase) :
    QGraphicsItem(),
    extShape()
{
    this->vBase=vBase;
    extBoundingRect=QRectF();
    setToolTip(vBase->objectName());

}

ProxyGraphicsItem::~ProxyGraphicsItem()
{
    // удалит всех детей
    QList<QGraphicsItem *> l=childItems();
    foreach (QGraphicsItem *I, l) {
        I->setParentItem(nullptr);
    }
    if (scene()) scene()->removeItem(this);
    this->vBase=nullptr;
}

QRectF ProxyGraphicsItem::boundingRect() const
{
    QRectF R=vBase->boundingRect();
    if(!extBoundingRect.isNull())
        R=R.united(extBoundingRect);
    if (!extShape.isEmpty()){
        QPolygonF P=extShape.toFillPolygon();
        R=R.united(P.boundingRect());
    }
    return R;
}

QPainterPath ProxyGraphicsItem::shape() const
{
    QPainterPath path=vBase->shape();
    path.setFillRule(Qt::WindingFill);
    if (!extShape.isEmpty()){
        if (path.intersects(extShape)) {
            path=path.united(extShape);
        } else {
            path.setFillRule(Qt::WindingFill);
            path.addPath(extShape);
        }
    }
    return path;
}

void ProxyGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    vBase->paint(painter, option, widget);
}

void ProxyGraphicsItem::advance(int phase)
{
    vBase->advance(phase);
    if ((isSelected())||(vBase->STATE_MK_FLAG()!=0)) vBase->update();

}

void ProxyGraphicsItem::drawBackground(QPainter *painter)
{
    vBase->drawBackground(painter);
}

void ProxyGraphicsItem::drawForeground(QPainter *painter)
{
    vBase->drawForeground(painter);
    if (isSelected())
        drawSelected(painter);
}

void ProxyGraphicsItem::prepareGeometryChange()
{
    QGraphicsItem::prepareGeometryChange();
}

void ProxyGraphicsItem::set_extShape(QPainterPath p)
{
    extShape=p;
}

void ProxyGraphicsItem::set_extBoundingRect(QRectF p)
{
    extBoundingRect=p;
}

void ProxyGraphicsItem::drawSelected(QPainter *painter)
{
    if (isSelected()){
        //QPolygonF P=shape().toFillPolygon();
        QColor clrB=vBase->getColor(v_Base::Color_object_selected);
        painter->save();
        painter->setBrush(QBrush(Qt::NoBrush));
        painter->setPen(QPen(QBrush(clrB),2,Qt::SolidLine));
        //painter->scale(1.1,1.1);
        QRectF FSELECTRECT=vBase->SELECTRECT();
        if (!FSELECTRECT.isEmpty()){
            QRectF R=QRectF(FSELECTRECT.left()*vBase->gridDX(),
                            FSELECTRECT.top()*vBase->gridDY(),
                            FSELECTRECT.width()*vBase->gridDX(),
                            FSELECTRECT.height()*vBase->gridDY()
                            );
            painter->drawRect(R);
        } else
            painter->drawPath(shape());
        painter->restore();
    }
}

QVariant ProxyGraphicsItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged && scene()) {
        vBase->setSelected(value.toBool());
        vBase->emitStateChanged();
    }
    if (change == ItemVisibleHasChanged && scene()) {
        vBase->setVisible(value.toBool());
        vBase->emitStateChanged();
    }
    return QGraphicsItem::itemChange(change, value);
}

