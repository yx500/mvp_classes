#ifndef V_CHANELSOSTARROW_H
#define V_CHANELSOSTARROW_H

#include "v_arrow.h"
#include "signaldescription.h"

class v_ChanelSostArrow : public v_Arrow
{
    Q_OBJECT
    Q_PROPERTY(SignalDescription SIGNAL_1 READ SIGNAL_1 WRITE setSIGNAL_1 DESIGNABLE true ) \
    MYPROP(QString,IP)
    Q_ENUMS(TAnimeSrc)
    MYPROP(QColor,COLOR_33)
    MYPROP(bool,OPACITY_ANIMATE)
    Q_PROPERTY(TAnimeSrc  ANIMATE_SRC READ ANIMATE_SRC WRITE seANIMATE_SRC DESIGNABLE true)

public:
    enum TAnimeSrc{change_tick,change_data};
    TAnimeSrc  ANIMATE_SRC() const {return FANIMATE_SRC;}
    void seANIMATE_SRC(TAnimeSrc p){FANIMATE_SRC=p;}

    SignalDescription SIGNAL_1() const {return FSIGNAL_1;}
    void setSIGNAL_1(SignalDescription p);
public:
    Q_INVOKABLE v_ChanelSostArrow(v_Base *parent = nullptr);
    virtual ~v_ChanelSostArrow(){}

    virtual void setModelObject(BaseObject *B);
    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void advance(int phase);

signals:

public slots:
protected:
    quint64 old_TICKDATACHANGED;
    quint64 old_TICK;
    TAnimeSrc  FANIMATE_SRC;
    SignalDescription FSIGNAL_1;
    bool STATE_IS_EXISTS() ;

};

Q_DECLARE_METATYPE(v_ChanelSostArrow::TAnimeSrc)

#endif // V_CHANELSOSTARROW_H
