#include "mvp_objectfactory.h"

#include <QDebug>
#include <QMetaType>
#include <QVariant>

#include "objectlink.h"
#include "signaldescription.h"
#include "qobject2xml.h"



#include <utility>



QObject *ClassRegData::newQObject() const
{
    QObject *O = o->newInstance();
    return O;
}



MVP_ObjectFactory::MVP_ObjectFactory()
{


    qRegisterMetaType<QObjectList>("QObjectList");


}

void MVP_ObjectFactory::addClassRegData(const QString& classname, QString rusname, QString group, const QMetaObject *o)
{
    ClassRegData d;
    d.classname=classname;
    d.rusname=std::move(rusname);
    d.group=group;
    d.o=o;
    if (getClassRegData(classname)!=nullptr){
        qDebug() <<"MVP_ObjectFactory::addClassRegData повторная регистрация " << classname;
    } else {
        vClassRegData.push_back(d);
    }
}

void MVP_ObjectFactory::addElementRegData(QString classname, QString rusname, QString group, const QMetaObject *o)
{
    ClassRegData d;
    d.classname=classname;
    d.rusname=rusname;
    d.group=group;
    d.o=o;
    if (getClassRegData(classname)!=nullptr){
        qDebug() <<"MVP_ObjectFactory::addClassRegData повторная регистрация " << classname;
    } else {
        vClassRegData.push_back(d);
        vElementsRegData.push_back(d);
    }
}

void MVP_ObjectFactory::addPropertyDescriptionData(QString classname, QString propertyname, QString ruspropertyname, QString toolTip, QString group)
{
    PropertyDescriptionData d;
    d.classname=classname;
    d.propertyname=propertyname;
    d.ruspropertyname=ruspropertyname;
    d.toolTip=toolTip;
    d.group=group;
    d.singleStep=0;
    vPropertyDescriptionData.push_back(d);
}

void MVP_ObjectFactory::addPropertyDescriptionData(QString classname, QString propertyname, QString ruspropertyname, QString toolTip, QString group, double singleStep, int decimalsAttribute)
{
    PropertyDescriptionData d;
    d.classname=classname;
    d.propertyname=propertyname;
    d.ruspropertyname=ruspropertyname;
    d.toolTip=toolTip;
    d.group=group;
    d.singleStep=singleStep;
    d.decimalsAttribute=decimalsAttribute;
    vPropertyDescriptionData.push_back(d);
}

const ClassRegData *MVP_ObjectFactory::getClassRegData(QString classname)
{
    for (int i=0;i<vClassRegData.size();i++)
        if (vClassRegData.at(i).classname==classname)
            return &vClassRegData.at(i);
    return nullptr;
}

const ClassRegData *MVP_ObjectFactory::getClassRegData(const QObject *O)
{
    for (int i=0;i<vClassRegData.size();i++)
        if (vClassRegData.at(i).o==O->metaObject())
            return &vClassRegData.at(i);
    return nullptr;
}

QObject *MVP_ObjectFactory::newQObject(QString classname)
{
    if (classname=="QObject"){
        return new QObject();
    }
    const ClassRegData *d=getClassRegData(classname);
    if (d!=nullptr){
        const QMetaObject *meta = d->o;
        QObject *o = meta->newInstance();
        return o;
    }
    int id = QMetaType::type(qPrintable(classname));
    if (id != QMetaType::UnknownType) {
        void *myClassPtr = QMetaType::create(id);
        return (QObject*)myClassPtr ;
    }
    qDebug() <<"MVP_ObjectFactory::newQObject тип не зарегестрирован " << classname;
    return nullptr;

}

QVector<ClassRegData> MVP_ObjectFactory::getVectorOfElementsRegData()
{
    return vClassRegData;
}








bool MVP_ObjectFactory::isClassExists(QString classname)
{
    const ClassRegData *d=getClassRegData(classname);
    if (d!=nullptr) return true;
    return false;
}

QVector<PropertyDescriptionData> *MVP_ObjectFactory::getvPropertyDescriptionData()
{
    return &vPropertyDescriptionData;
}

QString MVP_ObjectFactory::rusClassName(const QObject *O)
{
    const ClassRegData *R=getClassRegData(O);
    if (R==nullptr) return className(O);
    return  R->rusname;
}

QString MVP_ObjectFactory::className(const QObject *O)
{
    return O->metaObject()->className();
}

QString MVP_ObjectFactory::group(QObject *O)
{
    const ClassRegData *R=getClassRegData(O);
    if (R==nullptr) return "---";
    return  R->group;
}

void MVP_ObjectFactory::check()
{
        for (int i=0;i<vClassRegData.size();i++){
            QObject *O=newQObject(vClassRegData[i].classname);
            if (O==nullptr) {
                qDebug() << " Не могу создать " <<  vClassRegData[i].classname;
                continue;
            } else {
                delete O;
            }
        }


}

QStringList MVP_ObjectFactory::supportedList(QString S)
{
    QStringList l;
    for (int i=0;i<vClassRegData.size();i++){
        const QMetaObject *mo=vClassRegData[i].o;
        while (mo!=nullptr){
            if (mo->className()==S) {
                    l.push_back(vClassRegData[i].o->className());
                    break;
            }
            mo=mo->superClass();
        }
    }
    return l;
}

QString MVP_ObjectFactory::property_rusname(const QMetaObject *metaObject, const char *propName)
{
    const PropertyDescriptionData *d=propertyDescriptionData(metaObject,propName);
    if (d) return d->ruspropertyname;
    return propName;
}

QString MVP_ObjectFactory::property_toolTip(const QMetaObject *metaObject, const char *propName)
{
    const PropertyDescriptionData *d=propertyDescriptionData(metaObject,propName);
    if (d) return d->toolTip;
    return "";
}

QString MVP_ObjectFactory::property_group(const QMetaObject *metaObject, const char *propName)
{
    const PropertyDescriptionData *d=propertyDescriptionData(metaObject,propName);
    if (d) return d->group;
    return "";
}

double MVP_ObjectFactory::property_singleStep(const QMetaObject *metaObject, const char *propName)
{
    const PropertyDescriptionData *d=propertyDescriptionData(metaObject,propName);
    if (d) return d->singleStep;
    return 0;
}

int MVP_ObjectFactory::property_decimalsAttribute(const QMetaObject *metaObject, const char *propName)
{
    const PropertyDescriptionData *d=propertyDescriptionData(metaObject,propName);
    if (d) return d->decimalsAttribute;
    return 0;
}

const PropertyDescriptionData *MVP_ObjectFactory::propertyDescriptionData(const QMetaObject *metaObject, const char *propName) const
{
    QString propertyname=QString(propName);
    while (metaObject){
        QString classname=metaObject->className();
        for (int i=0;i<vPropertyDescriptionData.size();i++){
            const PropertyDescriptionData *d=&vPropertyDescriptionData.at(i);
            if ((d->classname==classname) && (d->propertyname==propertyname)) return d;

        }
        metaObject=metaObject->superClass();
    }
    return nullptr;
}

