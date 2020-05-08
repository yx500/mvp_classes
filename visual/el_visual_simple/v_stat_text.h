#ifndef V_STAT_TEXT_H
#define V_STAT_TEXT_H

#include "v_model.h"
#include "mvp_enums.h"

class LIBSHARED_EXPORT v_StaticText : public v_Model
{
    Q_OBJECT

public:

    MYPROP(MVP_Enums::TTextFlags, FLAGS)
    MYPROP(int,SH)
    MYPROP(QColor,COLOR_TEXT)
    MYPROP(QColor,COLOR_RECT)
    MYPROP(QColor,COLOR_BRUSH)
    MYPROP(QFont,FONT)

    MYPROP(bool,ISSHOWRECT)
    MYPROP(QSizeF,SIZEALLIGNRECT)

    MYPROP(bool,ROUND_RECT)
    MYPROP(bool,SH_IN_RECT)


public:

    Q_INVOKABLE v_StaticText(v_Base *parent = nullptr);
    virtual ~v_StaticText(){}

    virtual void updateAfterLoad();

    virtual QString getText(){return objectName();}
    virtual QColor getTextColor(){return FCOLOR_TEXT;}
    virtual QColor getRectColor(){return FCOLOR_RECT;}
    virtual QColor getBrusColor(){return FCOLOR_BRUSH;}

    virtual void calculateGeometry(); // перестраивает внтренние координаты



protected:

    QRectF  allign_rect;
    QRectF  boundrect_text;
    qreal RTEXTH;

    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);

    void drawRect(QPainter *painter);
    void drawText(QPainter *painter);

private:
    QPointF M[1];
signals:

public slots:


};

#endif // V_STAT_TEXT_H
