#ifndef V_WIDGET_H
#define V_WIDGET_H

#include "v_base.h"

class v_Widget : public v_Base
{
    Q_OBJECT
    Q_PROPERTY(QString TYPE READ TYPE WRITE setTYPE DESIGNABLE true ) \
    MYPROP(QSizeF,SIZE)
    MYPROP(QString,PROPERTYES)
    MYPROP(bool,SCALETOSIZE)
public:

    QGraphicsProxyWidget *PW;

    Q_INVOKABLE v_Widget(v_Base *parent = nullptr);
    virtual ~v_Widget(){}

    void setTYPE(QString p);
    QString TYPE() const{return FTYPE;}

    virtual void calculateGeometry(); // перестраивает внтренние координаты



signals:

public slots:
protected:
    QString FTYPE;
    QRectF rect;
    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);


};

#endif // V_WIDGET_H
