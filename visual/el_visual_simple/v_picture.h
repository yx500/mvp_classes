#ifndef V_PICTURE_H
#define V_PICTURE_H

#include "v_base.h"
#include <QtSvg>

class v_Picture : public v_Base
{
    Q_OBJECT
    MYPROP(QSizeF,SIZE)
    MYPROP(bool,FRAME)
    Q_PROPERTY(QString FN READ FN WRITE setFN DESIGNABLE true )
    Q_PROPERTY(QString SVG_XML READ SVG_XML WRITE setSVG_XML DESIGNABLE true )

public:
    Q_INVOKABLE v_Picture(v_Base *parent = 0);
    virtual ~v_Picture();
    QString FN() const {return FFN;}
    void setFN(QString p);
    QString SVG_XML() const {return FSVG_XML;}
    void setSVG_XML(QString p);



    virtual void calculateGeometry(); // перестраивает внтренние координаты



protected:

    QRectF  allign_rect;
    QRectF  boundrect_text;

    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);

private:
    QPointF M[1];
    QRectF pictureRect;
    QString FFN;
    QString FSVG_XML;
    QImage image;
    QSvgRenderer svgRenderer;
    qreal levelOfDetail;
    QRectF mappedR;
signals:

public slots:


};

#endif // V_PICTURE_H
