#ifndef V_LPDIAG_H
#define V_LPDIAG_H

#include "v_base.h"
class QTimer;

class LIBSHARED_EXPORT v_LPDiag : public v_Base
{
    Q_OBJECT
public:
    Q_PROPERTY(int ADDR READ ADDR DESIGNABLE true STORED false )
    Q_PROPERTY(QString CHANELNAME READ CHANELNAME WRITE setCHANELNAME DESIGNABLE true )
    Q_PROPERTY(int NORDER READ NORDER WRITE setNORDER DESIGNABLE true )


    MYPROP(int,ADDR_KRUG)
    MYPROP(int,ADDR00)
    MYPROP(int,ADDR01)
    MYPROP(int,ADDR10)
    MYPROP(int,ADDR11)

    MYSTATE(int,STATE_ADDR_IN00)
    MYSTATE(int,STATE_ADDR_IN01)
    MYSTATE(int,STATE_ADDR_IN10)
    MYSTATE(int,STATE_ADDR_IN11)

    MYPROP(QSizeF,LPSIZE)
    MYPROP(qreal,TEXTNAMEHEIGHT)
    MYPROP(qreal,TEXTADDRHEIGHT)
    MYPROP(bool ,ANIMATE)

    QString CHANELNAME(){return FCHANELNAME;}
    void setCHANELNAME(QString p);
    int NORDER(){return FNORDER;}
    virtual void setNORDER(int p);
    int ADDR()const {return FADDR_KRUG*0x100+FNORDER*0x10;}
public:
        enum Tinout{in=0,out=1};
    Q_INVOKABLE v_LPDiag(v_Base *parent = nullptr);
    virtual ~v_LPDiag();

    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

    virtual void calculateGeometry(); // перестраивает внтренние координаты

    virtual int isAddrExists(int addr);
    virtual QStringList supportedModels()const {return QStringList();}

    virtual QList<QPolygonF> getInOutPolygons(int n, Tinout inout, int ud1, int nord);
    void clearLines();
    void makeLines();
    QPolygonF lines[4];
    QPolygonF linesG[4];
    QPolygonF arrows[4];
    virtual void advance(int phase);

    virtual void updateStates();
protected:
    QString FCHANELNAME;
    int FNORDER;
    GtBuffer *chanels13[2];
    GtBuffer *chanels14[2];
    GtBuffer *chanels15[2];

    v_LPDiag *getLPDiag(int addkrug,int addr, int &n) const;

    qreal w_lp();
    qreal h_lp();
    //QTimer *timerAnimateNewPacket;
    //QTimer *timerAnimateNewAddr;
    QTime timeAnimateNewPacket;
    QTime timeAnimateNewAddr;

    qreal proclen[4];
    int FPACKET_RECIVED15;

    int addr[4];
    int addr_in[4];
    int last_addr_in[4];
    int last_addr[4];

    bool addr_not_found[4];
    int  addr_is_new[4];
    int M13_S[2];
    int M15_S[2];


    QRectF  boundrect_text;
    QRectF  lpR;

    QPolygonF makeLine(int n1, v_LPDiag *LP2, int n2);
    QPolygonF polygon2root(QPolygonF &PL);
    QPolygonF polygonFromRoot(QPolygonF &PL);
    QPolygonF polygonO2O(v_LPDiag* LP1,QPolygonF &PL,v_LPDiag* LP2);
    qreal getFontPointSize4rect(QPainter *painter, QString &text, QRectF R, qreal maxSize=0);
    qreal  YY(qreal yc, int ud, int np, int inout);
    QString getNameLink(int n);

    bool isIntersect(int addr_krug, QPolygonF &scr_p1);
    int sovpad(int addr_krug, QPolygonF &PL1);


};

#endif // V_LPDIAG_H
