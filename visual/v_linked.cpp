#include "v_linked.h"

#include "baseobjecttools.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_Linked,"Ссылка на визуал","VISUAL БАЗОВЫЕ")

v_Linked::v_Linked(QObject *parent) : v_Base(parent)
{
    FLNKVE.clear();
    vLinked=nullptr;
}


void v_Linked::updateAfterLoad()
{
    v_Base::updateAfterLoad();
    if (FLNKVE.id()!=0){
        BaseObject *B=superFindObjectById(this,FLNKVE.id());
        FLNKVE.linkObj(B);
        vLinked=qobject_cast<v_Base*>(B);
        if (!vLinked){
            qCritical() << "Ошибочная ссылка LNKVE ! " << objectName();
        }else {
            setObjectName(QString(">>%1").arg(vLinked->objectName()));
        }
    }
}

void v_Linked::setScreen(Visual_Screen *p)
{
    v_Base::setScreen(p);
    if (vLinked!=nullptr){
        // перескакиваем
        vLinked->setVisualParent(this);
        vLinked->setScreen(p);
    }
}


void v_Linked::setLNKVE(ObjectLink p)
{
    if (FLNKVE!=p){
        FLNKVE=p;
        if (FLNKVE.id()!=0){
            BaseObject *B=superFindObjectById(this,FLNKVE.id());
            vLinked=qobject_cast<v_Base*>(B);
            if (vLinked){
                setObjectName(QString(">>%1").arg(vLinked->objectName()));
            }
        }
    }
}
