#ifndef M_RC_GOR_H
#define M_RC_GOR_H

#include "m_rc.h"

// Горочная РЦ для использования в моделях старых грок


class m_RC_Gor : public m_RC
{
    Q_OBJECT

public:
    MYPROP(int, MINWAY)
    MYPROP(int, MAXWAY)
    MYPROP(int, MINWAY1)
    MYPROP(int, MAXWAY1)
    MYPROP(SignalDescription,SIGNAL_WAY1)
    MYSTATE(bool, STATE_WAY1)

    

public:
    Q_INVOKABLE m_RC_Gor(QObject *parent = nullptr);
    virtual ~m_RC_Gor(){}
    virtual void validation(ListObjStr *l) const;
    virtual void resetStates();


    virtual MVP_Enums::TStrelPol STATE_POL() const {return MVP_Enums::TStrelPol::pol_plus;}
    virtual void updateStates();

    QMap<int,QMap<int,int>> m_PN_M2N; // номер по порядку от зкр, Путь надвига,маршрут-> N
    QMap<int,QMap<int,int>> m_PN_M2X; // смещение по порядку от зкр, Путь надвига,маршрут-> Х

protected:
    
};

#endif // M_RC_GOR_H
