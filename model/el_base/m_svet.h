#ifndef M_SVET_H
#define M_SVET_H
/*
 * базовый класс для всех моделей светофоров
 *
 * */
#include "m_base.h"
#include "signaldescription.h"
class m_Svet : public m_Base
{
    Q_OBJECT

public:
    MYSTATE(bool, STATE_OPEN)

public:
    Q_INVOKABLE m_Svet(QObject *parent = 0);
    virtual ~m_Svet(){}
    virtual void resetStates();
    virtual QString defaultGroupName() const {return "СВТ";}

signals:

protected:
};

#endif // M_SVET_H
