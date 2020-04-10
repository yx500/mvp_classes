#ifndef V_STRELKA_H
#define V_STRELKA_H

#include "v_rc.h"

class v_Strelka : public v_RC
{
    Q_OBJECT

    Q_PROPERTY(QPointF M3 READ M3 WRITE setM3 DESIGNABLE true)
    Q_PROPERTY(QPointF MD3 READ MD3 WRITE setMD3 DESIGNABLE true)
    Q_PROPERTY(MVP_Enums::TStikType STIK3 READ STIK3 WRITE setSTIK3 DESIGNABLE true)
    Q_PROPERTY(MVP_Enums::TRCSrezType MSRZ3 READ MSRZ3 WRITE setMSRZ3 DESIGNABLE true)
public:
    MYPROP(bool, INVERSPOL)
    MYPROP(bool,SHOWPLUSPOL)
    MYPROP(int, PODSV_MARSH_M1)


    QPointF M3()const {return FM[3];}
    QPointF MD3()const {return FMD[3];}
    MVP_Enums::TStikType STIK3()const {return FSTIK[2];}
    void setM3(QPointF p){setM(&FM[3],p,tcp_onlyGreat_0,tcp_onlyGreat0);}
    void setMD3(QPointF p){setM(&FMD[3],p,tcp_onlyGreat_0,tcp_any);}
    void setSTIK3(MVP_Enums::TStikType p){setSTIK(2,p);}
    void setMSRZ3(MVP_Enums::TRCSrezType p){setMSRZ(2,p);}
    MVP_Enums::TRCSrezType  MSRZ3()const {return FMSRZ[2];}
public:

    DECLARECOLOR(strel_text_plus)
    DECLARECOLOR(strel_text_minus)
    DECLARECOLOR(strel_text_w)

    MYSTATE(MVP_Enums::TStrelPol,STATE_POL)
    MYSTATE(bool,STATE_BLOCK)


    Q_INVOKABLE v_Strelka(v_Base *parent = nullptr);
    virtual ~v_Strelka(){}
    virtual void setModelObject(BaseObject *B);
    void resetStates();

    virtual void calculateGeometry(); // перестраивает внтренние координаты
    virtual QPolygonF centreLine() const; // возвращает осевую лдя елемента

    virtual QMap<int, QPointF> getMarkersInfo() const;
    virtual void moveMarker(int markerId,QPointF xyItem);

signals:

public slots:
protected:
    QPointF Z2,r13_1,r32_1;  // пересечения!!!
    QPointF z13_1,z13_2,z12_1,z12_2,z32_1,z32_2,z23_1,z23_2;
    void showStrelka(QPainter * painter,
                     MVP_Enums::TStrelPol pol,
                     QColor clrB1,QColor clrB2,QColor clrP1,QColor clrP2);
    void showUkPolPlus(QPainter * painter);


    virtual QPainterPath shape() const;
    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    void drawBackground(QPainter *painter);
    virtual void showTEXT(QPainter *painter);
    virtual void setTextBoundRect();

    virtual void updateStatesFromModel();
    virtual void updateSTATE_IN_MARSH();


    virtual void showSHTRIH(QPainter *painter,MVP_Enums::TStrelPol pol);
    MVP_Enums::TStrelPol getDrawPol()const ;

    QFont font;

private:

    QPolygonF PLUSPL[2]; // полигоны плюса
    QVector<int> vPLUSPL_CL[2];
    QPolygonF MINUSPL[2];// полигоны минуса
    QVector<int> vMINUSPL_CL[2];

    QPolygonF UKPOLPLUSPL; // указатель положения
    QPolygonF UKPOLMINUSPL;


};




#endif // V_STRELKA_H
