#ifndef V_LAMP4X_H
#define V_LAMP4X_H

#include "v_lamp1x.h"

class v_Lamp4x : public v_Lamp1x
{
    Q_OBJECT

    MYPROP(SignalDescription, SIGNAL_2)
    MYPROP(SignalDescription, SIGNAL_3)
    MYPROP(SignalDescription, SIGNAL_4)

    Q_PROPERTY(QString SCOLOR_P2 READ getSCOLOR_P2 WRITE setSCOLOR_P2 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_P3 READ getSCOLOR_P3 WRITE setSCOLOR_P3 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_P4 READ getSCOLOR_P4 WRITE setSCOLOR_P4 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_P5 READ getSCOLOR_P5 WRITE setSCOLOR_P5 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_P6 READ getSCOLOR_P6 WRITE setSCOLOR_P6 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_P7 READ getSCOLOR_P7 WRITE setSCOLOR_P7 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_P8 READ getSCOLOR_P8 WRITE setSCOLOR_P8 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_P9 READ getSCOLOR_P9 WRITE setSCOLOR_P9 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_P10 READ getSCOLOR_P10 WRITE setSCOLOR_P10 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_P11 READ getSCOLOR_P11 WRITE setSCOLOR_P11 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_P12 READ getSCOLOR_P12 WRITE setSCOLOR_P12 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_P13 READ getSCOLOR_P13 WRITE setSCOLOR_P13 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_P14 READ getSCOLOR_P14 WRITE setSCOLOR_P14 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_P15 READ getSCOLOR_P15 WRITE setSCOLOR_P15 DESIGNABLE true )


    Q_PROPERTY(QString SCOLOR_B2 READ getSCOLOR_B2 WRITE setSCOLOR_B2 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_B3 READ getSCOLOR_B3 WRITE setSCOLOR_B3 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_B4 READ getSCOLOR_B4 WRITE setSCOLOR_B4 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_B5 READ getSCOLOR_B5 WRITE setSCOLOR_B5 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_B6 READ getSCOLOR_B6 WRITE setSCOLOR_B6 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_B7 READ getSCOLOR_B7 WRITE setSCOLOR_B7 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_B8 READ getSCOLOR_B8 WRITE setSCOLOR_B8 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_B9 READ getSCOLOR_B9 WRITE setSCOLOR_B9 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_B10 READ getSCOLOR_B10 WRITE setSCOLOR_B10 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_B11 READ getSCOLOR_B11 WRITE setSCOLOR_B11 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_B12 READ getSCOLOR_B12 WRITE setSCOLOR_B12 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_B13 READ getSCOLOR_B13 WRITE setSCOLOR_B13 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_B14 READ getSCOLOR_B14 WRITE setSCOLOR_B14 DESIGNABLE true )
    Q_PROPERTY(QString SCOLOR_B15 READ getSCOLOR_B15 WRITE setSCOLOR_B15 DESIGNABLE true )

public:
    QString getSCOLOR_P2() const {return FSCOLOR_P[2];}
    QString getSCOLOR_P3() const {return FSCOLOR_P[3];}
    QString getSCOLOR_P4() const {return FSCOLOR_P[4];}
    QString getSCOLOR_P5() const {return FSCOLOR_P[5];}
    QString getSCOLOR_P6() const {return FSCOLOR_P[6];}
    QString getSCOLOR_P7() const {return FSCOLOR_P[7];}
    QString getSCOLOR_P8() const {return FSCOLOR_P[8];}
    QString getSCOLOR_P9() const {return FSCOLOR_P[9];}
    QString getSCOLOR_P10() const {return FSCOLOR_P[10];}
    QString getSCOLOR_P11() const {return FSCOLOR_P[11];}
    QString getSCOLOR_P12() const {return FSCOLOR_P[12];}
    QString getSCOLOR_P13() const {return FSCOLOR_P[13];}
    QString getSCOLOR_P14() const {return FSCOLOR_P[14];}
    QString getSCOLOR_P15() const {return FSCOLOR_P[15];}

    QString getSCOLOR_B2() const {return FSCOLOR_B[2];}
    QString getSCOLOR_B3() const {return FSCOLOR_B[3];}
    QString getSCOLOR_B4() const {return FSCOLOR_B[4];}
    QString getSCOLOR_B5() const {return FSCOLOR_B[5];}
    QString getSCOLOR_B6() const {return FSCOLOR_B[6];}
    QString getSCOLOR_B7() const {return FSCOLOR_B[7];}
    QString getSCOLOR_B8() const {return FSCOLOR_B[8];}
    QString getSCOLOR_B9() const {return FSCOLOR_B[9];}
    QString getSCOLOR_B10() const {return FSCOLOR_B[10];}
    QString getSCOLOR_B11() const {return FSCOLOR_B[11];}
    QString getSCOLOR_B12() const {return FSCOLOR_B[12];}
    QString getSCOLOR_B13() const {return FSCOLOR_B[13];}
    QString getSCOLOR_B14() const {return FSCOLOR_B[14];}
    QString getSCOLOR_B15() const {return FSCOLOR_B[15];}

    void setSCOLOR_P2(QString &p){setSCOLOR_P(2,p);}
    void setSCOLOR_P3(QString &p){setSCOLOR_P(3,p);}
    void setSCOLOR_P4(QString &p){setSCOLOR_P(4,p);}
    void setSCOLOR_P5(QString &p){setSCOLOR_P(5,p);}
    void setSCOLOR_P6(QString &p){setSCOLOR_P(6,p);}
    void setSCOLOR_P7(QString &p){setSCOLOR_P(7,p);}
    void setSCOLOR_P8(QString &p){setSCOLOR_P(8,p);}
    void setSCOLOR_P9(QString &p){setSCOLOR_P(9,p);}
    void setSCOLOR_P10(QString &p){setSCOLOR_P(10,p);}
    void setSCOLOR_P11(QString &p){setSCOLOR_P(11,p);}
    void setSCOLOR_P12(QString &p){setSCOLOR_P(12,p);}
    void setSCOLOR_P13(QString &p){setSCOLOR_P(13,p);}
    void setSCOLOR_P14(QString &p){setSCOLOR_P(14,p);}
    void setSCOLOR_P15(QString &p){setSCOLOR_P(15,p);}

    void setSCOLOR_B2(QString &p){setSCOLOR_B(2,p);}
    void setSCOLOR_B3(QString &p){setSCOLOR_B(3,p);}
    void setSCOLOR_B4(QString &p){setSCOLOR_B(4,p);}
    void setSCOLOR_B5(QString &p){setSCOLOR_B(5,p);}
    void setSCOLOR_B6(QString &p){setSCOLOR_B(6,p);}
    void setSCOLOR_B7(QString &p){setSCOLOR_B(7,p);}
    void setSCOLOR_B8(QString &p){setSCOLOR_B(8,p);}
    void setSCOLOR_B9(QString &p){setSCOLOR_B(9,p);}
    void setSCOLOR_B10(QString &p){setSCOLOR_B(10,p);}
    void setSCOLOR_B11(QString &p){setSCOLOR_B(11,p);}
    void setSCOLOR_B12(QString &p){setSCOLOR_B(12,p);}
    void setSCOLOR_B13(QString &p){setSCOLOR_B(13,p);}
    void setSCOLOR_B14(QString &p){setSCOLOR_B(14,p);}
    void setSCOLOR_B15(QString &p){setSCOLOR_B(15,p);}


public:
    Q_INVOKABLE v_Lamp4x(v_Base *parent = nullptr);
    virtual ~v_Lamp4x(){}
    virtual void updateStates();
};

#endif // V_LAMP4X_H
