#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QMetaProperty>
#include <QMetaType>
#include <QDebug>
#include "qobject2xml.h"
#include "unknowobject.h"

#include "mvp_objectfactory.h"
#include "baseobject.h"
#include <QFile>
#include <QFileInfo>

#include "groupbaseobjects.h"
#include "mvp_system.h"





QObject2XML::QObject2XML()
{
    this->OF=MVP_ObjectFactory::instance();
}

QObject2XML::QObject2XML(MVP_ObjectFactory *OF)
{
    this->OF=OF;

}

QString QObject2XML::write(QObject *O)
{
    QString  string;
    QXmlStreamWriter xml(&string);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    _write(O, &xml);
    xml.writeEndDocument();
    return string;

}

void QObject2XML::writeQByteArray(QObject *O, QByteArray *array)
{
    QXmlStreamWriter xml(array);
    xml.setAutoFormatting(true);
    _write(O, &xml);
    xml.writeEndDocument();
}


QString QObject2XML::write_QObject(QObject *O)
{
    QString  string;
    QXmlStreamWriter xml(&string);
    xml.setAutoFormatting(true);
    _write_QObject(O, &xml);
    xml.writeEndDocument();
    return string;
}
QString ObjectXmlFile(QObject*O)
{
    if (!O) return "";
    GroupBaseObjects *B=qobject_cast<GroupBaseObjects *>(O);
    if (B){
        if (!B->XMLfile().isEmpty()) return B->XMLfile();
    }
    return ObjectXmlFile(O->parent());
}

void QObject2XML::_write(QObject *O, QXmlStreamWriter *xml)
{

    const QMetaObject *   metaO = O->metaObject();
    QString tagName;
    BaseObject *B=qobject_cast<BaseObject *>(O);
    if ((B)&& (!B->isStoredXML())) return;
    tagName=metaO->className();
    // незнакомец
    UnknowObject*UO=qobject_cast<UnknowObject*>(B);
    if (UO){
        tagName=UO->storedType;
        xml->writeStartElement(tagName);
        for(int i = 0; i < O->dynamicPropertyNames().size(); i++){
            QString propName = O->dynamicPropertyNames().at(i);
            QString s = O->property(qPrintable(propName)).toString();
            xml->writeAttribute( propName,s);
        }
    } else {
        xml->writeStartElement(tagName);
        for(int i = 0; i < metaO->propertyCount(); i++)
        {
            QMetaProperty prop = metaO->property(i);
            if (!prop.isStored()) continue;
            QString propName = prop.name();
            QVariant value = O->property(qPrintable(propName));

            if (value.isValid()){
                QString s;
                if (prop.isEnumType()){
                    int n=value.toInt();
                    s=QString("%1").arg(n);
                } else
                    if (!OF->QVariantToQString(value,s)){
                        err(QString("QObject2XML::_write не могу сконвертировать тип %1").arg(value.typeName()));
                        s="";
                    } else {

                    }
                xml->writeAttribute( propName,s);
            }
        }
    }
    // дети
    for (int i=0;i<O->children().size();i++){
        GroupBaseObjects *GB=qobject_cast<GroupBaseObjects *>(O->children().at(i));
        // проверяем на внешнее сохранение
        if ((GB) && (!GB->XMLfile().isEmpty())){
            QString fullfn=GB->XMLfile();
            QString path=QFileInfo(fileName).path();
            fullfn=fullfn.replace(path,"");

            tagName="extend";
            xml->writeStartElement(tagName);
            xml->writeAttribute("FILE",fullfn);
            //xml->writeAttribute("Object",QString("%1.%2").arg(metaO->className()).arg(O->objectName()));
            xml->writeEndElement();
            // считаем что он себя сам уже сохранил
        } else {
            _write(O->children().at(i),xml);
        }
    }
    xml->writeEndElement();
}

void QObject2XML::_write_QObject(QObject *O, QXmlStreamWriter *xml)
{
    QString tagName;
    QString className=O->property("className").toString();
    tagName=className;
    xml->writeStartElement(tagName);

    xml->writeAttribute("objectName",O->objectName());
    QList<QByteArray> lprop=O->dynamicPropertyNames() ;
    for(auto & i : lprop)
    {
        QString propName=i;
        QString propVal=O->property(i).toString();
        if (propName=="className") continue;
        xml->writeAttribute( propName,propVal);
    }
    // дети
    for (int i=0;i<O->children().size();i++){
        _write_QObject(O->children().at(i),xml);
    }
    xml->writeEndElement();
}

QObject *QObject2XML::_read(QXmlStreamReader *xml)
{
    QString propName;
    QString propValue;
    QStringRef value;
    QVariant newvalue;

    QString tagName=xml->name().toString();
    QObject *O=nullptr;
    // проверяем на внешнее сохранение
    if (tagName=="extend"){
        for (int i=0;i<xml->attributes().size();i++){
            if (xml->attributes().at(i).name().toString()=="FILE"){
                QString fn=xml->attributes().at(i).value().toString();
                QString fnn1=xml->name().toString();
                QString fnn2=xml->namespaceUri().toString();
                QString fullfn=QFileInfo(fileName).path()+fn;
                if (QFileInfo(fullfn).exists()){
                    // проверяем нет ли в кэше
                    if (MVP.isObjectLoaded(fullfn)){
                        O=MVP.loadObject(fullfn);
                    } else {
                        QObject2XML *XML=new QObject2XML(OF);
                        O=XML->readFile(fullfn);
                        error=error|XML->error;
                        errorLog+=XML->errorLog;
                        if (O){
                            GroupBaseObjects *GB=qobject_cast<GroupBaseObjects *>(O);
                            if (GB) GB->setXMLfile(fullfn);
                        }
                        delete XML;
                    }
                } else {
                    err(QString("_read  %1 файл не найден %2").arg(tagName).arg(fullfn));
                    qCritical() <<"QObject2XML::_read  " << tagName << "файл не найден " << fullfn;
                }
            }
        }
        // дети
        while (xml->readNextStartElement()) {
            QObject *child=_read(xml);
            if (child!=nullptr)
                child->setParent(O); //else return nullptr;
        }
    } else {
        O=OF->newQObject(tagName);
        if (O!=nullptr){
            const QMetaObject *   metaO = O->metaObject();
            for (int i=0;i<xml->attributes().size();i++){
                propName=xml->attributes().at(i).name().toString();
                value=xml->attributes().at(i).value();
                int ind=metaO->indexOfProperty(qPrintable(propName));
                if (ind<0){
                    err(QString("_read тип  %1 не имеет свойства %2").arg(tagName).arg(propName));
                    qCritical() <<"QObject2XML::_read тип " << tagName << "не имеет свойства" << propName;
                }else {
                    QMetaProperty prop=metaO->property(ind);
                    if (prop.isEnumType()){
                        newvalue=value.toInt();
                        prop.write(O, newvalue);
                    } else {
                        newvalue.clear();
                        newvalue = O->property(qPrintable(propName));
                        propValue=value.toString();
                        if (!OF->QVariantFromQString(newvalue,propValue)){
                            err(QString("_read не могу сконвертировать тип  %1 из %2 для %3").arg(newvalue.typeName()).arg(propValue).arg(propName));
                            qCritical() <<"QObject2XML::_read не могу сконвертировать тип " << qPrintable(newvalue.typeName()) << " из " <<propValue << "для " << propName;
                        } else {
                            QVariant vv=O->property(qPrintable(propName));
                            //if (!O->setProperty(qPrintable(propName),newvalue)){
                            if ((prop.isWritable())&&(!prop.write(O,newvalue))){
                                err(QString("_read не могу записать свойство  %1 из %2 для %3").arg(newvalue.typeName()).arg(propValue).arg(propName));
                                qCritical() <<"QObject2XML::_read не могу записать свойство " << qPrintable(newvalue.typeName()) << " из " <<propValue << "для " << propName;
                            }
                            vv = O->property(qPrintable(propName));
                            O->setProperty(qPrintable(propName),newvalue);
                            vv = O->property(qPrintable(propName));
                        }
                    }
                }
            }

        } else {
            err(QString("_read тип не создан %1").arg(tagName));
            qCritical() <<"QObject2XML::_read тип не создан " << tagName;
            UnknowObject *UO=new UnknowObject();
            O=UO;
            UO->storedType=tagName;
            for (int i=0;i<xml->attributes().size();i++){
                propName=xml->attributes().at(i).name().toString();
                QString s=xml->attributes().at(i).value().toString();
                O->setProperty(qPrintable(propName),s);
            }
        }
        // дети
        while (xml->readNextStartElement()) {
            QObject *child=_read(xml);
            if (child!=nullptr)
                child->setParent(O); //else return nullptr;
        }
    }
    return O;

}

bool QObject2XML::_go2class(QXmlStreamReader *xml, QString className)
{
    QString tagName=xml->name().toString();
        if (tagName==className){
            return true;
    }
    // дети
    while (xml->readNextStartElement()) {
        if (_go2class(xml,className))
            return true;
    }
    return false;
}

QList<GroupBaseObjects*> QObject2XML::extendedObjects(QObject *O)
{
    QList<GroupBaseObjects*> m;
    QList<GroupBaseObjects*> l=O->findChildren<GroupBaseObjects*>();
    foreach (GroupBaseObjects*B, l) {
        if (!B->isStoredXML()) continue;
        if (B->XMLfile().isEmpty()) continue;
        m.push_back(B);
    }
    return m;
}



QObject *QObject2XML::read(QString string)
{
    QXmlStreamReader xml(string);
    if (xml.readNextStartElement()) {
        return _read(&xml);
    }
    if (xml.hasError()) {
        err(QString("xml.hasError %1 %2 %3 %4").arg(xml.errorString()).arg(xml.lineNumber()).arg(xml.columnNumber()).arg(xml.characterOffset()));
        qDebug()<<xml.errorString() << xml.lineNumber() << xml.columnNumber() << xml.characterOffset();
    }
    return nullptr;
}

QObject *QObject2XML::readFile(QString fn)
{
    error=false;
    fileName=fn;
    QObject *O=nullptr;
    QFile *file=new QFile(fn);
    file->open(QIODevice::ReadOnly);
    QXmlStreamReader xml(file);
    if (xml.readNextStartElement()) {
        O=_read(&xml);
        GroupBaseObjects *B=qobject_cast<GroupBaseObjects *>(O);
        if (B) B->setXMLfile(fn);
    }
    file->close();
    delete file;
    return O;
}

QObject *QObject2XML::readFile(QString fn, QString className)
{
    error=false;
    fileName=fn;
    QObject *O=nullptr;
    QFile *file=new QFile(fn);
    file->open(QIODevice::ReadOnly);
    QXmlStreamReader xml(file);
    if (xml.readNextStartElement()) {
        if (_go2class(&xml,className)){
            O=_read(&xml);
        }
    }
    file->close();
    delete file;
    return O;
}

void QObject2XML::writeFile(QObject *O, QString fn)
{
    error=false;
    fileName=fn;
    QFile *file=new QFile(fn);
    file->open(QIODevice::WriteOnly);
    QXmlStreamWriter xml(file);
    xml.setAutoFormatting(true);
    _write(O, &xml);
    xml.writeEndDocument();
    file->close();
    delete file;
}


bool QObject2XML::isChanged(GroupBaseObjects *O,QString fullfn)
{
    GroupBaseObjects *B=qobject_cast<GroupBaseObjects *>(O);
    if (!B) return false;
    if (!B->isStoredXML()) return false;
    if (B->XMLfile().isEmpty()) return true;
    if (!QFileInfo(fullfn).exists()) return true;
    QFile file(B->XMLfile());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    QByteArray array1=file.readAll();
    QByteArray array2;
    fileName=fullfn;
    writeQByteArray(O,&array2);
    if (array1.size()!=array2.size()) return true;
    long r=memcmp(array1.constData() ,array2.constData(),array1.size());
    if (r!=0) {
//        for (long i=0;i<array1.size();i++){
//            if (array1.at(i)!=array2.at(i)){
//                r=i;
//                break;
//            }
//        }
        return true;
    }
    return false;
}




void QObject2XML::err(QString st)
{
    error=true;
    errorLog.push_back(st);
}



