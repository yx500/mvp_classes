#ifndef V_STRELKA_GAC_H
#define V_STRELKA_GAC_H

#include "v_strelka.h"
#include "mvp_enums.h"

class v_Strelka_GAC : public v_Strelka
{
    Q_OBJECT
public:
    MYPROP(MVP_Enums::TGroupBool,SHOWIPDRTDS)

    DECLARECOLOR(strel_prp_p)
    DECLARECOLOR(strel_prp_b)
public:

    Q_INVOKABLE v_Strelka_GAC(v_Base *parent = nullptr);
    virtual ~v_Strelka_GAC(){}

    virtual void resetStates();

    virtual void calculateGeometry(); // перестраивает внтренние координаты
    virtual void updateState();

signals:

public slots:
protected:
    QPolygonF PRPPL; // указатель ПРП
    QPolygonF PRMPL;
    QTime dtPrOff[2];
    void showPRPPRM(QPainter * painter);
    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);

};

#endif // V_STRELKA_GAC_H
