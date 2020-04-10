#ifndef V_SVET_H
#define V_SVET_H

#include "v_model.h"


class LIBSHARED_EXPORT v_Svet : public v_Model
{
    Q_OBJECT
    Q_ENUMS(TColorOpen)
    Q_ENUMS(TColorClose)
    Q_PROPERTY(int     SH READ SH WRITE setSH DESIGNABLE true)

public:
    MYPROP(bool,MANEVROV)
    MYPROP(QString,TEXT)


    enum TColorOpen {clr_green=0,clr_white,clr_yellow};
    enum TColorClose {clr_fon=0,clr_red,clr_blue} ;
    void setSH(int p);
    int SH(){return FSH;}


public:
    //static const VisualGroupProperty_int groupProp_SHSVET;

    DECLARECOLOR(svet_zel)
    DECLARECOLOR(svet_bel)
    DECLARECOLOR(svet_kra)
    DECLARECOLOR(svet_jelt)
    DECLARECOLOR(svet_siniy)
    DECLARECOLOR(svet_zel_mig)
    DECLARECOLOR(svet_bel_mig)
    DECLARECOLOR(svet_kra_mig)
    DECLARECOLOR(svet_jelt_mig)
    DECLARECOLOR(svet_siniy_mig)



    Q_INVOKABLE v_Svet(v_Base *parent = 0);
    virtual ~v_Svet(){}

    virtual void calculateGeometry(); // перестраивает внтренние координаты


protected:

    int FSH;
    qreal S;
    QPolygonF FULLP;
    QRectF  boundrect_text;
    void show_text(QPainter *painter);

    QRectF ZablokR;
    void showZABLOK(QPainter *painter);

    virtual void calculateGeometry_T();
    QPointF M[1];
    QPointF T[4];
    QRectF RSV;
    QFont font;

    virtual void updateStatesFromModel();


private:
signals:

public slots:


};

#endif // V_SVET_H
