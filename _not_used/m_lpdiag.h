#ifndef M_LPDIAG_H
#define M_LPDIAG_H

#include "m_base.h"
class m_LPDiag : public m_Base
{
    Q_OBJECT
    Q_PROPERTY(int ADDR READ ADDR DESIGNABLE true )
    Q_PROPERTY(QString CHANELNAME READ CHANELNAME WRITE setCHANELNAME DESIGNABLE true )
    Q_PROPERTY(int NORDER READ NORDER WRITE setNORDER DESIGNABLE true )

public:
    MYPROP(int,ADDR_KRUG)
    MYPROP(int,ADDR00)
    MYPROP(int,ADDR01)
    MYPROP(int,ADDR10)
    MYPROP(int,ADDR11)

    MYSTATE(int,STATE_ADDR_IN00)
    MYSTATE(int,STATE_ADDR_IN01)
    MYSTATE(int,STATE_ADDR_IN10)
    MYSTATE(int,STATE_ADDR_IN11)

    MYSTATE(int,PACKET_RECIVED13)
    MYSTATE(int,PACKET_RECIVED14)
    MYSTATE(int,PACKET_RECIVED15)

    QString CHANELNAME(){return FCHANELNAME;}
    void setCHANELNAME(QString p);
    int NORDER(){return FNORDER;}
    void setNORDER(int p);
    int ADDR();
public:

    Q_INVOKABLE m_LPDiag(QObject *parent = 0);
    virtual ~m_LPDiag(){}

    virtual ListObjStr validation();
    m_LPDiag *find_lp_by_addrlink(int addr);

public slots:
    virtual void updateStates();
    virtual void work(){}
protected:
    QString FCHANELNAME;
    int FNORDER;
    quint64 tick_150,tick_158,tick_130,tick_138,tick_140,tick_148;

};

#endif // M_LPDIAG_H
