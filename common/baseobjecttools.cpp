#include "baseobjecttools.h"

#include <QMetaProperty>
#include "objectlink.h"
#include "station.h"


const QObject *superParent(const QObject *O)
{
    QObject *p=O->parent();
    if (p!=nullptr) return superParent(p);
    return O;
}


bool isPropertyExists(QObject *O, const QString &propertyName)
{
    int ind=O->metaObject()->indexOfProperty(qPrintable(propertyName));
    return ind>=0;
}

void setPropertyValue(QObject *O, const QString &propertyName, const QVariant &v)
{
    const QMetaObject *   metaO = O->metaObject();
    int ind=metaO->indexOfProperty(qPrintable(propertyName));
    if (ind>=0){
        QMetaProperty prop=metaO->property(ind);
        prop.write(O,v);
    }
}

BaseObject *findObjectById(const QObject *O, const quint64 &id)
{
    const auto *B=qobject_cast<const BaseObject *>(O);
    if ((B)&&(B->id()==id)) return const_cast<BaseObject *>(B);
    foreach(QObject * o,O->children()){
        BaseObject *BF=findObjectById(o,id);
        if (BF!=nullptr) return BF;
    }
    return nullptr;
}

QList<BaseObject *> findObjectByName(QObject *O, const QString &name)
{
    QList<BaseObject *> l;
    auto *B=qobject_cast<BaseObject *>(O);
    if ((B)&&(B->objectName()==name)) l.push_back(B);
    foreach(QObject * o,O->children()){
        QList<BaseObject *> l1=findObjectByName(o,name);
        if (!l1.empty()) l+=l1;
    }
    return l;
}
QList<BaseObject *> findObjectByIdstr(QObject *O, const QString &idstr)
{
    QList<BaseObject *> l;
    auto *B=qobject_cast<BaseObject *>(O);
    if ((B)&&(B->idstr()==idstr)) l.push_back(B);
    foreach(QObject * o,O->children()){
        QList<BaseObject *> l1=findObjectByIdstr(o,idstr);
        if (!l1.empty()) l+=l1;
    }
    return l;
}

BaseObject *superFindObjectById(QObject *O,const quint64 &id)
{
    if (O==nullptr) return nullptr;
    if (id==0) return nullptr;
    auto B = qobject_cast<BaseObject *>(O);
    if ((B)&&(B->id()==id)) return B;

    const QObject * superP=superParent(O);
    if (superP!=nullptr) return findObjectById(superP,id);
    return nullptr;
}
BaseObject *reLink(const QObject *O,ObjectLink &lnk)
{
    if (lnk.id()==0) {
        lnk.clear();
        return nullptr;
    }
    if (O==nullptr) return nullptr;
    const auto *B=qobject_cast<const BaseObject *>(O);
    if ((B)&&(lnk.id()==B->id())) {
        lnk.linkObj(B);
        return const_cast<BaseObject *>(B);
    }
    const QObject * superP=superParent(O);
    if (superP!=nullptr) B=findObjectById(superP,lnk.id());
    if (B) lnk.linkObj(B);
    return const_cast<BaseObject *>(B);
}




QList<BaseObject *> listAllBaseObjects(QObject *O)
{
    QList<QObject *> lo=O->findChildren<QObject *>();
    QList<BaseObject *> l;
    foreach(QObject * o,lo){
        auto *B=qobject_cast<BaseObject *>(o);
        if (B) l.push_back(B);
    }
        return l;
}

bool isInherited(const QMetaObject *o,const QString& className)
{
    while (o!=nullptr){
        QString className1=QString(o->className());
        if (className==className1)
            return true;
        o=o->superClass();
    }
    return false;
}


bool isChildrenExists(QObject *parent, QObject *child)
{
    if (parent==nullptr) return false;
    if (child==nullptr) return false;
    if (parent->children().indexOf(child)>=0) return true;
    foreach (QObject *O, parent->children()) {
        if (isChildrenExists(O, child)) return true;
    }
    return false;
}

QObject *firstParentInheritedFromClass(QObject *child, const QString& className)
{
    if (child==nullptr) return nullptr;
    QObject *p=child->parent();
    while (p!=nullptr){
        if (isInherited(p->metaObject(),className)) return p;
        p=p->parent();
    }
    return nullptr;
}


int objectCountLinked2Object(const BaseObject *linkedB, const QObject *_superParent)
{
    // считаем кол-во линков по всему дерову от с-папы startB
    const QObject *_superP=nullptr;
    if (_superParent==nullptr) _superP=superParent(linkedB);
    if (_superP==nullptr) return 0;
    QList<BaseObject*> l=_superP->findChildren<BaseObject*>();
    int cnt=0;
    foreach(BaseObject * b,l){
        if (isB1hasLinkToB2(b,linkedB))  cnt++;
    }
    return cnt;
}


QList<BaseObject *> listAllObjectsLinked2B2(const BaseObject *B2, const QObject *_superParent)
{
    QList<BaseObject *> lResult;
    if (_superParent==nullptr) _superParent=superParent(B2);
    if (_superParent==nullptr) return lResult;
    QList<BaseObject*> l=_superParent->findChildren<BaseObject*>();
    foreach(BaseObject * b,l){
        if (isB1hasLinkToB2(b,B2))  lResult.push_back(b);
    }
    return lResult;
}

QStringList linkedInfo(const BaseObject * B2, const QObject *_superParent)
{
    QStringList lres;
    if (_superParent==nullptr) _superParent=superParent(B2);
    if (_superParent==nullptr) return lres;
    QList<BaseObject *>l= listAllObjectsLinked2B2(B2, _superParent);
    foreach (BaseObject *B1, l) {
        QStringList l1=listPropertyesB1hasLinkToB2(B1,B2,B1->objectName()+".");
        lres+=l1;
    }
    return lres;
}

void unlinkAllObjects2B2(const QObject * O2, const QObject *_superParent)
{
    const auto * B2=qobject_cast<const BaseObject *>(O2);
    if (B2) unlinkAllObjects2B2(B2, _superParent);
}

void unlinkAllObjects2B2(const BaseObject * B2, const QObject *_superParent)
{
    if (_superParent==nullptr) _superParent=superParent(B2);
    if (_superParent==nullptr) return ;
    QList<BaseObject *> l=listAllObjectsLinked2B2(B2, _superParent);
    foreach(BaseObject * b,l){
            unlinkB1fromB2(b,B2);
    }
}


//QList<BaseObject *> listLinkedObjects(BaseObject * B2,QObject *superParent)
//{
//    QList<QObject *> lres;
//    if (superParent==0) superParent=superParent(B2);
//    if (SuperParent==0) return lres;

//    QList<QObject *> l=rootO->findChildren<QObject *>();
//    foreach (QObject *O1, l) {
//        if (O1==O) continue;
//        const QMetaObject *   metaO = O1->metaObject();
//        for(int i = 0; i < metaO->propertyCount(); i++)
//        {
//            QMetaProperty prop = metaO->property(i);
//            if (prop.userType()!=qMetaTypeId<ObjectLink>()) continue;
//            QVariant val = prop.read(O1);
//            ObjectLink p =  val.value<ObjectLink>();
//            QString pn=QString(prop.name());
//            if (p.obj()==O) lres.push_back(O1);
//        }
//    }
//    return lres;
//}

Station * stationParent(QObject *O)
{
    QObject *p=O->parent();
    while(p!=nullptr){
        if (qobject_cast<Station*>(p)!=nullptr) return qobject_cast<Station*>(p);
        p=p->parent();

    }
    return nullptr;
}
Station * superStationParent(QObject *O)
{
    Station *result=nullptr;
    QObject *p=O->parent();
    while(p!=nullptr){
        if (qobject_cast<Station*>(p)!=nullptr) result=qobject_cast<Station*>(p);
        p=p->parent();

    }
    return result;
}


void updateLinksNum(BaseObject * B){  // обновляет числовое значение ид,нужно если объект на который ссылаются перегенерил id
    const QMetaObject *metaObject=B->metaObject();
    for (int idx = 0; idx < metaObject->propertyCount(); idx++) {
        QMetaProperty metaProperty = metaObject->property(idx);
        int type = metaProperty.userType();
        if (type == qMetaTypeId<ObjectLink>()){
            QVariant val=metaProperty.read(B);
            ObjectLink p = val.value<ObjectLink>();
            if (!p.isNull()&&(p.baseObject()!=nullptr)){
                if (p.id()!=p.baseObject()->id()){
                    p.linkObj(p.baseObject());
                    // заменим на неравный, чтоб сработал сеттер
                    val.setValue(ObjectLink()); metaProperty.write(B,val);

                    val.setValue(p);
                    metaProperty.write(B,val);


                }
            }
        }
    }
    foreach(QObject * b,B->children()){
        auto *sB=qobject_cast<BaseObject *>(b);
        if (sB) updateLinksNum(sB);
    }
}

void updateLinksObjects(BaseObject * B,BaseObject * SParent){  //
    const QMetaObject *metaObject=B->metaObject();
    for (int idx = 0; idx < metaObject->propertyCount(); idx++) {
        QMetaProperty metaProperty = metaObject->property(idx);
        int type = metaProperty.userType();
        if (type == qMetaTypeId<ObjectLink>()){
            QVariant val=metaProperty.read(B);
            ObjectLink p = val.value<ObjectLink>();
            if (p.id()!=0){
                BaseObject *B1=superFindObjectById(SParent,p.id());
                if (B1){
                    p.linkObj(B1);
                    // заменим на неравный, чтоб сработал сеттер
                    val.setValue(ObjectLink()); metaProperty.write(B,val);
                    val.setValue(p);
                    metaProperty.write(B,val);
                }

            }
        }
    }
    foreach(QObject * b,B->children()){
        auto *sB=qobject_cast<BaseObject *>(b);
        if (sB) updateLinksObjects(sB,SParent);
    }
}



void deleteBaseObject(BaseObject * B, QObject *_superParent)
{
    // отлинкуемся от всех
    // нудно пройдемся по всему дереву superParent  и отрубим B
    unlinkAllObjects2B2(B,_superParent);
    B->setParent(nullptr);
    delete B;
}

bool isB1hasLinkToB2(const BaseObject *B1,const BaseObject *B2)
{
    const QMetaObject *metaObject=B1->metaObject();
    for (int idx = 0; idx < metaObject->propertyCount(); idx++) {
        QMetaProperty metaProperty = metaObject->property(idx);
        int type = metaProperty.userType();
        if (type == qMetaTypeId<ObjectLink>()){
            QVariant val=metaProperty.read(B1);
            ObjectLink p = val.value<ObjectLink>();
            if (!p.isNull()&&(p.baseObject()!=nullptr)){
                if (p.id()==B2->id()){
                    return true;
                }
            }
        }
    }
    return false;
}

QStringList listPropertyesB1hasLinkToB2(const BaseObject *B1,const BaseObject *B2,const QString& prefix)
{
    QStringList lres;
    const QMetaObject *metaObject=B1->metaObject();
    for (int idx = 0; idx < metaObject->propertyCount(); idx++) {
        QMetaProperty metaProperty = metaObject->property(idx);
        int type = metaProperty.userType();
        if (type == qMetaTypeId<ObjectLink>()){
            QVariant val=metaProperty.read(B1);
            ObjectLink p = val.value<ObjectLink>();
            if (!p.isNull()&&(p.baseObject()!=nullptr)){
                if (p.id()==B2->id()){
                    QString s=prefix+QString(metaProperty.name());
                    lres.push_back(s);
                }
            }
        }
    }
    return lres;
}

void unlinkB1fromB2(BaseObject *B1,const BaseObject *B2)
{
    // ищем у B1 все свойства с линком на B2 и обнуляем
    const QMetaObject *metaObject=B1->metaObject();
    for (int idx = 0; idx < metaObject->propertyCount(); idx++) {
        QMetaProperty metaProperty = metaObject->property(idx);
        int type = metaProperty.userType();
        if (type == qMetaTypeId<ObjectLink>()){
            QVariant val=metaProperty.read(B1);
            ObjectLink p = val.value<ObjectLink>();
            if (!p.isNull()&&(p.baseObject()!=nullptr)){
                if (p.id()==B2->id()){
                    p.clear();
                    val.setValue(p);
                    metaProperty.write(B1,val);
                }
            }
        }
    }
}


QObjectList linkedObjects(const QObject *O)
{
    QObjectList l;
    const QMetaObject *metaObject=O->metaObject();
    for (int idx = 0; idx < metaObject->propertyCount(); idx++) {
        QMetaProperty metaProperty = metaObject->property(idx);
        int type = metaProperty.userType();
        if (type == qMetaTypeId<ObjectLink>()){
            QVariant val=metaProperty.read(O);
            ObjectLink p = val.value<ObjectLink>();
            if (p.obj()!=nullptr){
                l.push_back(p.obj());
            }
        }
    }
    return l;
}


void replaceLinks(BaseObject *B, quint64 oldId, const quint64 &newId)
{
    // Заменяем все линки на новые
    const QMetaObject *metaObject=B->metaObject();
    for (int idx = 0; idx < metaObject->propertyCount(); idx++) {
        QMetaProperty metaProperty = metaObject->property(idx);
        int type = metaProperty.userType();
        if (type == qMetaTypeId<ObjectLink>()){
            QVariant val=metaProperty.read(B);
            ObjectLink p = val.value<ObjectLink>();
            if (p.id()!=0){
                if (p.id()==oldId){
                    p.setId(newId);
                    val.setValue(p);
                    metaProperty.write(B,val);
                }
            }
        }
    }
}



QObject *findIDSTRObject(QObject *O,QString &IDSTR){
    QVariant value = O->property(qPrintable("IDSTR"));
    if ((value.isValid()) && (value.toString()==IDSTR)) return O;
    foreach (QObject *childO, O->children()) {
        QObject *foundO=findIDSTRObject(childO,IDSTR);
        if (foundO) return foundO;
    }
    return nullptr;

}

void resetBaseObjectStates(QObject *O)
{
    const QMetaObject *metaObject=O->metaObject();
    for (int idx = 0; idx < metaObject->propertyCount(); idx++) {
        QMetaProperty metaProperty = metaObject->property(idx);
        if ((!metaProperty.isStored()) && (QString(metaProperty.name()).contains("STATE_"))){
            int type = metaProperty.userType();
            if (type == qMetaTypeId<bool>()){
                QVariant val=false;
                metaProperty.write(O,val);
            } else {
                QVariant val=0;
                metaProperty.write(O,val);
            }
        }
    }
}

QMetaProperty getMetaProperty(QObject *O, QString propertyName)
{
    if (O) {
        const QMetaObject *   metaO = O->metaObject();
        int ind=metaO->indexOfProperty(qPrintable(propertyName));
        if (ind>=0){
            return metaO->property(ind);
        }
    }
    return {};
}

QStringList  listPropertyes(QObject *obj)
{
    const QMetaObject* metaObject = obj->metaObject();
    QStringList properties;
    for(int i = 0; i < metaObject->propertyCount(); i++)
        properties << QString::fromLatin1(metaObject->property(i).name());
    return properties;
}

QString fullObjectName(QObject *O)
{
    QString N=O->objectName();
    QObject *P=O->parent();
    while (P){
        N=P->objectName()+"/"+N;
        auto *G=qobject_cast<GroupBaseObjects *>(P);
        if ((G)&&(!G->XMLfile().isEmpty())) break;
        P=P->parent();
    }
    return N;
}


void deleteObjectList(QObjectList &l)
{
    foreach (QObject *o, l) {
        delete o;
    }
    l.clear();
}
