#ifndef BASEOBJECTTOOLS_H
#define BASEOBJECTTOOLS_H

#include <QObject>
#include <QMetaObject>
#include <QMetaProperty>
#include "baseobject.h"


class Station;
class ObjectLink;

const QObject *superParent(const QObject *O);
bool isPropertyExists(QObject *O, const QString &propertyName);
void setPropertyValue(QObject *O, const QString &propertyName,const QVariant &v);
BaseObject *findObjectById(const QObject *O, const quint64 &id);
QList<BaseObject *> findObjectByName(QObject *O, const QString &name);
QList<BaseObject *> findObjectByIdstr(QObject *O, const QString &idstr);
BaseObject *superFindObjectById(QObject *O, const quint64 &id);
QList<BaseObject *> listAllBaseObjects(QObject *O);
bool isInherited(const QMetaObject *o,const QString& className);
bool isChildrenExists(QObject *parent,QObject *child);
QObject *firstParentInheritedFromClass(QObject *child,const QString& className);

int objectCountLinked2Object(const BaseObject *linkedB, const QObject *_superParent=nullptr) ;
QList<BaseObject *> listAllObjectsLinked2B2(const BaseObject *B2, const QObject *_superParent=nullptr);
void unlinkAllObjects2B2(const BaseObject * B2,const QObject *_superParent=nullptr);
void unlinkAllObjects2B2(const QObject * B2,const QObject *_superParent=nullptr);
QStringList linkedInfo(const BaseObject * B2, const QObject *_superParent=nullptr);
void replaceLinks(BaseObject *B, quint64 oldId, const quint64 &newId);
QObjectList linkedObjects(const QObject *O);

//QList<BaseObject *> listLinkedObjects(BaseObject *B2, QObject *superParent=0);
Station * stationParent(QObject *O);
Station * superStationParent(QObject *O);
void updateLinksNum(BaseObject * B);  // обновляет числовое значение ид,нужно если объект на который ссылаются перегенерил id
void updateLinksObjects(BaseObject * B,BaseObject * SParent);
bool isB1hasLinkToB2(const BaseObject *B1,const BaseObject *B2);
QStringList listPropertyesB1hasLinkToB2(const BaseObject *B1,const BaseObject *B2,const QString& prefix);
void unlinkB1fromB2(BaseObject *B1,const BaseObject *B2);
void deleteBaseObject(BaseObject * B,QObject *_superParent=nullptr);
void deleteObjectList(QObjectList &l);


QObject *findIDSTRObject(QObject *O,QString &IDSTR);
void resetBaseObjectStates(QObject *O);

QMetaProperty getMetaProperty(QObject *O,QString propertyName);
QStringList  listPropertyes(QObject *obj);
QString fullObjectName(QObject *O);


#endif // BASEOBJECTTOOLS_H
