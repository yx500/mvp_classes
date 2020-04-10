#ifndef V_RC_H
#define V_RC_H

#include <QMetaProperty>

#include "v_model.h"
#include "mvp_enums.h"

class LIBSHARED_EXPORT v_RC : public v_Model
{
    Q_OBJECT


    Q_PROPERTY(QPointF M1 READ M1 WRITE setM1 DESIGNABLE true)
    Q_PROPERTY(QPointF M2 READ M2 WRITE setM2 DESIGNABLE true)
    Q_PROPERTY(QPointF MD1 READ MD1 WRITE setMD1 DESIGNABLE true)
    Q_PROPERTY(QPointF MD2 READ MD2 WRITE setMD2 DESIGNABLE true)
    Q_PROPERTY(MVP_Enums::TRCSrezType MSRZ1 READ MSRZ1 WRITE setMSRZ1 DESIGNABLE true)
    Q_PROPERTY(MVP_Enums::TRCSrezType MSRZ2 READ MSRZ2 WRITE setMSRZ2 DESIGNABLE true)

    Q_PROPERTY(MVP_Enums::TStikType STIK1 READ STIK1 WRITE setSTIK1 DESIGNABLE true)
    Q_PROPERTY(MVP_Enums::TStikType STIK2 READ STIK2 WRITE setSTIK2 DESIGNABLE true)
    Q_PROPERTY(int     SH READ SH WRITE setSH DESIGNABLE true)
    Q_PROPERTY(QString TEXT READ TEXT WRITE setTEXT DESIGNABLE true)



public:
//    static const VisualGroupProperty_int groupProp_SHRC;
//    static const VisualGroupProperty_int groupProp_TEXTH_STREL;
//    static const VisualGroupProperty_int groupProp_STIKW;

    enum TCheckP{tcp_any,tcp_onlyGreat0,tcp_onlyGreat_0 };


    MYPROP(MVP_Enums::TTextPos,TEXTPOS)

    MYPROP(int, TEXTH)
    MYPROP(QPointF, TEXTM)
    MYPROP(qreal, STIKW)
    MYPROP(MVP_Enums::TDirect,  DIRECT_D0)
    MYPROP(bool,  TRAIN_NUM_BY_END)
    MYPROP(int, PODSV_MARSH)
    MYPROP(int, PODSV_BLOK)

    void setM1(QPointF p){setM(&FM[1],p,tcp_onlyGreat_0,tcp_any);}
    void setM2(QPointF p){setM(&FM[2],p,tcp_onlyGreat_0,tcp_any);}
    void setMD1(QPointF p){setM(&FMD[1],p,tcp_onlyGreat_0,tcp_any);}
    void setMD2(QPointF p){setM(&FMD[2],p,tcp_onlyGreat_0,tcp_any);}
    QPointF M1()const {return FM[1];}
    QPointF M2()const {return FM[2];}
    QPointF MD1()const {return FMD[1];}
    QPointF MD2()const {return FMD[2];}
    void setMSRZ1(MVP_Enums::TRCSrezType p){setMSRZ(0,p);}
    void setMSRZ2(MVP_Enums::TRCSrezType p){setMSRZ(1,p);}
    MVP_Enums::TRCSrezType MSRZ1()const {return FMSRZ[0];}
    MVP_Enums::TRCSrezType MSRZ2()const {return FMSRZ[1];}
    void setSTIK1(MVP_Enums::TStikType p){setSTIK(0,p);}
    void setSTIK2(MVP_Enums::TStikType p){setSTIK(1,p);}
    MVP_Enums::TStikType STIK1()const {return FSTIK[0];}
    MVP_Enums::TStikType STIK2()const {return FSTIK[1];}
    void setSH(int p);
    int SH()const {return FSH;}
    void setTEXT(QString p);
    QString TEXT()const {return Ftext;}
public:

    MYSTATE(bool,STATE_SHTRIH_PLUS)
    MYSTATE(bool,STATE_SHTRIH_MINUS)
    MODEL_STATE(MVP_Enums::TRCBusy,STATE_BUSY)
    MYSTATE(bool,STATE_IN_MARSH)
    MYSTATE(bool,STATE_IN_BLOCK_AREA)

    DECLARECOLOR(rc_busy)
    DECLARECOLOR(rc_free)
    DECLARECOLOR(rc_in_marsh)
    DECLARECOLOR(rc_busy_unknow)
    DECLARECOLOR(rc_blok)
    DECLARECOLOR(rc_blok_tu)



    Q_INVOKABLE v_RC(v_Base *parent = 0);
    virtual ~v_RC(){}

    virtual void setModelObject(BaseObject *B);

    virtual void resetStates();

    virtual void calculateGeometry(); // перестраивает внтренние координаты

    virtual QPolygonF centreLine() const; // возвращает осевую лдя елемента

    virtual QMap<int, QPointF> getMarkersInfo() const;
    virtual void moveMarker(int markerId,QPointF xyItem);


    virtual void updateStates();

protected:

    QPolygonF P[4];
    QPolygonF PD[4];

    QPointF FM[4];  // в координатах сетки
    QPointF FMD[4]; // в координатах сетки
    int FSH;        // ширина в проперти
    qreal S;        // ширина полученная
    QPointF M[4];   // в координатах элемента
    QPointF MD[4];  // в координатах элемента
    QPointF MDs[4]; // в координатах элемента
    MVP_Enums::TStikType FSTIK[3];
    MVP_Enums::TRCSrezType FMSRZ[3];
    QLineF LSTIK[3];
    qreal stik_width;

    QPolygonF FULLPL;
    QVector<int> vFULLPL_CL;

    // текст
    QString Ftext; // PROPERTY
    QString text;  // полученый

    QRectF  boundrect_text;
    int     textH;

    QRectF ZablokR; // блокировка


    bool bPD1,bPD2,bPD3;
    QPointF rD1_1,rD1_2;
    QPointF rD2_1,rD2_2;
    QPointF rD3_1,rD3_2;
    QPointF Z1,r21_1;

    void calculateP123();

    void setM(QPointF *F,QPointF p,TCheckP xc=tcp_any,TCheckP yc=tcp_any);
    void setSTIK(int i, MVP_Enums::TStikType p);
    void setMSRZ(int i, MVP_Enums::TRCSrezType p);
    void assign_M_MD_S_MT();


    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    void showStik(QPainter *painter, QLineF L, MVP_Enums::TStikType t);
    qreal StikD(int t);
    void drawColoredPolygonF(QPainter *painter, QPolygonF &P, QColor brushColor, QVector<QColor> &vC, qreal penWidth=1) const;
    void drawColored0PolygonF(QPainter *painter,QPolygonF &P,QVector<int> &vCL) const;
    virtual void setTextBoundRect();
    virtual void showTEXT(QPainter *painter);
    void showZABLOK(QPainter *painter,bool tu);
    virtual void showSHTRIH(QPainter *painter,MVP_Enums::TStrelPol pol);
    void showStrelka(QPainter * painter,
                     QColor clrB, QColor clrP);
    QColor busyColor();

    virtual void updateStatesFromModel();
    virtual void updateSTATE_IN_MARSH();
    void updateSTATE_IN_BLOCK_AREA();
};


//Q_DECLARE_METATYPE(v_RC::TSrezType)
//Q_DECLARE_METATYPE(v_RC::TStikType)
//Q_DECLARE_METATYPE(v_RC::TTextPos)


#endif // V_RC_H
