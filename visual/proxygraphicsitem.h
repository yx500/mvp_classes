#ifndef PROXYGRAPHICSITEM_H
#define PROXYGRAPHICSITEM_H

#include <QGraphicsItem>
#include <libshared_global.h>
class v_Base;

class LIBSHARED_EXPORT ProxyGraphicsItem : public  QGraphicsItem
{
public:
    explicit ProxyGraphicsItem(v_Base *vBase);
    virtual ~ProxyGraphicsItem();
    enum { Type = UserType + 1 };

       int type() const
       {
           // Enable the use of qgraphicsitem_cast with this item.
           return Type;
       }

       // Drawing
       virtual QRectF boundingRect() const ;
       virtual QPainterPath shape() const;
       virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
       virtual void advance(int phase);
       virtual void drawBackground(QPainter * painter)  ;
       virtual void drawForeground(QPainter *painter);
       v_Base * getvBase(){return vBase;}
       void prepareGeometryChange();

       void set_extShape(QPainterPath p);
       void set_extBoundingRect(QRectF p);
       void drawSelected(QPainter *painter);





signals:

public slots:
protected:
       v_Base *vBase;
       QPainterPath extShape;
       QRectF       extBoundingRect;
       QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // PROXYGRAPHICSITEM_H
