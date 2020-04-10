#ifndef m_RC_H
#define m_RC_H

#include <QDateTime>

#include "m_base.h"
#include "signaldescription.h"
#include "objectlink.h"

// базовый класс для всех моделей РЦ
// 4 полюсник определяющий состояний занятия и положения
// отдельного базового касса стрелки нет - все должно наследоваться от РЦ в конкретных моделях
//
//  00
class m_Svet;
class tos_RcTracking;
class m_RC : public m_Base
{
    Q_OBJECT
    Q_PROPERTY(ObjectLink NEXTDM00 READ getnext00 WRITE setnext00 DESIGNABLE true )
    Q_PROPERTY(ObjectLink NEXTDM10 READ getnext10 WRITE setnext10 DESIGNABLE true )
    Q_PROPERTY(ObjectLink NEXTDM01 READ getnext01 WRITE setnext01 DESIGNABLE true )
    Q_PROPERTY(ObjectLink NEXTDM11 READ getnext11 WRITE setnext11 DESIGNABLE true )
    Q_PROPERTY(ObjectLink SV_M00 READ getsv_m00 WRITE setsv_m00 DESIGNABLE true )
    Q_PROPERTY(ObjectLink SV_M10 READ getsv_m10 WRITE setsv_m10 DESIGNABLE true )
    Q_PROPERTY(ObjectLink SV_M01 READ getsv_m01 WRITE setsv_m01 DESIGNABLE true )
    Q_PROPERTY(ObjectLink SV_M11 READ getsv_m11 WRITE setsv_m11 DESIGNABLE true )

    Q_PROPERTY(MVP_Enums::TStrelPol STATE_POL READ STATE_POL WRITE setSTATE_POL DESIGNABLE true STORED false)


public:

    MYPROP(qreal,LEN)
    MYPROP(SignalDescription,SIGNAL_BUSY)
    MYPROP(SignalDescription,SIGNAL_OTC1)
    MYPROP(SignalDescription,SIGNAL_OTC2)

    MYPROP(SignalDescription,SIGNAL_ERR_LS)
    MYPROP(SignalDescription,SIGNAL_ERR_LZ)
    MYPROP(SignalDescription,SIGNAL_ERR_KZ)


    MYSTATE(MVP_Enums::TRCBusy,STATE_BUSY)
    MYSTATE(bool, STATE_BLOCK)

    MYSTATE(bool, STATE_ERR_LS)
    MYSTATE(bool, STATE_ERR_LZ)
    MYSTATE(bool, STATE_ERR_KZ)


    virtual void setSTATE_POL(MVP_Enums::TStrelPol p) {Q_UNUSED(p);}
    virtual MVP_Enums::TStrelPol STATE_POL() const {return MVP_Enums::TStrelPol::pol_w;}

    void setnext00(ObjectLink p){SETPROP(Fnext[0][0]);}
    void setnext10(ObjectLink p){SETPROP(Fnext[1][0]);}
    void setnext01(ObjectLink p){SETPROP(Fnext[0][1]);}
    void setnext11(ObjectLink p){SETPROP(Fnext[1][1]);}
    ObjectLink getnext00()const {return Fnext[0][0];}
    ObjectLink getnext10()const {return Fnext[1][0];}
    ObjectLink getnext01()const {return Fnext[0][1];}
    ObjectLink getnext11()const {return Fnext[1][1];}

    void setLink(ObjectLink &lnk,const ObjectLink &link_new);
    void setsv_m(ObjectLink p,int d,int m);
    void setsv_m00(ObjectLink p){setsv_m(p,0,0);}
    void setsv_m10(ObjectLink p){setsv_m(p,1,0);}
    void setsv_m01(ObjectLink p){setsv_m(p,0,1);}
    void setsv_m11(ObjectLink p){setsv_m(p,1,1);}
    ObjectLink getsv_m00()const {return Fsv_m[0][0];}
    ObjectLink getsv_m10()const {return Fsv_m[1][0];}
    ObjectLink getsv_m01()const {return Fsv_m[0][1];}
    ObjectLink getsv_m11()const {return Fsv_m[1][1];}


    virtual int DIRECTM()const;
    virtual int getNextCount(int d) const {Q_UNUSED(d);return 1;} // минус в зависимости от DIRECTM


public:
    explicit m_RC(QObject *parent = nullptr);
    virtual ~m_RC(){}
    virtual QString defaultGroupName() const {return "РЦ";}

    virtual void resetStates();

    virtual void validation(ListObjStr *l) const;

    virtual void updateAfterLoad();

    void linkNext(int d,int m,BaseObject *B);
    m_RC * getNextRC(int d,int m) const;
    m_RC * getNextRCcfb(int d,int m) const; // следующий с проверкой обратной связи
    m_RC * getNextRCpolcfb(int d) const; // следующий по положению,с проверкой обратной связи
    m_Svet * getSV_M(int d,int m) const;
    static QList<m_RC *> goGrafpolcfb(m_RC *rcStart,int d,int maxRC=100); // список РЦ

    m_RC * next_rc[2];
    QDateTime dtBusy;
    QDateTime dtFree;

    void addDevice(m_Base *m);
    QList<m_Base *> devices() {return l_devices;}

    virtual bool is33();
    virtual void updateStates();

    tos_RcTracking * rcs=nullptr; // ссылка на доп состояния используемые в work

protected:
    ObjectLink Fnext[2][2];
    ObjectLink Fsv_m[2][2];
    m_Svet *_sv_m[2][2];
    QList<m_Base *> l_devices;


};


#endif // m_RC_H
