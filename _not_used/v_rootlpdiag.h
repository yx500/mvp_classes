#ifndef V_ROOTLPDIAG_H
#define V_ROOTLPDIAG_H

#include "visualelements.h"
#include <QPolygonF>
class v_LPDiag;
struct netPoint;
class v_RootLPDiag : public VisualElements
{
    Q_OBJECT
public:
public:
    Q_INVOKABLE v_RootLPDiag(QObject *parent = 0);
    virtual ~v_RootLPDiag(){}
    virtual void updateAfterLoad();

    bool makeLine(v_LPDiag *LP1,int addr1,int addr2,QPolygonF *LNS,QPolygonF *LNG);
    void updateLines(int addr_krug);
    v_LPDiag *getLPDiag(int addkrug, int addr, int &n);
    bool isIntersect(int addr_krug, QPolygonF &p);
    int sovpad(int addr_krug, QPolygonF &PL1);

protected:
 // QList <netPoint*> lnp;
};

#endif // V_ROOTLPDIAG_H
