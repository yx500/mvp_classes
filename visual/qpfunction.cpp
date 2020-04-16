#include "qpfunction.h"

#include <qmath.h>
#include <QFontMetrics>

QPFunction::QPFunction()
{
}


void QPFunction::pointsOnPerpendikular(QPointF T1,QPointF T2,qreal len1,qreal len2,QPointF &TP1,QPointF &TP2)
{
    if (T1.y()==T2.y())  {
        TP1=QPointF(T1.x(),T1.y()-len1);
        TP2=QPointF(T1.x(),T1.y()+len2);
        return;
    } else
    if (T1.x()==T2.x())  {
        TP1=QPointF(T1.x()-len1,T1.y());
        TP2=QPointF(T1.x()+len2,T1.y());
    }else{
        qreal k=(T2.y()-T1.y())/(T2.x()-T1.x()); //расчет углового коэффициента
        //qreal b=(T2.x()*T1.y()-T1.x()*T2.y())/(T2.x()-T1.x()); //И расчет смещения первой прямой
        //qreal d=T1.y()+1./k*T1.x(); //Из теорем о перпендикулярных прямых
        k=-1./k;
        qreal x1=qSqrt((len1*len1)/(1+k*k));
        qreal y1=k*x1;
        qreal x2=qSqrt((len2*len2)/(1+k*k));
        qreal y2=k*x2;
        TP1=QPointF(T1.x()+x1,T1.y()+y1);
        TP2=QPointF(T1.x()-x2,T1.y()-y2);
    }

}
QPointF QPFunction::pointOnPerpendikular(QPointF T1,QPointF T2,qreal len1)
{
    if (T1.y()==T2.y())  {
        if (T1.x()<T2.x())
            return QPointF(T1.x(),T1.y()+len1); else
            return QPointF(T1.x(),T1.y()-len1);
    } else
    if (T1.x()==T2.x())  {
        if (T1.y()<T2.y())
            return QPointF(T1.x()-len1,T1.y()); else
            return QPointF(T1.x()+len1,T1.y());
    }else{
        qreal k=(T2.y()-T1.y())/(T2.x()-T1.x()); //расчет углового коэффициента
        k=-1./k;
        qreal x1=qSqrt((len1*len1)/(1+k*k));
        if (T1.y()>T2.y()) x1=-1*x1;
        if (len1>0) x1=-1*x1;
        qreal y1=k*x1;
        return QPointF(T1.x()+x1,T1.y()+y1);
    }

}

QPointF QPFunction::pointOnLine(QPointF T1,QPointF T2,qreal len1)
{
    qreal kx=1;qreal ky=1;
      //if (T1.y()>T2.y()) {ky=-1;kx=-1;}
    if (T1.x()>T2.x()) {ky=-1;kx=-1;}
    if (len1<0){kx=-1*kx;ky=-1*ky;}
    if (T1.y()==T2.y())  {
        if (T1.x()<T2.x())
            return QPointF(T1.x()+len1,T1.y()); else
            return QPointF(T1.x()-len1,T1.y());
    } else
    if (T1.x()==T2.x())  {
        if (T1.y()<T2.y())
            return QPointF(T1.x(),T1.y()+len1); else
            return QPointF(T1.x(),T1.y()-len1);
    }else{
        qreal k=(T2.y()-T1.y())/(T2.x()-T1.x()); //расчет углового коэффициента
        qreal x1=qSqrt((len1*len1)/(1+k*k));
        qreal y1=k*x1;
        return QPointF(T1.x()+x1*kx,T1.y()+y1*ky);
    }

}


bool QPFunction::isPointOnLine(QPointF T1,QPointF T2,QPointF TX)
{

    if (T1.y()==T2.y())  {
        if (T1.x()<T2.x()) {
            if ((T1.x()<=TX.x()) &&(T2.x()>=TX.x())) return true; else return false;
        } else {
            if ((T2.x()<=TX.x()) &&(T1.x()>=TX.x())) return true; else return false;
        }
    } else
    if (T1.x()==T2.x())  {
        if (T1.y()<T2.y()){
            if ((T1.y()<=TX.y()) &&(T2.y()>=TX.y())) return true; else return false;
        } else {
            if ((T2.y()<=TX.y()) &&(T1.y()>=TX.y())) return true; else return false;
        }
    }else{
        qreal kx=1;qreal ky=1;
        qreal len1=TX.x()-T1.x();
        if (T1.x()>T2.x()) {ky=-1;kx=-1;}
        if (len1<0){kx=-1*kx;ky=-1*ky;}

        qreal k=(T2.y()-T1.y())/(T2.x()-T1.x()); //расчет углового коэффициента
        qreal x1=qSqrt((len1*len1)/(1+k*k));
        qreal y1=k*x1;
        QPointF P=QPointF(T1.x()+x1*kx,T1.y()+y1*ky);
        if (P==TX) return true;
    }
    return false;
}


bool QPFunction::intersection(QPointF p1, QPointF p2, QPointF p3, QPointF p4,QPointF &ret) {
// Store the values for fast access and easy
// equations-to-code conversion
qreal x1 = p1.x(), x2 = p2.x(), x3 = p3.x(), x4 = p4.x();
qreal y1 = p1.y(), y2 = p2.y(), y3 = p3.y(), y4 = p4.y();

qreal d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
// If d is zero, there is no intersection
if (d == 0) return false;

// Get the x and y
qreal pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
qreal x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
qreal y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;

/*
// Check if the x and y coordinates are within both lines
if ( x < min(x1, x2) || x > max(x1, x2) ||
x < min(x3, x4) || x > max(x3, x4) ) return nullptr;
if ( y < min(y1, y2) || y > max(y1, y2) ||
y < min(y3, y4) || y > max(y3, y4) ) return nullptr;

*/

// Return the point of intersection
ret.setX(x);
ret.setY(y);
return true;
}

bool QPFunction::intersectionLines(QPointF p1, QPointF p2, QPointF p3, QPointF p4,QPointF &ret) {
// Store the values for fast access and easy
// equations-to-code conversion


qreal x1 = p1.x(), x2 = p2.x(), x3 = p3.x(), x4 = p4.x();
qreal y1 = p1.y(), y2 = p2.y(), y3 = p3.y(), y4 = p4.y();

if ( qMax(x1, x2) < qMin(x3, x4))  return false;
if ( qMin(x1, x2) > qMax(x3, x4))  return false;
if ( qMax(y1, y2) < qMin(y3, y4))  return false;
if ( qMin(y1, y2) > qMax(y3, y4))  return false;

qreal d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
// If d is zero, there is no intersection
if (d == 0) return false;

// Get the x and y
qreal pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
qreal x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
qreal y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;


// Check if the x and y coordinates are within both lines
if ( x < qMin(x1, x2) || x > qMax(x1, x2) ||
x < qMin(x3, x4) || x > qMax(x3, x4) ) return false;
if ( y < qMin(y1, y2) || y > qMax(y1, y2) ||
y < qMin(y3, y4) || y > qMax(y3, y4) ) return false;

// Return the point of intersection
ret.setX(x);
ret.setY(y);
return true;
}

const qreal pochti0=1./(10000.*10000.);
QPointF QPFunction::intersectionPoint(QPointF p1, QPointF p2, QPointF p3, QPointF p4)
{
    QPointF ret;
    // Store the values for fast access and easy
    // equations-to-code conversion

    qreal x1 = p1.x(), x2 = p2.x(), x3 = p3.x(), x4 = p4.x();
    qreal y1 = p1.y(), y2 = p2.y(), y3 = p3.y(), y4 = p4.y();

    if ((y1==y2)&&(y3==y4)&&(y1==y3)){
        // на одной прямой
        qreal maxx1=qMax(x1,x2);
        qreal minx2=qMin(x3,x4);
        ret.setX((maxx1+minx2)/2.);
        ret.setY(y1);
        return ret;
    }


    qreal d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    // If d is zero, there is no intersection
    // для паралельных берем просто среднюю точку

    if (qAbs(d)<pochti0){
        QPointF P1=p1;
        if (p2.x()>p1.x()) P1=p2;
        QPointF P2=p3;
        if (p3.x()>p4.x()) P2=p4;
        ret.setX((P1.x()+P2.x())/2.);
        ret.setY((P1.y()+P2.y())/2.);
        return ret;
    }
    //if (d == 0) return ret;

    // Get the x and y
    qreal pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
    qreal x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
    qreal y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;


    // Return the point of intersection
    ret.setX(x);
    ret.setY(y);
    return ret;
}
QPointF QPFunction::intersectionPointHard(QPointF p1, QPointF p2, QPointF p3, QPointF p4) {
QPointF ret=QPointF(0,0);
// Store the values for fast access and easy
// equations-to-code conversion

qreal x1 = p1.x(), x2 = p2.x(), x3 = p3.x(), x4 = p4.x();
qreal y1 = p1.y(), y2 = p2.y(), y3 = p3.y(), y4 = p4.y();


qreal d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
// If d is zero, there is no intersection
if (d == 0) return ret;
// Get the x and y
qreal pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
qreal x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
qreal y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;
// Return the point of intersection
ret.setX(x);
ret.setY(y);
return ret;
}

QPolygonF  QPFunction::makerect(QPointF T1,QPointF T2,qreal S)
{
    QPolygonF T;
    T.push_back(pointOnPerpendikular(T1,T2,-S));
    T.push_back(pointOnPerpendikular(T2,T1,+S));
    T.push_back(pointOnPerpendikular(T2,T1,-S));
    T.push_back(pointOnPerpendikular(T1,T2,+S));
    return T;
}

bool QPFunction::isPointInSquea(QPointF T, QPointF C, qreal R)
{
     if ((fabs(T.x()-C.x())<R)&&
         (fabs(T.y()-C.y())<R)) return true;
     return false;
}

QRectF QPFunction::R1inR2(QRectF R1,QRectF R2)
{

    // выдаем часть R1 которая находится в R2
    QRectF R;
    R=QRectF(0,0,0,0);
    // вообще пересекаемся?
    if ((R1.bottom()<R2.top())||
        (R1.top()>R2.bottom())||
        (R1.left()>R2.right())||
        (R1.right()<R2.left()))
          return R;
    qreal top=      qMax(R1.top(),R2.top());
    qreal bottom=   qMin(R1.bottom(),R2.bottom());
    qreal left=     qMax(R1.left(),R2.left());
    qreal right=   qMin(R1.right(),R2.right());
    R=QRectF(left,top,right-left,bottom-top);
    return R;
}

void QPFunction::P2PL(QPolygonF *PL, QPointF P, bool isAdd)
{
    if (!isAdd) return;
    PL->push_back(P);
}

QPointF QPFunction::reflectPx(QPointF P, qreal x)
{
    return QPointF(x-P.x(),P.y());
}

void QPFunction::reflectPLx(QPolygonF *PL, qreal x)
{
    QPolygonF _PL;
    for (int  i=0;i<PL->size();i++){
        _PL.push_back(reflectPx(PL->at(i),x));
    }
    PL->clear();
    for (int  i=0;i<_PL.size();i++){
        PL->push_back(_PL[i]);
    }

}

void QPFunction::reflectRectx(QRectF *R, qreal x)
{
    R->moveCenter(QPointF(x-R->center().x(),R->center().y()));
}
void QPFunction::reflectRecty(QRectF *R, qreal y)
{
    R->moveCenter(QPointF(R->center().x(),y-R->center().y()));
}

QLineF QPFunction::reflectLx(QLineF L, qreal x)
{
    return QLineF(QPointF(x-L.p1().x(),L.p1().y()),
                  QPointF(x-L.p2().x(),L.p2().y()));
}

QPointF QPFunction::reflectPy(QPointF P, qreal y)
{
    return QPointF(P.x(),y-P.y());
}

void QPFunction::reflectPLy(QPolygonF *PL, qreal y)
{
    QPolygonF _PL;
    for (int  i=0;i<PL->size();i++){
        _PL.push_back(reflectPy(PL->at(i),y));
    }
    PL->clear();
    for (int  i=0;i<_PL.size();i++){
        PL->push_back(_PL[i]);
    }

}

QLineF QPFunction::reflectLy(QLineF L, qreal y)
{
    return QLineF(QPointF(L.p1().x(),y-L.p1().y()),
                  QPointF(L.p2().x(),y-L.p2().y()));
}

bool QPFunction::isLineIntersectPolygon(QPointF p1, QPointF p2, QPolygonF PL)
{
    QPointF ret;
    for (int i=0;i<PL.size()-1;i++){
        if (intersectionLines(p1, p2, PL[i], PL[i+1],ret)) return true;
    }
    if (PL.size()>1)
        if (intersectionLines(p1, p2, PL[PL.size()-1], PL[0],ret)) return true;
    return false;
}

bool QPFunction::isLineIntersectRect(QPointF p1, QPointF p2, QRectF R)
{
    QPolygonF PL;
    PL.push_back(QPoint(R.left(),R.top()));
    PL.push_back(QPoint(R.right(),R.top()));
    PL.push_back(QPoint(R.right(),R.bottom()));
    PL.push_back(QPoint(R.left(),R.bottom()));
    return isLineIntersectPolygon(p1, p2, PL);
}

bool QPFunction::isPolylineIntersectPolygon(QPolygonF &PL1, QPolygonF &PL2)
{
    for (int i=0;i<PL1.size()-1;i++){
        if (isLineIntersectPolygon(PL1[i], PL1[i+1], PL2)) return true;
    }
    return false;
}

bool QPFunction::isPolylineIntersectRect(QPolygonF &PL1, QRectF R)
{
    QPolygonF PL2;
    PL2.push_back(QPoint(R.left(),R.top()));
    PL2.push_back(QPoint(R.right(),R.top()));
    PL2.push_back(QPoint(R.right(),R.bottom()));
    PL2.push_back(QPoint(R.left(),R.bottom()));
    return isPolylineIntersectPolygon(PL1,PL2);
}


qreal QPFunction::lenPolyline(QPolygonF &PL1)
{
    qreal len=0;
    for (int i=0;i<PL1.size()-1;i++){
        QLineF L=QLineF(PL1[i],PL1[i+1]);
        len+=L.length();
    }
    return len;
}
QPolygonF QPFunction::cutPolyline(QPolygonF &PL1, qreal rFrom, qreal rTo)
{
    QPolygonF resP;
    qreal len=lenPolyline(PL1);
    if ((rFrom==1) && (PL1.size()>0)) {
        resP<<PL1.last();
        return resP;
    }
    if ((rTo==0) && (PL1.size()>0)) {
        resP<<PL1.first();
        return resP;
    }
    if (rFrom>=1) return PL1;
    if ((rFrom<=0)&&(rTo>=1)) return PL1;
    qreal lenFrom=len*rFrom;
    qreal lenTo=len*rTo;

    QList<qreal> x;
    x.push_back(0);
    len=0;
    for (int i=0;i<PL1.size()-1;i++){
        QLineF L=QLineF(PL1[i],PL1[i+1]);
        len+=L.length();
        x.push_back(len);
    }

    for (int i=0;i<PL1.size()-1;i++){
        QPointF P=PL1[i];
        if (lenFrom==x[i]){
            resP << P;
        }
        if (lenTo==x[i]){
            resP << P;return resP;
        }
        if ((lenFrom>x[i])&&(lenFrom<x[i+1])) {
            P=pointOnLine(PL1[i],PL1[i+1],lenFrom-x[i]);
            resP << P;
        }

        if ((lenTo>x[i])&&(lenTo<x[i+1])){
            resP << P;
            P=pointOnLine(PL1[i],PL1[i+1],lenTo-x[i]);
            resP << P;
            return resP;
        }
        if (x[i]>=lenFrom) resP << P;


    }
    resP <<PL1.last();
    return resP;
}

QPolygonF QPFunction::makePerOffsetPolyline(QPolygonF &PL, qreal s)
{
    QPolygonF PL0;
    if (PL.size()<2) return PL0;
    QPointF P0;
    P0=pointOnPerpendikular(PL[0],PL[1],s);
    PL0.push_back(P0);
    for (int i=0;i<PL.size()-1;i++){
        if (i==PL.size()-2){
            P0=pointOnPerpendikular(PL[i+1],PL[i],-s);
            PL0.push_back(P0);
        } else {
            QPointF T1=PL[i];
            QPointF T2=PL[i+1];
            QPointF T3=PL[i+2];
            {
                QPointF _P1=pointOnPerpendikular(T1,T2,s);
                QPointF _P2=pointOnPerpendikular(T2,T1,-s);
                QPointF _P3=pointOnPerpendikular(T2,T3,s);
                QPointF _P4=pointOnPerpendikular(T3,T2,-s);
                P0=intersectionPoint(_P1, _P2, _P3,_P4);
                PL0.push_back(P0);
            }

        }
    }

    return PL0;
}



QPolygonF QPFunction::makeFullPolygonFromPolyline(QPolygonF &PL, qreal s)
{
    QPolygonF P;
    if (PL.size()<2) return P;
    QPolygonF PL0=makePerOffsetPolyline(PL,s);
    QPolygonF PL1=makePerOffsetPolyline(PL,-s);
    P << PL0;
    for (int i=PL1.size()-1;i>=0;i--)
        P << PL1[i];
    return P;
}

QPolygonF QPFunction::makeFullArrowPolygonFromPolyline(QPolygonF &PL, qreal s, qreal arr_len, qreal arr_width, QPFunction::TArrowType at0, QPFunction::TArrowType at1)
{
    QPolygonF P;

    if (PL.size()<2) return P;

    QPolygonF PL0=makePerOffsetPolyline(PL,s);
    QPolygonF PL1=makePerOffsetPolyline(PL,-s);

    if (arr_len==0) arr_len=3*s;
    if (arr_width==0) arr_width=2*s;

    // стрелка
    bool add0=false;
    if (at0==arrow_normal){
        add0=true;
        QPointF PS0=pointOnLine(PL[0],PL[1],arr_len);
        QPointF PS10=pointOnLine(PL0[0],PL0[1],arr_len);
        QPointF PS20=pointOnPerpendikular(PS0,PL[0],-arr_width);
        QPointF PS11=pointOnLine(PL1[0],PL1[1],arr_len);
        QPointF PS21=pointOnPerpendikular(PS0,PL[0],arr_width);
        PL0.remove(0);
        PL1.remove(0);
        PL0.insert(0,PS10);PL0.insert(0,PS20);
        PL1.insert(0,PS11);PL1.insert(0,PS21);
    }
    bool add1=false;
    if (at1==arrow_normal){
        add1=true;
        QPointF PS0=pointOnLine(PL[PL.size()-1],PL[PL.size()-2],arr_len);

        QPointF PS10=pointOnLine(PL0[PL.size()-1],PL0[PL.size()-2],arr_len);
        QPointF PS20=pointOnPerpendikular(PS0,PL.back(),arr_width);
        QPointF PS11=pointOnLine(PL1[PL.size()-1],PL1[PL.size()-2],arr_len);
        QPointF PS21=pointOnPerpendikular(PS0,PL.back(),-arr_width);
        PL0.remove(PL0.size()-1);
        PL1.remove(PL1.size()-1);
        PL0.insert(PL0.size(),PS10);PL0.insert(PL0.size(),PS20);
        PL1.insert(PL1.size(),PS11);PL1.insert(PL1.size(),PS21);
    }
    if (add0)
        P << PL[0];
    P << PL0;
    if (add1)
        P << PL.back();
    for (int i=PL1.size()-1;i>=0;i--)
        P << PL1[i];
    return P;
}

QPointF QPFunction::pointOnPolygon(QPolygonF &PL, qreal len)
{
    QPointF P;
    if (PL.empty()) return P;
    if (PL.size()==1) return PL[0];

    qreal lens=0;
    for (int i=0;i<PL.size()-1;i++){
        QLineF L=QLineF(PL[i],PL[i+1]);
        qreal len1=L.length();
        if (lens+len1>=len){
            qreal lenost=len-lens;
            return pointOnLine(PL[i],PL[i+1],lenost);
        }
        lens+=len1;
    }
    return PL.back();
}

qreal QPFunction::setFontSizeInRect(QFont &font, QRectF R, QString text, qreal maxsz, qreal dsz)
{
    qreal sz=maxsz;
    while (sz>1){
        font.setPointSizeF(sz);
        {
            QFontMetrics fm(font);
            QRectF boundrect_text=fm.boundingRect(text);
            if ((boundrect_text.width()<=R.width())&&(boundrect_text.height()<=R.height())) {
                return sz;
            }
        }
        sz-=dsz;
    }
    return sz;
}

QPointF QPFunction::pointFirstX(QPolygonF &PL,MVP_Enums::TDirect d)
{
    QPointF &P=PL[0];
    for (int i=1;i<PL.size();i++){
        if (d==MVP_Enums::left2right){
            if (PL[i].x()<P.x()) P=PL[i];
        } else {
            if (PL[i].x()>P.x()) P=PL[i];
        }
    }
    return P;
}
QPointF QPFunction::pointLastX(QPolygonF &PL,MVP_Enums::TDirect d)
{
    QPointF &P=PL[0];
    for (int i=1;i<PL.size();i++){
        if (d==MVP_Enums::left2right){
            if (PL[i].x()>P.x()) P=PL[i];
        } else {
            if (PL[i].x()<P.x()) P=PL[i];
        }
    }
    return P;
}

bool pointXLessThan(const QPointF &P1, const QPointF &P2)
{
    if (P1.x()<P2.x()) return true;
    return false;
}
bool pointXUnLessThan(const QPointF &P1, const QPointF &P2)
{
    if (P1.x()>P2.x()) return true;
    return false;
}


void QPFunction::sortByX(QPolygonF &PL, MVP_Enums::TDirect d)
{
    if (d==MVP_Enums::left2right)
        std::sort(PL.begin(),PL.end(),pointXLessThan); else
        std::sort(PL.begin(),PL.end(),pointXUnLessThan);
}

