#ifndef V_LPDIAG_SHLUZ_H
#define V_LPDIAG_SHLUZ_H

#include "v_lpdiag.h"
class LIBSHARED_EXPORT v_LPDiag_Shluz : public v_LPDiag
{
    Q_OBJECT
public:
    MYPROPG(int,NUM)
    MYPROP(int,ADDR_KRUG)
public:

    Q_INVOKABLE v_LPDiag_Shluz(v_Base *parent = 0);
    virtual ~v_LPDiag_Shluz(){}
    QRectF boundingRect() const;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    virtual void calculateGeometry(); // перестраивает внтренние координаты
    virtual int isAddrExists(int addr);
    virtual QList<QPolygonF> getInOutPolygons(int n,Tinout inout);
    QString getNameLink(int n);

};
#endif // V_LPDIAG_SHLUZ_H
