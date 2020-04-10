#ifndef MODELGROUPGORKA_H
#define MODELGROUPGORKA_H
#include "modelrootgroup.h"
#include "m_rc_gor_park.h"
#include "m_rc_gor_zam.h"
#include "m_rc_gor_zkr.h"
#include "m_strel_gor_y.h"
#include "m_strel_gor.h"
#include "m_otceps.h"

// для горок
// добалены исключительные сигналы РОСПУСК ПАУЗА СТОП

class LIBSHARED_EXPORT ModelGroupGorka : public ModelRootGroup
{
    Q_OBJECT

public:
    enum RegimRospusk{
        regimUnknow=0,
        regimRospusk,
        regimPausa,
        regimStop
    };

    MYPROP(SignalDescription,SIGNAL_ROSPUSK)
    MYPROP(SignalDescription,SIGNAL_PAUSA)
    MYPROP(SignalDescription,SIGNAL_STOP)
    MYSTATE(int,STATE_REGIM)
public:
    Q_INVOKABLE ModelGroupGorka(BaseObject *parent = nullptr);
    virtual ~ModelGroupGorka();
    virtual void resetStates();
    virtual void validation(ListObjStr *l) const;
    virtual void updateAfterLoad();

    virtual void updateStates(); // основной жизненный цикл

signals:
    void regimChanged(int oldRegim,int newRegim);



protected:
    //m_Otceps * Otceps;
    void updateRegim();

};

#endif // MODELGROUPGORKA_H
