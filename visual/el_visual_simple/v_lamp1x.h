#ifndef V_LAMP1X_H
#define V_LAMP1X_H

#include "v_stat_text.h"
#include "signaldescription.h"

class v_Lamp1x : public v_StaticText
{
    Q_OBJECT
    MYPROP(SignalDescription, SIGNAL_1)

    Q_PROPERTY(int STATE_SIGNAL READ getSTATE_SIGNAL WRITE setSTATE_SIGNAL DESIGNABLE true STORED false) \

    Q_PROPERTY(QString SCOLOR_P0 READ getSCOLOR_P0 WRITE setSCOLOR_P0 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_P1 READ getSCOLOR_P1 WRITE setSCOLOR_P1 DESIGNABLE true )

    Q_PROPERTY(QString SCOLOR_B0 READ getSCOLOR_B0 WRITE setSCOLOR_B0 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_B1 READ getSCOLOR_B1 WRITE setSCOLOR_B1 DESIGNABLE true )

    Q_PROPERTY(QString STEXT_0 READ getSTEXT_0 WRITE setSTEXT_0 DESIGNABLE true )
    Q_PROPERTY(QString STEXT_1 READ getSTEXT_1 WRITE setSTEXT_1 DESIGNABLE true )
public:

    QString getSCOLOR_P0() const {return FSCOLOR_P[0];}
    QString getSCOLOR_P1() const {return FSCOLOR_P[1];}

    QString getSCOLOR_B0() const {return FSCOLOR_B[0];}
    QString getSCOLOR_B1() const {return FSCOLOR_B[1];}

    QString getSTEXT_0() const {return FSTEXT[0];}
    QString getSTEXT_1() const {return FSTEXT[1];}

    void setSCOLOR_P(int i,QString &p);
    void setSCOLOR_B(int i,QString &p);
    void setTEXT_(int i,QString &p);

    void setSCOLOR_P0(QString &p){setSCOLOR_P(0,p);}
    void setSCOLOR_P1(QString &p){setSCOLOR_P(1,p);}

    void setSCOLOR_B0(QString &p){setSCOLOR_B(0,p);}
    void setSCOLOR_B1(QString &p){setSCOLOR_B(1,p);}

    void setSTEXT_0(QString &p){setTEXT_(0,p);}
    void setSTEXT_1(QString &p){setTEXT_(1,p);}

public:
    Q_INVOKABLE v_Lamp1x(v_Base *parent = nullptr);
    virtual ~v_Lamp1x(){}

    virtual int getSTATE_SIGNAL()const {return FSTATE_SIGNAL;}
    void setSTATE_SIGNAL(int p);

    virtual void resetStates();

    virtual QString getText();
    virtual QColor getTextColor();
    virtual QColor getRectColor();
    virtual QColor getBrusColor();

    virtual void updateStates();

protected:
    QString FSCOLOR_P[16];
    QString FSCOLOR_B[16];
    QString FSTEXT[16];
    int FSTATE_SIGNAL;

};

#endif // V_LAMP1X_H
