#ifndef MCOLOR_H
#define MCOLOR_H
/*
 * описание цвета в системе MVP
 * */
#include "baseobject.h"
#include <QColor>
#include <QDateTime>
class QTimer;

class MColor : public BaseObject
{
    Q_OBJECT
    Q_PROPERTY(QColor     COLOR1 READ getCOLOR1 WRITE setCOLOR1 DESIGNABLE true)
    Q_PROPERTY(int        ALPHA1 READ getALPHA1 WRITE setALPHA1 DESIGNABLE true)
    Q_PROPERTY(int        DT1 READ getDT1 WRITE setDT1 DESIGNABLE true)
    Q_PROPERTY(bool       SMOOTH1 READ getSMOOTH1 WRITE setSMOOTH1 DESIGNABLE true)
    Q_PROPERTY(QColor     COLOR2 READ getCOLOR2 WRITE setCOLOR2 DESIGNABLE true)
    Q_PROPERTY(int        ALPHA2 READ getALPHA2 WRITE setALPHA2 DESIGNABLE true)
    Q_PROPERTY(int        DT2 READ getDT2 WRITE setDT2 DESIGNABLE true)
    Q_PROPERTY(bool       SMOOOH2 READ getSMOOTH2 WRITE setSMOOTH2 DESIGNABLE true)
    Q_PROPERTY(QColor     COLOR3 READ getCOLOR3 WRITE setCOLOR3 DESIGNABLE true)
    Q_PROPERTY(int        ALPHA3 READ getALPHA3 WRITE setALPHA3 DESIGNABLE true)
    Q_PROPERTY(int        DT3 READ getDT3 WRITE setDT3 DESIGNABLE true)
    Q_PROPERTY(bool       SMOOTH3 READ getSMOOTH3 WRITE setSMOOTH3 DESIGNABLE true)
    Q_PROPERTY(QColor     COLOR4 READ getCOLOR4 WRITE setCOLOR4 DESIGNABLE true)
    Q_PROPERTY(int        ALPHA4 READ getALPHA4 WRITE setALPHA4 DESIGNABLE true)
    Q_PROPERTY(int        DT4 READ getDT4 WRITE setDT4 DESIGNABLE true)
    Q_PROPERTY(bool       SMOOTH4 READ getSMOOTH4 WRITE setSMOOTH4 DESIGNABLE true)

public:

    QColor getCOLOR1()const {return COLOR[0];}
    QColor getCOLOR2()const {return COLOR[1];}
    QColor getCOLOR3()const {return COLOR[2];}
    QColor getCOLOR4()const {return COLOR[3];}
    void setCOLOR1(QColor p){setCOLOR(0,p);}
    void setCOLOR2(QColor p){setCOLOR(1,p);}
    void setCOLOR3(QColor p){setCOLOR(2,p);}
    void setCOLOR4(QColor p){setCOLOR(3,p);}
    int getDT1()const{return DT[0];}
    int getDT2()const{return DT[1];}
    int getDT3()const{return DT[2];}
    int getDT4()const{return DT[3];}
    void setDT1(int p){DT[0]=p;}
    void setDT2(int p){DT[1]=p;}
    void setDT3(int p){DT[2]=p;}
    void setDT4(int p){DT[3]=p;}
    void setALPHA1(int p){setALPHA(0,p);}
    void setALPHA2(int p){setALPHA(1,p);}
    void setALPHA3(int p){setALPHA(2,p);}
    void setALPHA4(int p){setALPHA(3,p);}
    int getALPHA1()const{return ALPHA[0];}
    int getALPHA2()const{return ALPHA[1];}
    int getALPHA3()const{return ALPHA[2];}
    int getALPHA4()const{return ALPHA[3];}
    bool getSMOOTH1()const{return SMOOTH[0];}
    bool getSMOOTH2()const{return SMOOTH[1];}
    bool getSMOOTH3()const{return SMOOTH[2];}
    bool getSMOOTH4()const{return SMOOTH[3];}
    void setSMOOTH1(int p){setSMOOTH(0,p);}
    void setSMOOTH2(int p){setSMOOTH(1,p);}
    void setSMOOTH3(int p){setSMOOTH(2,p);}
    void setSMOOTH4(int p){setSMOOTH(3,p);}
    void setCOLOR(int i,QColor p);
    void setALPHA(int i,int p);
    void setSMOOTH(int i,int p);

public:

    Q_INVOKABLE MColor(QObject *parent = 0);
    explicit MColor(const MColor &mclr,QObject *parent = 0);
    explicit MColor(int uid,QString name,QColor clr,QObject *parent = 0);
    void assign(const MColor *mclr);
    virtual ~MColor();


    virtual void updateAfterLoad();

    int UID() const {return fuid;}
    void setUID(int p){fuid=p;}

    QColor getCOLOR();
    bool isAnimated();
    void update();

signals:
    void changed();

public slots:
    virtual void updateStates(){}
    void updateTimer();
protected:
    QColor currentColor;
    QColor COLOR[4];
    int DT[4];
    int ALPHA[4];
    bool SMOOTH[4];
    int fuid;
    int currentIndex;
    QDateTime startT;
};

#endif // MCOLOR_H
