#include "visualgrouppropertyes.h"

#include <QDebug>

#include "mvp_objectfactory.h"
#include "v_base.h"
REGISTERELEMENT(VisualGroupPropertyes,"Список общих свойств","VISUAL БАЗОВЫЕ")


VisualGroupPropertyes::VisualGroupPropertyes(QObject *parent) : BaseObject(parent)
{
    target=nullptr;
}

VisualGroupProperty *VisualGroupPropertyes::visualGroupProperty(QString propertyName)
{
    QList<VisualGroupProperty *> l=findChildren<VisualGroupProperty *>(propertyName);
    if (l.size()!=0) return l.first();
    return 0;
}


VisualGroupProperty *VisualGroupPropertyes::add(QString objectPropName, QString groupPropName, QObject *O)
{
    if (mName2Prop.contains(groupPropName))  return mName2Prop[groupPropName];

        if (O->metaObject()->indexOfProperty(qPrintable(objectPropName))<0) return nullptr;
        VisualGroupProperty *p=new VisualGroupProperty(this);
        p->setParent(this);
        p->blockSignals(true);
        p->setObjectName(MVP_ObjectFactory::instance()->property_rusname(O->metaObject(),qPrintable(objectPropName)));
        p->setidstr(groupPropName);
        p->setV(O->property(qPrintable(objectPropName)));
        blockSignals(false);
        connect(p,SIGNAL(propertyChanged(QObject*)),this,SLOT(slot_propertyChanged(QObject*)));
        mName2Prop[groupPropName]=p;
    return p;
}

void VisualGroupPropertyes::updateAfterLoad()
{
    BaseObject::updateAfterLoad();
    QList<VisualGroupProperty *> l=findChildren<VisualGroupProperty *>();
    foreach (VisualGroupProperty*p, l) {
        mName2Prop[p->idstr()]=p;
        connect(p,SIGNAL(propertyChanged(QObject*)),this,SLOT(slot_propertyChanged(QObject*)));
    }
}

void VisualGroupPropertyes::collectGroupPropertyes(v_Base *p)
{
    target=p;
    if (!target) return;
    QList<v_Base*> lv=target->findChildrenVisual<v_Base*>();
    foreach (v_Base*vb, lv) {
        foreach (const QString &objectPropertyName,vb->COMMON_PROPERTYESH().keys()){
            QString groupPropertyName=vb->COMMON_PROPERTYESH().value(objectPropertyName).toString();
            if (!mName2Prop.contains(groupPropertyName)){
                add(objectPropertyName,groupPropertyName,vb);
            }
        }
    }
    updateObjectsPropertyes();
}

void VisualGroupPropertyes::slot_propertyChanged(QObject *O)
{
    updateObjectsPropertyes();
}

void VisualGroupPropertyes::updateObjectsPropertyes()
{
    if (!target) return;
    QList<v_Base*> lv=target->findChildrenVisual<v_Base*>();
    QList<VisualGroupProperty *> lp=findChildren<VisualGroupProperty *>();
    foreach (VisualGroupProperty *vp, lp) {
        QString groupPropertyName=vp->idstr();
        QVariant groupPropertyVal=vp->V();
        foreach (v_Base*vb, lv) {
            foreach (const QString &objectPropertyName,vb->COMMON_PROPERTYESH().keys()){
                QString groupPropertyName2=vb->COMMON_PROPERTYESH().value(objectPropertyName).toString();
                if (groupPropertyName==groupPropertyName2){
                    QVariant V;
                    V=vb->property(qPrintable(objectPropertyName));
                    if (V.isValid()){
                            vb->setProperty(qPrintable(objectPropertyName),groupPropertyVal);
                    }
                }
            }
        }
    }
}

