#ifndef V_BBK_ARS_DIAG_H
#define V_BBK_ARS_DIAG_H

#include "v_stat_text.h"
class GtBuffer;

class v_BBK_ARS_Diag : public v_StaticText
{
    Q_OBJECT
    Q_PROPERTY(QString IP READ IP WRITE setIP DESIGNABLE true )
public:
    MYPROP(int,TO)
    void setIP(QString p);
    QString IP()const {return FIP;}

public:
    Q_INVOKABLE  v_BBK_ARS_Diag(v_Base *parent = 0);
    virtual ~v_BBK_ARS_Diag(){}

    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void advance(int phase);
    virtual void updateAfterLoad();// восстановление параметров после загрузки(линки)

protected:
    QString FIP;
    quint64 old_TICKDATACHANGED;
    quint64 old_TICK;
    GtBuffer *chanel_main;
    GtBuffer *chanel_debug;

    qlonglong tick_debug,tick_main;
    QTime tick_time;

    QString getText(bool &error);
public slots:
};

#endif // V_BBK_ARS_DIAG_H
