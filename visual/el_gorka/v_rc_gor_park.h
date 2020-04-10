#ifndef V_RC_GOR_PARK_H
#define V_RC_GOR_PARK_H

#include "v_rc.h"

class v_RC_Gor_Park : public v_RC
{
    Q_OBJECT
    MYPROP(int,WIDTH)
    MYPROP(int,SHRCKZP)
    MYPROP(int,MASHTAB_MAX_LEN)
    MYPROP(QPointF,TEXT_SVOB_XY)
    MYPROP(int,TEXT_SVOB_HEIGHT)
    MYPROP(bool,USL_VAGON)
    MYSTATE(int,STATE_KZP_D)
    MYSTATE(int,STATE_KZP_E)
    MYSTATE(qreal,STATE_KZP_V)
    MYSTATE(int,STATE_KZP_P)
public:

    Q_INVOKABLE v_RC_Gor_Park(v_Base *parent = 0);
    virtual ~v_RC_Gor_Park(){}
    void resetStates();
    void updateAfterLoad();
    virtual void calculateGeometry();
    virtual QPolygonF centreLine() const;
    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void advance(int phase);

signals:

public slots:
protected:
    virtual void updateStatesFromModel();
    qreal len_rc;
    QRectF text_sv_rect;
    qreal k_mashtab;
    int width_abs,width_mashtab;
    QString valueString() const;
    bool isUSL_VAGON;
    QTime animateT;
    qreal animateK;
    QFont font;

private:
};

#endif // V_RC_GOR_PARK_H
