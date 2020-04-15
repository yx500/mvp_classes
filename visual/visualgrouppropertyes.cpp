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



//qreal VisualGroupPropertyes::viewProperty_qreal(const VisualGroupProperty_qreal &prop, qreal elementValue)
//{
//    VisualGroupProperty * p=0;
//    if (mName2Prop.contains(prop.name)){
//        p=mName2Prop[prop.name];
//    }

//    if (p==0) {
//        p=new VisualGroupProperty(this);
//        p->setParent(this);
//        p->setVALUE(prop.elementValueD);
//        p->setVALUE_DEF(prop.elementValueR);
//        p->setObjectName(prop.text);
//        p->setidstr(prop.name);
//        connect(p,SIGNAL(propertyChanged(QObject*)),this,SLOT(slot_propertyChanged(QObject*)));
//        mName2Prop[prop.name]=p;
//    }
//    if (p->VALUE_DEF()==elementValue) return p->VALUE();
//    return elementValue;
//}

//int VisualGroupPropertyes::viewProperty_int(const VisualGroupProperty_int &prop, int elementValue)
//{
//    VisualGroupProperty * p=0;
//    if (mName2Prop.contains(prop.name)){
//        p=mName2Prop[prop.name];
//    }

//    if (p==0) {
//        p=new VisualGroupProperty(this);
//        p->setParent(this);
//        p->setVALUE(prop.elementValueD);
//        p->setVALUE_DEF(prop.elementValueR);
//        p->setObjectName(prop.text);
//        p->setidstr(prop.name);
//        connect(p,SIGNAL(propertyChanged(QObject*)),this,SLOT(slot_propertyChanged(QObject*)));
//        mName2Prop[prop.name]=p;
//    }
//    if (p->VALUE_DEF()==elementValue) return p->VALUE();
//    return elementValue;
//}

VisualGroupProperty *VisualGroupPropertyes::add(QString objectPropName, QString groupPropName, QObject *O)
{
    VisualGroupProperty * p=0;
    if (mName2Prop.contains(groupPropName)){
        p=mName2Prop[groupPropName];
        if (!p->property("VAL").isValid()){
            QVariant V=O->property(qPrintable(objectPropName));
            p->setProperty("VAL",V);
        }
    }

    if (p==0) {
        if (O->metaObject()->indexOfProperty(qPrintable(objectPropName))<0) return 0;
        p=new VisualGroupProperty(this);
        p->setParent(this);

        //        p->setVALUE(0);
        //        p->setVALUE_DEF(0);
        p->setObjectName(MVP_ObjectFactory::instance()->property_rusname(O->metaObject(),qPrintable(objectPropName)));
        p->setidstr(groupPropName);
        QVariant V=O->property(qPrintable(objectPropName));
        p->blockSignals(true);
        p->setProperty("VAL",V);
        blockSignals(false);
        QString S;
        MVP_ObjectFactory::instance()->QVariantToQString( O->property(qPrintable(objectPropName)),S);
        p->setSTORED_VALUE(S);
        connect(p,SIGNAL(propertyChanged(QObject*)),this,SLOT(slot_propertyChanged(QObject*)));
        mName2Prop[groupPropName]=p;
    }
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

void VisualGroupPropertyes::setTarget(v_Base *p)
{
    target=p;
    if (!target) return;
    QList<v_Base*> lv=target->findChildrenVisual<v_Base*>();
    foreach (v_Base*vb, lv) {
        foreach (const QString &groupPropertyName,vb->COMMON_PROPERTYESG2S().keys()){
            if (!mName2Prop.contains(groupPropertyName)){
                const QString &objectPropertyName=vb->COMMON_PROPERTYESG2S().value(groupPropertyName);
                add(objectPropertyName,groupPropertyName,vb);
            } else {
                VisualGroupProperty *vp=mName2Prop[groupPropertyName];
                if (!vp->property("VAL").isValid()){
                    const QString &objectPropertyName=vb->COMMON_PROPERTYESG2S().value(groupPropertyName);
                    QVariant V=vb->property(qPrintable(objectPropertyName));
                    bool b=vp->blockSignals(true);
                    vp->setProperty("VAL",V);
                    vp->blockSignals(b);
                }
            }
        }
    }
    updateGroupPropertyes();
}

void VisualGroupPropertyes::slot_propertyChanged(QObject *O)
{
    updateGroupPropertyes();
    emit visualGroupPropertyesChanged(O);
}

void VisualGroupPropertyes::updateGroupPropertyes()
{
    if (!target) return;
    QList<v_Base*> lv=target->findChildrenVisual<v_Base*>();
    QList<VisualGroupProperty *> lp=findChildren<VisualGroupProperty *>();
    foreach (VisualGroupProperty *vp, lp) {
        QString groupPropertyName=vp->idstr();
        QString groupPropertyVal=vp->STORED_VALUE();
        foreach (v_Base*vb, lv) {
            if (vb->COMMON_PROPERTYESG2S().contains(groupPropertyName)){
                const QString &objectPropertyName=vb->COMMON_PROPERTYESG2S().value(groupPropertyName);
                QVariant V;
                V=vb->property(qPrintable(objectPropertyName));
                if (V.isValid()){
                    if (MVP_ObjectFactory::instance()->QVariantFromQString(V,groupPropertyVal)){
                        vb->setProperty(qPrintable(objectPropertyName),V);
                    }
                }
            }
        }
    }
}

