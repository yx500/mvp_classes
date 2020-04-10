#include "visual_elements.h"

#include <QDebug>
#include "v_model.h"
#include "visual_screen.h"
#include "baseobjecttools.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(Visual_Elements,"Список элементов","VISUAL БАЗОВЫЕ")


Visual_Elements::Visual_Elements(QObject *parent):
    v_Base(parent)
{
    setObjectName("Список элементов");
}

void Visual_Elements::updateAfterLoad()
{
    v_Base::updateAfterLoad();
}

void Visual_Elements::reCalculateGeometry()
{
    QList<v_Base *> lV=findChildren<v_Base *>();
    foreach (v_Base *V, lV) {
        V->setxy(V->xy());
        V->calculateGeometry();
        V->getProxyGraphicsItem()->update();
    }
}

QList<v_Model *> Visual_Elements::findElementsByModel(BaseObject *M)
{
    QList<v_Model *> l;
    // ищем по детям
    QList<v_Model *> lv=this->findChildren<v_Model *>();
    foreach(v_Model *v,lv){
        if (v->modelObject()==M) l.push_back(v);
    }
    return l;
}

bool Visual_Elements::isCanAddObject(QObject *O) const
{
    if (qobject_cast<v_Base*>(O)) return true;
    return false;
}


void Visual_Elements::updateStates()
{
    // если не на сцене ничего не делаем
    if ((!screen()) || (!screen()->getProxyGraphicsItem()->scene())) return;
    v_Base::updateStates();
}

