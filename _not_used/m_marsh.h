#ifndef M_MARSH_H
#define M_MARSH_H

#include "m_marsh_str.h"

class m_Marsh : public m_Base
{
    Q_OBJECT
    MYPROP(int, GROUP)
    MYPROP(ObjectLink, SV1)
    MYPROP(ObjectLink, SV2)
    Q_PROPERTY(bool USTANOVLEN READ USTANOVLEN DESIGNABLE true STORED false)
public:
    Q_INVOKABLE m_Marsh(QObject *parent = 0);
    virtual ~m_Marsh(){}
    virtual QString defaultGroupName() const {return "Список маршрутов";}

    bool USTANOVLEN();
    void setUSTANOVLEN(bool p);
    QList<m_Marsh_Str*> marsh_Str(){return lMarsh_Str;}

    virtual void updateAfterLoad();
    void  addMSTR(m_RC *rc,MVP_Enums::TStrelPol pol);

protected:
    QList<m_Marsh_Str*> lMarsh_Str;
public slots:
    virtual void updateStates();
};

#endif // M_MARSH_H
