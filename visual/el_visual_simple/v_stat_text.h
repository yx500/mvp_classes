#ifndef V_STAT_TEXT_H
#define V_STAT_TEXT_H

#include "v_base.h"
#include "mvp_enums.h"

class LIBSHARED_EXPORT v_StaticText : public v_Base
{
    Q_OBJECT

public:

    MYPROP(MVP_Enums::TTextFlags, FLAGS)
    MYPROP(int,SH)
    MYPROP(QColor,COLOR_TEXT)
    MYPROP(QFont,FONT)

    MYPROP(bool,ISSHOWRECT)
    MYPROP(QSizeF,SIZEALLIGNRECT)
    MYPROP(QColor,COLOR_RECT)
    MYPROP(bool,ROUND_RECT)
    MYPROP(bool,SH_IN_RECT)


public:

    Q_INVOKABLE v_StaticText(v_Base *parent = nullptr);
    virtual ~v_StaticText(){}


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
