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
        regimUnknow=-1,
        regimRospusk=1,
        regimPausa=2,
        regimStop=0
    };

    MYPROP(SignalDescription,SIGNAL_ROSPUSK)
    MYPROP(SignalDescription,SIGNAL_PAUSA)
    MYPROP(SignalDescription,SIGNAL_STOP)
    MYSTATE(int,STATE_REGIM)
    Q_PROPERTY(int PUT_NADVIG READ PUT_NADVIG  DESIGNABLE true STORED false)
public:
    Q_INVOKABLE ModelGroupGorka(BaseObject *parent = nullptr);
    virtual ~ModelGroupGorka();
    virtual void resetStates();
    virtual void validation(ListObjStr *l) const;
    virtual void updateAfterLoad();

    virtual void updateStates(); // основной жизненный цикл

    QList<m_RC_Gor*> marshrut(int put_nadvig, int m);
    QMap<int,int> mSP2MAR;
    QMap<int,int> mMAR2SP;
    int PUT_NADVIG() const;

signals:
    void regimChanged(int oldRegim,int newRegim);



protected:
    //m_Otceps * Otceps;
    void updateRegim();
    QList<m_RC_Gor_ZKR*> lzkr;


};

#endif // MODELGROUPGORKA_H
