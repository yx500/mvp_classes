#ifndef V_DSO_H
#define V_DSO_H

#include "v_model.h"
#include <QElapsedTimer>
class m_DSO;

class v_DSO : public v_Model
{
    Q_OBJECT
    Q_PROPERTY(int     SH READ SH WRITE setSH DESIGNABLE true)
    Q_PROPERTY(int     SHRC READ SHRC WRITE setSHRC DESIGNABLE true)
public:
    MYSTATE(int,STATE_SRAB)
    MYSTATE(qlonglong,STATE_OSY_COUNT)
    MYSTATE(int, STATE_ERROR)


    void setSH(int p);
    int SH(){return FSH;}
    void setSHRC(int p);
    int SHRC(){return FSHRC;}
public:
    DECLARECOLOR(dso_free)
    DECLARECOLOR(dso_busy)
    DECLARECOLOR(dso_error)

    Q_INVOKABLE v_DSO(v_Base *parent = 0);
    virtual ~v_DSO(){}
    virtual void resetStates();
    virtual void updateAfterLoad();

    virtual void calculateGeometry();



signals:

public slots:
    void slotModelStateChanged(QObject *O);
private:
    int FSH,FSHRC;
    qreal S,SRC;
    QPointF M[1];
    QPointF mt;
    QRectF  boundrect_text;

    QPolygonF PP;
    QPolygonF P[2];

protected:
    int lastOsyCount;
    void showText(QPainter *painter);
    QElapsedTimer dtStartShowText;

    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void updateStatesFromModel();
    m_DSO* _m_DSO;
};

#endif // V_DSO_H
