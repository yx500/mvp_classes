#ifndef M_DSO2_H
#define M_DSO2_H

#include "qobject.h"

class m_DSO;
// левый/правый от горба 0/1 (L/R для свойства)

class m_DSO2 : public QObject
{
    Q_OBJECT
    Q_ENUMS(TState_DSO2)
public:
    enum TState_DSO2{
        error=-1,   // ошибка,
        nothing=0,  // ничего
        os2park=1,  // ось в парк
        os2gorb=2   // ось на горб
    };
    explicit m_DSO2(m_DSO* DSO0,m_DSO* DSO1);
     virtual ~m_DSO2(){}

    void work();
    TState_DSO2  GetState(void);
//    void ClearCount(void);

protected:
    m_DSO * fDSO[2];
    int flag_in01_DSO;
    int flag_in10_DSO;
    TState_DSO2 state_DSO;    // -1 - ошибка, 0 - ничего, 1 - ось в парк, 2 - ось на горб
                      // устанавливается датчиком, сбрасывается по опросу состояния
                      // ошибка сбрасывается при корректной работе датчиков
};
Q_DECLARE_METATYPE(m_DSO2::TState_DSO2)
#endif // M_DSO2_H
