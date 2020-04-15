#include "baseobject.h"
#include <QMetaObject>
#include <QMetaProperty>
#include <utility>
#include "signaldescription.h"
#include "mvp_objectfactory.h"
#include "objectlink.h"


QString objectId2Str(const quint64 &id){
    return QString("%1").arg(id);
}
quint64 str2objectId(const QString &s){
    return s.toULongLong();
}



BaseObject::BaseObject(QObject *parent) :
    QObject(parent)
{
    FId=0;
    Fidstr.clear();
    xmlFile.clear();
}


// восстановление параметров после загрузки(линки)
void BaseObject::updateAfterLoad(){
    foreach(QObject * b,children()){
        auto *B=qobject_cast<BaseObject *>(b);
        if (B) B->updateAfterLoad();
    }
    // инициализируем буфера
    for (int idx = 0; idx < metaObject()->propertyCount(); idx++) {
        QMetaProperty metaProperty = metaObject()->property(idx);
        int type = metaProperty.userType();
        if (type == qMetaTypeId<SignalDescription>()){
            QVariant val=metaProperty.read(this);
            SignalDescription p = val.value<SignalDescription>();
            if (p.isNotUse()) continue;
            if (!p.isEmpty()){
                p.acceptGtBuffer();
            }
        }
    }
}


void BaseObject::setTag(int key, QVariant val)
{
    mTags[key]=std::move(val);
}

QVariant BaseObject::getTag(int key) const
{
    return mTags[key];
}



void BaseObject::updateStates()
{
    if (disableUpdateStates) return;
    foreach (QObject *O, children()) {
        auto B=qobject_cast<BaseObject*>(O);
        if ((B!=nullptr) && (!B->disableUpdateStates))
        {
            B->_prepare_updateStates();
            B->updateStates();
            B->_emit_after();
        }
    }

}

void BaseObject::resetStates()
{
    _stateChangedCount=0;
    QList<BaseObject*> l=findChildren<BaseObject*>(QString(),Qt::FindDirectChildrenOnly);
    foreach (BaseObject*B, l) {
        B->resetStates();
    }
}

void BaseObject::doPropertyChanged(){
    emit propertyChanged(this);
}
void BaseObject::doStateChanged(){
    _stateChangedCount++;
    if (onlyOneEmitEnabled)
            stateChangedEmit=true; else
            emit stateChanged(this);
}


void BaseObject::validationEmptySignals(ListObjStr *l) const
{
    for (int idx = 0; idx < metaObject()->propertyCount(); idx++) {
        QMetaProperty metaProperty = metaObject()->property(idx);
        int type = metaProperty.userType();
        if (type == qMetaTypeId<SignalDescription>()){
            QVariant val=metaProperty.read(this);
            SignalDescription p = val.value<SignalDescription>();
            if (p.isNotUse()) continue;
            if (p.isEmpty()){
                l->warning(this,"Сигнал не задан",
                           MVP_ObjectFactory::instance()->property_rusname(metaObject(),metaProperty.name()),
                           metaProperty.name()
                           );
            }
        }
    }
}

void BaseObject::validationEmptyLinks(ListObjStr *l) const
{
    for (int idx = 0; idx < metaObject()->propertyCount(); idx++) {
        QMetaProperty metaProperty = metaObject()->property(idx);
        int type = metaProperty.userType();
        if (type == qMetaTypeId<ObjectLink>()){
            QVariant val=metaProperty.read(this);
            ObjectLink p = val.value<ObjectLink>();
            if (p.isNotUse()) continue;
            if (p.isEmpty()){
                l->warning(this,"Объект не задан",
                           MVP_ObjectFactory::instance()->property_rusname(metaObject(),metaProperty.name()),
                           metaProperty.name()
                           );
            }
        }
    }
}

void BaseObject::_prepare_updateStates()
{
    onlyOneEmitEnabled=true;
    stateChangedEmit=false;
}

void BaseObject::_emit_after()
{
    if (stateChangedEmit) emit stateChanged(this);
    onlyOneEmitEnabled=false;
    stateChangedEmit=false;
}

void BaseObject::addTagObject(QObject *ob,int key)
{
    if (ob==nullptr) mTagObjects.remove(key);
    mTagObjects[key]=ob;
    if (ob){
        foreach (QPointer<QObject> P, vTagObjects) {
            if (P.data()==ob) return;
        }
        QPointer<QObject> P=ob;
        vTagObjects.push_back(P);
    }
}

void BaseObject::delTagObject(QObject *ob)
{
    foreach(auto p,mTagObjects.values()){
        if (p.data()==ob) mTagObjects.remove(mTagObjects.key(p));
    }
    if (ob){
        foreach (QPointer<QObject> P, vTagObjects) {
            if (P.data()==ob) vTagObjects.removeAll(P);
        }
    }
}

QObject* BaseObject::tagObject(int key) const
{
    if (mTagObjects.contains(key)) return mTagObjects[key].data();
    return nullptr;
}
QObjectList BaseObject::tagObjects() const
{
    //    QObjectList l;
    //    foreach (auto p, mTagObjects.values()) {
    //        l.push_back(p.data());
    //    }
    //    return l;
    QObjectList l;
    foreach (auto P, vTagObjects) {
        if (P.data()) l.push_back(P.data());
    }
    return l;
}
