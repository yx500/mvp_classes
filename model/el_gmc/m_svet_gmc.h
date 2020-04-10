#ifndef M_SVET_GMC_H
#define M_SVET_GMC_H

#include "m_svet.h"

class m_Svet_GMC : public m_Svet
{
    Q_OBJECT
public:

    Q_PROPERTY(int DBK_LINE READ DBK_LINE WRITE setDBK_LINE DESIGNABLE true )
    Q_PROPERTY(int DBK_NUM READ DBK_NUM WRITE setDBK_NUM DESIGNABLE true )
    Q_PROPERTY(int DBK_OBJ READ DBK_OBJ WRITE setDBK_OBJ DESIGNABLE true )

    Q_PROPERTY(SignalDescription SIGNAL_GMC_OPEN_ENABLE READ SIGNAL_GMC_OPEN_ENABLE DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_GMC_BLOCK READ SIGNAL_GMC_BLOCK DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_GMC_TU_OPEN READ SIGNAL_GMC_TU_OPEN DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_GMC_TU_CLOSE READ SIGNAL_GMC_TU_CLOSE DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_GMC_ERROR READ SIGNAL_GMC_ERROR DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_GMC_OPEN_CONDITION READ SIGNAL_GMC_OPEN_CONDITION DESIGNABLE true)

    Q_PROPERTY(SignalDescription SIGNAL_DBK_OPEN0       READ SIGNAL_DBK_OPEN0 DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_DBK_OPEN_BLINK0 READ SIGNAL_DBK_OPEN_BLINK0 DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_DBK_ERROR0      READ SIGNAL_DBK_ERROR0 DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_DBK_OPEN1       READ SIGNAL_DBK_OPEN1 DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_DBK_OPEN_BLINK1 READ SIGNAL_DBK_OPEN_BLINK1 DESIGNABLE true)
    Q_PROPERTY(SignalDescription SIGNAL_DBK_ERROR1      READ SIGNAL_DBK_ERROR1 DESIGNABLE true)






    MYSTATE(bool, STATE_GMC_OPEN_ENABLE)
    MYSTATE(bool, STATE_GMC_BLOCK)
    MYSTATE(bool, STATE_GMC_TU_OPEN)
    MYSTATE(bool, STATE_GMC_TU_CLOSE)
    MYSTATE(bool, STATE_GMC_ERROR)
    MYSTATE(bool, STATE_GMC_OPEN_CONDITION)

    MYSTATE(bool, STATE_DBK_OPEN0)
    MYSTATE(bool, STATE_DBK_OPENBLINK0)
    MYSTATE(bool, STATE_DBK_ERROR0)
    MYSTATE(bool, STATE_DBK_OPEN1)
    MYSTATE(bool, STATE_DBK_OPENBLINK1)
    MYSTATE(bool, STATE_DBK_ERROR1)

    SignalDescription SIGNAL_GMC_OPEN_ENABLE() const {return FSIGNAL_GMC_OPEN_ENABLE;}
    SignalDescription SIGNAL_GMC_BLOCK() const {return FSIGNAL_GMC_BLOCK;}
    SignalDescription SIGNAL_GMC_TU_OPEN() const {return FSIGNAL_GMC_TU_OPEN;}
    SignalDescription SIGNAL_GMC_TU_CLOSE() const {return FSIGNAL_GMC_TU_CLOSE;}
    SignalDescription SIGNAL_GMC_ERROR() const {return FSIGNAL_GMC_ERROR;}
    SignalDescription SIGNAL_GMC_OPEN_CONDITION() const {return FSIGNAL_GMC_OPEN_CONDITION;}

    SignalDescription SIGNAL_DBK_OPEN0() const {return FSIGNAL_DBK_OPEN[0];}
    SignalDescription SIGNAL_DBK_OPEN_BLINK0() const {return FSIGNAL_DBK_OPEN_BLINK[0];}
    SignalDescription SIGNAL_DBK_ERROR0() const {return FSIGNAL_DBK_ERROR[0];}

    SignalDescription SIGNAL_DBK_OPEN1() const {return FSIGNAL_DBK_OPEN[1];}
    SignalDescription SIGNAL_DBK_OPEN_BLINK1() const {return FSIGNAL_DBK_OPEN_BLINK[1];}
    SignalDescription SIGNAL_DBK_ERROR1() const {return FSIGNAL_DBK_ERROR[1];}


    int DBK_LINE()const{return FDBK_LINE;}
    int DBK_NUM()const{return FDBK_NUM;}
    int DBK_OBJ() const{return FDBK_OBJ;}
    void setDBK_LINE(int p);
    void setDBK_NUM(int p);
    void setDBK_OBJ(int p);


public:
    Q_INVOKABLE m_Svet_GMC(QObject *parent = 0);
    virtual ~m_Svet_GMC(){}
    virtual void validation(ListObjStr *l) const;
    virtual void updateAfterLoad();
    virtual bool is33();
    virtual void updateStates();

protected:
    void updateSignals();
    int FDBK_LINE;
    int FDBK_NUM;
    int FDBK_OBJ;
    SignalDescription FSIGNAL_GMC_OPEN_ENABLE;
    SignalDescription FSIGNAL_GMC_BLOCK;
    SignalDescription FSIGNAL_GMC_TU_OPEN;
    SignalDescription FSIGNAL_GMC_TU_CLOSE;
    SignalDescription FSIGNAL_GMC_ERROR;
    SignalDescription FSIGNAL_GMC_OPEN_CONDITION;
    SignalDescription FSIGNAL_DBK_OPEN[5];
    SignalDescription FSIGNAL_DBK_OPEN_BLINK[5];
    SignalDescription FSIGNAL_DBK_ERROR[5];
};

#endif // M_SVET_GMC_H
