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

struct RcInMarsrut{
    m_RC_Gor *rc;
    int pol;
};

struct Marsrut{
    QList<RcInMarsrut> l_rc;
};
struct MarsrutsOnPut{
    QMap<int,Marsrut*> mN2M;
};

inline bool inway(int way,int minway,int maxway)
{
    if ((way>0)&&(way>=minway)&&(way<=maxway)) return true;
    return false;
}

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
    MYPROP(SignalDescription,SIGNAL_RRC)
    MYPROP(SignalDescription,SIGNAL_RRC_TU)
    MYPROP(SignalDescription,SIGNAL_GAC_FINISH)
    MYPROP(ObjectLink,LNK_OTCEPS)

    MYSTATE(int,STATE_REGIM)
    MYSTATE(int,STATE_PUT_NADVIG)
    MYSTATE(bool,STATE_RRC)
    MYSTATE(bool,STATE_GAC_FINISH)
public:
    Q_INVOKABLE ModelGroupGorka(BaseObject *parent = nullptr);
    virtual ~ModelGroupGorka();
    virtual void resetStates();
    virtual void validation(ListObjStr *l) const;
    virtual void updateAfterLoad();
    virtual bool is33();
    virtual void updateStates(); // основной жизненный цикл

    const Marsrut *getMarshrut(int putNadvig,int nm) const;

    QMap<int,int> mSP2MAR;
    QMap<int,int> mMAR2SP;

    m_RC_Gor_ZKR *active_zkr() const;

signals:
    void regimChanged(int oldRegim,int newRegim);



protected:
    //m_Otceps * Otceps;
    void updateRegim();
    QList<m_RC_Gor_ZKR*> l_zkr;
    QMap<int,MarsrutsOnPut> mZ2M;


};

#endif // MODELGROUPGORKA_H
