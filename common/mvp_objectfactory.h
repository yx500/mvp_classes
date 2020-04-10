#ifndef MVP_OBJECTFACTORY_H
#define MVP_OBJECTFACTORY_H

/*
 * Фабрика объектов системы MVP
 * */


#include <QObject>
#include <QHash>
#include <QVector>
#include <QVariant>
#include <QDebug>

#include "libshared_global.h"

struct ClassRegData{
    QString classname;
    QString rusname;
    QString group;
    const QMetaObject *o;
    QObject *newQObject() const;
};

struct PropertyDescriptionData{
    QString classname;
    QString propertyname;
    QString ruspropertyname;
    QString toolTip;
    QString group;
    double singleStep;
    int decimalsAttribute;
};


class LIBSHARED_EXPORT MVP_ObjectFactory
{
public:
    MVP_ObjectFactory();
    virtual ~MVP_ObjectFactory(){}
    static MVP_ObjectFactory* instance(){
           static MVP_ObjectFactory one;
           return &one;

    }

    class REGQCLASS{
                 public:
                        REGQCLASS(QString classname,QString rusname,QString group,const QMetaObject*o){
                          MVP_ObjectFactory::instance()->addClassRegData(classname,rusname,group,o);
                        }
    };
    class REGPROPERTY{
                 public:
                        REGPROPERTY(QString classname,QString propertyname,QString ruspropertyname,QString toolTip,QString group){
                          MVP_ObjectFactory::instance()->addPropertyDescriptionData(classname,propertyname, ruspropertyname,toolTip,group);
                        }
    };
    class REGPROPERTY2{
                 public:
                        REGPROPERTY2(QString classname,QString propertyname,QString ruspropertyname,QString toolTip,QString group,double singleStep,int decimalsAttribute){
                          MVP_ObjectFactory::instance()->addPropertyDescriptionData(classname,propertyname, ruspropertyname,toolTip,group,singleStep,decimalsAttribute);
                        }
    };


    virtual bool QVariantToQString(const QVariant &v,QString &s);
    virtual bool QVariantFromQString(QVariant &v,const QString &s);


    void addClassRegData(const QString& classname,QString rusname,QString group,const QMetaObject*o);
    void addElementRegData(QString classname,QString rusname,QString group,const QMetaObject*o);
    void addPropertyDescriptionData(QString classname,QString propertyname,QString ruspropertyname,QString toolTip,QString group);
    void addPropertyDescriptionData(QString classname,QString propertyname,QString ruspropertyname,QString toolTip,QString group,double singleStep,int decimalsAttribute);
    const ClassRegData *getClassRegData(QString classname);
    const ClassRegData *getClassRegData(const QObject *O);
    QObject *newQObject(QString classname);
    QVector <ClassRegData> getVectorOfElementsRegData();

    bool isClassExists(QString classname);
    QVector<PropertyDescriptionData> *getvPropertyDescriptionData();
    QString rusClassName(const QObject *O);
    QString className(const QObject *O);
    QString group(QObject *O);
    void check();
    QStringList supportedList(QString S);
    QString property_rusname(const QMetaObject *metaObject,const char *propName) ;
    QString property_toolTip(const QMetaObject *metaObject,const char *propName) ;
    QString property_group(const QMetaObject *metaObject,const char *propName) ;
    double property_singleStep(const QMetaObject *metaObject,const char *propName) ;
    int property_decimalsAttribute(const QMetaObject *metaObject,const char *propName) ;


protected:
    QVector<ClassRegData> vClassRegData;
    QVector<ClassRegData> vElementsRegData;
    QVector<PropertyDescriptionData> vPropertyDescriptionData;
    const PropertyDescriptionData * propertyDescriptionData(const QMetaObject *metaObject,const char *propName) const;
};

#define REGISTERPROPERTY(classname,propertyname,ruspropertyname,toolTip,group) static MVP_ObjectFactory::REGPROPERTY regprop_##classname##propertyname(#classname,#propertyname,ruspropertyname,toolTip,group);
#define REGISTERPROPERTY2(classname,propertyname,ruspropertyname,toolTip,group,singleStep,decimalsAttribute) MVP_ObjectFactory::REGPROPERTY2 regprop_##classname##propertyname(#classname,#propertyname,ruspropertyname,toolTip,group,singleStep,decimalsAttribute);

#define REGISTERELEMENT(classname,rusname,group) static MVP_ObjectFactory::REGQCLASS reggi_##classname(#classname,rusname, group, &classname::staticMetaObject);

#endif // MVP_OBJECTFACTORY_H
