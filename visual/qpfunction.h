#ifndef QPFUNCTION_H
#define QPFUNCTION_H

#include <QPointF>
#include <QLineF>
#include <QPolygonF>
#include <QFont>
#include "mvp_enums.h"

class QPFunction
{
public:
    enum TArrowType{arrow_none,arrow_normal};
    QPFunction();
    static void pointsOnPerpendikular(QPointF T1,QPointF T2,qreal len1,qreal len2,QPointF &TP1,QPointF &TP2);
    static QPointF pointOnPerpendikular(QPointF T1,QPointF T2,qreal len1);
    static QPointF pointOnLine(QPointF T1,QPointF T2,qreal len1);
    static bool isPointOnLine(QPointF T1,QPointF T2,QPointF TX);
    static bool intersection(QPointF p1, QPointF p2, QPointF p3, QPointF p4,QPointF &ret) ;
    static bool intersectionLines(QPointF p1, QPointF p2, QPointF p3, QPointF p4,QPointF &ret) ;
    static QPointF intersectionPoint(QPointF p1, QPointF p2, QPointF p3, QPointF p4);
    static QPointF intersectionPointHard(QPointF p1, QPointF p2, QPointF p3, QPointF p4);
    static QPolygonF makerect(QPointF T1,QPointF T2,qreal S);
    static bool isPointInSquea(QPointF T,QPointF C,qreal R);
    static QRectF R1inR2(QRectF R1,QRectF R2);
    static void P2PL(QPolygonF *PL,QPointF P,bool isAdd=true);
    static QPointF reflectPx(QPointF P, qreal x);
    static void reflectPLx(QPolygonF *PL,qreal x);
    static void reflectRectx(QRectF *R, qreal x);
    static void reflectRecty(QRectF *R, qreal y);
    static QLineF reflectLx(QLineF L, qreal x);
    static QPointF reflectPy(QPointF P, qreal y);
    static void reflectPLy(QPolygonF *PL,qreal y);
    static QLineF reflectLy(QLineF L, qreal y);
    static bool isLineIntersectPolygon(QPointF p1,QPointF p2,QPolygonF PL);
    static bool isLineIntersectRect(QPointF p1,QPointF p2,QRectF R);
    static bool isPolylineIntersectPolygon(QPolygonF &PL1, QPolygonF &PL2);
    static bool isPolylineIntersectRect(QPolygonF &PL1, QRectF R);
    static qreal lenPolyline(QPolygonF &PL1);
    static QPolygonF cutPolyline(QPolygonF &PL1, qreal rFrom,qreal rTo);
    static QPolygonF makePerOffsetPolyline(QPolygonF &PL,qreal s);
    static QPolygonF makeFullPolygonFromPolyline(QPolygonF &PL,qreal s);
    static QPolygonF makeFullArrowPolygonFromPolyline(QPolygonF &PL,qreal s,qreal arr_len=0,qreal arr_width=0,TArrowType at0=arrow_none,TArrowType at1=arrow_normal);
    static QPointF pointOnPolygon(QPolygonF &PL,qreal len);

    static qreal setFontSizeInRect(QFont &font,QRectF R,QString text,qreal maxsz=5,qreal dsz=0.5);
    static QPointF pointFirstX(QPolygonF &PL, MVP_Enums::TDirect d);
    static QPointF pointLastX(QPolygonF &PL,MVP_Enums::TDirect d);
    static void sortByX(QPolygonF &PL,MVP_Enums::TDirect d);


};

#endif // QPFUNCTION_H
