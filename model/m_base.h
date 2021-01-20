#ifndef M_BASE_H
#define M_BASE_H

#include "baseobject.h"
#include "signaldescription.h"

// выделенный класс для моделей
// выделено свойство  STATE_33

class LIBSHARED_EXPORT m_Base : public BaseObject
{
    Q_OBJECT

public:
    MYPROP(int, OLD_ID)
    Q_PROPERTY(bool STATE_33 READ STATE_33 WRITE setSTATE_33 DESIGNABLE true STORED false) \
    MYPROP(QString,EXTFLAGSRC)
    void setSTATE_33(bool p);
    bool STATE_33() const {return FSTATE_33;}
    bool setSignalState(const SignalDescription &s,bool &state);
public:
    explicit m_Base(QObject *parent = nullptr);
    virtual ~m_Base(){}
    virtual void validation(ListObjStr *l) const;
    virtual void resetStates();
    virtual QString defaultGroupName() const {return "";}
    static QObject* modelRootGroup(m_Base *m);

    virtual bool is33(){return false;}
    virtual void updateStates();

protected:
    bool FSTATE_33;

};

#endif // M_BASE_H
