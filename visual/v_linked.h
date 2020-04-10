#ifndef V_LINKED_H
#define V_LINKED_H

#include "v_base.h"
#include "objectlink.h"

class v_Linked : public v_Base
{
    Q_OBJECT
    Q_PROPERTY(ObjectLink   LNKVE READ LNKVE WRITE setLNKVE DESIGNABLE true)
public:
    Q_INVOKABLE v_Linked(QObject *parent = nullptr);
    ObjectLink LNKVE() const {return FLNKVE;}
    void setLNKVE(ObjectLink p);
    virtual void updateAfterLoad();
    void setScreen(Visual_Screen *p);

signals:

public slots:
protected:
    ObjectLink FLNKVE;
    v_Base *vLinked;
};

#endif // V_LINKED_H
