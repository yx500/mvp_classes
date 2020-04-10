#ifndef V_IP_TEXT_H
#define V_IP_TEXT_H

#include "v_stat_text.h"
#include "signaldescription.h"

class v_IP_Text : public v_StaticText
{
    Q_OBJECT
    Q_PROPERTY(SignalDescription IPSIGNAL READ IPSIGNAL WRITE setIPSIGNAL DESIGNABLE true )
public:
    MYPROP(int,TO)
    SignalDescription IPSIGNAL() const {return FIPSIGNAL;}
    void setIPSIGNAL(SignalDescription p);
public:
    Q_INVOKABLE v_IP_Text(v_Base *parent = nullptr);
    virtual ~v_IP_Text(){}

    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void advance(int phase);
    virtual void updateAfterLoad();// восстановление параметров после загрузки(линки)


signals:

public slots:
protected:
    SignalDescription FIPSIGNAL;
    quint64 tick;
    QTime tick_time;
};

#endif // V_IP_TEXT_H
