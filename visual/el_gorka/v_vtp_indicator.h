#ifndef V_VTP_INDICATOR_H
#define V_VTP_INDICATOR_H

#include "v_base.h"
#include "m_zam.h"

class m_Otcep;
class m_Otceps;
class v_VTP_Indicator : public v_Base
{
    enum{_noDescr=0,inDescr=1,outDescr=2};

    Q_OBJECT
    Q_PROPERTY(ObjectLink ZAM1 READ ZAM1 WRITE setZAM1 DESIGNABLE true )
    Q_PROPERTY(ObjectLink ZAM2 READ ZAM2 WRITE setZAM2 DESIGNABLE true )
    Q_PROPERTY(ObjectLink OTCEPS READ OTCEPS WRITE setOTCEPS DESIGNABLE true )
public:
    MYPROP(qreal,HEIGHT)
    MYPROP(qreal,WIDTH_FACT)
    MYPROP(qreal,WIDTH_VES)
    MYPROP(qreal,WIDTH_ZAD)
    MYPROP(QFont,FONT)
    MYPROP(bool,IS_SMALL)
    MYPROP(QSizeF,SMALL_SIZE)

    DECLARECOLOR(tpind_in)
    DECLARECOLOR(tpind_out)
    DECLARECOLOR(tpind_podl)

    MYSTATES(qreal,STATE_VF)
    MYSTATES(qreal,STATE_VZ)
    MYSTATES(qreal,STATE_VO)
    MYSTATES(int,STATE_ST)
    MYSTATE(int,STATE_INOUT)

    ObjectLink ZAM1() const {return FZAM1;}
    void setZAM1(ObjectLink p);
    ObjectLink ZAM2() const {return FZAM2;}
    void setZAM2(ObjectLink p);
    ObjectLink OTCEPS() const {return FOTCEPS;}
    void setOTCEPS(ObjectLink p);

public:
    Q_INVOKABLE v_VTP_Indicator(v_Base *parent = 0);
    virtual ~v_VTP_Indicator(){}
    virtual void validation(ListObjStr *l) const;
    virtual void updateAfterLoad();
    virtual void calculateGeometry(); // перестраивает внтренние координаты


    virtual void updateStates();

protected:
    ObjectLink FZAM1;
    ObjectLink FZAM2;
    ObjectLink FOTCEPS;
    m_Zam *zam1;
    m_Zam *zam2;
    m_Otceps *mOtceps;
    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    m_Otcep *optcep_on_zam(m_Zam *zam);

    QRectF R;
    QRectF RF;
    QRectF RZ;
    QRectF RV;
    QRectF R_SMALL;

    QString stVZ;
    QString stVF;
    QString stVO;
    QString stVV;

    int inoutDescr;

};

#endif // V_VTP_INDICATOR_H
