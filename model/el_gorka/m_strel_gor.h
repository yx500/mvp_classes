#ifndef M_STREL_GOR_H
#define M_STREL_GOR_H
#include "m_rc_gor.h"

class m_Strel_Gor: public m_RC_Gor
{
    Q_OBJECT
public:
    MYPROP(SignalDescription, SIGNAL_PLUS)
    MYPROP(SignalDescription, SIGNAL_MINUS)
    MYPROP(SignalDescription, SIGNAL_BLOCK)

    virtual void setSTATE_POL(MVP_Enums::TStrelPol p);
    virtual MVP_Enums::TStrelPol STATE_POL() const {return FSTATE_POL;}

    virtual int DIRECTM()const {return FDIRECTM;}
    virtual void setDIRECTM(int p){SETPROP(FDIRECTM);}


public:

    Q_INVOKABLE m_Strel_Gor(QObject *parent = nullptr);
    virtual ~m_Strel_Gor(){}
    virtual void validation(ListObjStr *l) const;
    virtual void resetStates();

    virtual int getNextCount(int d) const ;
    virtual bool is33();
    virtual void updateStates();

protected:
    MVP_Enums::TStrelPol FSTATE_POL;
    int FDIRECTM;

};

#endif // M_STREL_GOR_H
