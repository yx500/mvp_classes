#ifndef V_ZAM_H
#define V_ZAM_H

#include "v_model.h"
class v_Zam : public v_Model
{
    Q_OBJECT
public:
    MYPROP(int ,SHRC)
    MYPROP(int ,WIDTH)
    MYPROP(int ,HEIGHT)
    MYPROP(int ,X1)
    MYPROP(int ,Y1)

    DECLARECOLOR(zam_tr_st_T14_p)
    DECLARECOLOR(zam_tr_st_T14_b)
    DECLARECOLOR(zam_pr_st_T14_p)
    DECLARECOLOR(zam_pr_st_T14_b)
    DECLARECOLOR(zam_tr_st_R_p)
    DECLARECOLOR(zam_tr_st_R_b)
    DECLARECOLOR(zam_pr_st_R_p)
    DECLARECOLOR(zam_pr_st_R_b)
    DECLARECOLOR(zam_tr_st_Un_p)
    DECLARECOLOR(zam_tr_st_Un_b)
    DECLARECOLOR(zam_pr_st_Un_p)
    DECLARECOLOR(zam_pr_st_Un_b)
    DECLARECOLOR(zam_pr_st_N_p)
    DECLARECOLOR(zam_pr_st_N_b)
    DECLARECOLOR(zam_podl_RRC_p)
    DECLARECOLOR(zam_podl_A_p)
    DECLARECOLOR(zam_podl_A_b)
    DECLARECOLOR(zam_podl_ARRC_p)
    DECLARECOLOR(zam_podl_ARRC_b)

    MYSTATE(bool,STATE_A)
    MYSTATE(bool,STATE_KZ)
    MYSTATE(bool,STATE_OG)
    MYSTATE(bool,STATE_RRC)
    MYSTATE(int,STATE_STUPEN)
    MYSTATE(bool,STATE_MK_RRC)
    MYSTATE(bool,STATE_MK_33)


    Q_INVOKABLE v_Zam(v_Base *parent = 0);
    virtual ~v_Zam(){}

    virtual void resetStates();
    virtual void calculateGeometry();


signals:

public slots:
protected:
    struct TStupPoly{
        QList<QPolygonF> Tr[2];
        QPolygonF pr[2];
    };
    TStupPoly sp[6]; //  нет+ступени+расторможка
    qreal S;
    QPointF M[1];
    QRectF Podl[2];
    QRectF PodlS;
    QRectF ZablokR; // блокировка

    void showOG(QPainter *painter);

    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    void drawBackground(QPainter *painter);
    virtual void updateStatesFromModel();


private:

};

#endif // V_ZAM_H
