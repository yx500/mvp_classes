#ifndef M_MARSH_STR_H
#define M_MARSH_STR_H
#include "m_base.h"
#include "signaldescription.h"
#include "m_rc.h"

class m_Marsh_Str : public m_Base
{
    Q_OBJECT
    Q_PROPERTY(ObjectLink LINKRC READ LINKRC WRITE setLINKRC DESIGNABLE true )
public:
    MYPROP(MVP_Enums::TStrelPol, POL)

    ObjectLink LINKRC(){return FLINKRC;}
    void setLINKRC(ObjectLink p);
public:
    Q_INVOKABLE m_Marsh_Str(QObject *parent = 0);
    virtual ~m_Marsh_Str(){}
    virtual QString defaultGroupName() const {return "Список маршрутов";}
    virtual QString defaultVisualElement() const {return "";}
    virtual void updateAfterLoad();

    bool MPOL();
    void setMPOL();
    void setIN_MARSH(bool p);
    m_RC *RC(){return FRC;}


signals:

public slots:
    virtual void updateStates();
protected:
    ObjectLink FLINKRC;
    m_RC *FRC;
};

#endif // M_MARSH_STR_H
