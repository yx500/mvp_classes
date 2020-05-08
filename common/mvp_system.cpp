#include "mvp_system.h"

#include "qobject2xml.h"
#include "baseobject.h"
#include "objectlink.h"
#include "signaldescription.h"
#include <QMetaObject>
#include <QMetaProperty>
#include <QPointF>
#include <QSizeF>
#include <QRectF>

MVP_System MVP;

MVP_System::MVP_System() : QObject(nullptr)
{
    // регестрим свои базовые типы
    qRegisterMetaType<ObjectLink>("ObjectLink");
    QMetaType::registerConverter<ObjectLink,QString> (&ObjectLink::_toString);
    QMetaType::registerConverter<QString,ObjectLink> (&ObjectLink::_fromString);

    qRegisterMetaType<SignalDescription>("SignalDescription");
    QMetaType::registerConverter<SignalDescription,QString> (&SignalDescription::_toString);
    QMetaType::registerConverter<QString,SignalDescription> (&SignalDescription::_fromString);

    // к сожалению в qt почемуто не добали конвертацию в стр всех типоа варианта
//    QMetaType::unregisterType(qMetaTypeId<QVariantHash>() ) ;
//    qRegisterMetaType<QVariantHash>("QVariantHash");
//    QMetaType::registerConverter<QVariantHash,QString> (&QVariantHashToQString);
//    QMetaType::registerConverter<QString,QVariantHash> (&QStringToQVariantHash);

//    qRegisterMetaType<QPointF>("QPointF");
//    QMetaType::registerConverter<QPointF,QString> (&QMetaTypeRegistrator::_QPointFfromString);
//    QMetaType::registerConverter<QString,QPointF> (&QMetaTypeRegistrator::_QPointFtoString);
}




MVP_System::~MVP_System()
{
}

bool MVP_System::QVariantFromQString(QVariant &v,const QString &s)
{
    QVariant vs=s;
    if (vs.canConvert(v.userType())){
        vs.convert(v.userType());
        v=vs;
        return true;
    }

    if(v.type()==QVariant::Hash){
        v=QStringToQVariantHash(s);
        return true;
    } else
    if(v.type()==QVariant::PointF){
        QPointF p=QPointF(s.section(',',0,0).toDouble(),s.section(',',1,1).toDouble());
        v=p;
        return true;
    } else
    if(v.type()==QVariant::SizeF){
            QSizeF p=QSizeF(s.section(',',0,0).toDouble(),s.section(',',1,1).toDouble());
            v=p;
            return true;
    }else
    if(v.type()==QVariant::RectF){
                QRectF p=QRectF(s.section(',',0,0).toDouble(),s.section(',',1,1).toDouble(),s.section(',',2,2).toDouble(),s.section(',',3,3).toDouble());
                v=p;
                return true;
        }


    return false;
}
bool MVP_System::QVariantToQString(const QVariant &v, QString &s)
{
    QVariant vs=v;
    if (vs.canConvert(QMetaType::QString)){
        vs.convert(QMetaType::QString);
        s=vs.toString();
        return true;
    }



    if(v.type()==QVariant::Hash){
        s=QVariantHashToQString(v.toHash());
        return true;
    } else
    if(v.type()==QVariant::PointF){
        s=QString("%1,%2").arg(v.toPointF().x()).arg(v.toPointF().y());
        return true;
    } else
    if(v.type()==QVariant::SizeF){
        s=QString("%1,%2").arg(v.toSizeF().width()).arg(v.toSizeF().height());
        return true;
    }
    if(v.type()==QVariant::RectF){

        s=QString("%1,%2,%3,%4").arg(v.toRectF().left())
                                .arg(v.toRectF().top())
                                .arg(v.toRectF().width())
                                .arg(v.toRectF().height());
        return true;
    }

    return false;
}
static QString _qtype_str="_qtype:";
bool MVP_System::QVariantToQString(int metaTypeId, const QVariant &v, QString &s)
{
    if (!QVariantToQString(v,s)) return false;
    s=QString("%1%2%3").arg(metaTypeId).arg(_qtype_str).arg(s);
    return true;
}

bool MVP_System::QVariantFromQString(int &metaTypeId, QVariant &v, const QString &s)
{
    metaTypeId=0;
    int p=s.indexOf("_qtype:");
    if (p<0) return false;

    QString s1=s.left(p);
    QString s2=s.mid(p+_qtype_str.length());
    metaTypeId=s1.toInt();
    if (metaTypeId==0) return false;
    v=s2;
    if (v.convert(metaTypeId)) return true;
    return QVariantFromQString(v,s2);
}
QString MVP_System::QVariantHashToQString(const QVariantHash &h)
{
    QString s;
    foreach (QString k, h.keys()) {
        if (!s.isEmpty()) s=s+";";
        QVariant v=h[k];
        if (v.type()==QVariant::String)
            s=s+QString("%1=%2").arg(k).arg(v.toString()); else
        {
            QString sv;
            if (QVariantToQString(v.userType(),v,sv))
                s=s+QString("%1=%2").arg(k).arg(sv);
           }

    }
    return s;
}

QVariantHash MVP_System::QStringToQVariantHash(const QString &s)
{
    QVariantHash h;
    QStringList sl=s.split(";");
    foreach (QString s1, sl) {
        QStringList sl1=s1.split("=");
        if (sl1.size()==2){
            if(s.indexOf("_qtype:")>1){
                QVariant v;
                int metaTypeId;
                if (QVariantFromQString(metaTypeId,v,s)){
                    h[sl1[0]]=v;
                } else h[sl1[0]]=sl1[1];
            }
            h[sl1[0]]=sl1[1];
        }
    }
    return h;
}

QObject *MVP_System::loadObject(QString fileName,QStringList *errorLog)
{
    if (mFileName2QObject.contains(fileName)){
        return mFileName2QObject[fileName];
    }
    QObject2XML XML;
    QObject *O=XML.readFile(fileName);
    if (O){
        mFileName2QObject[fileName]=O;
        mQObject2FileName[O]=fileName;
        BaseObject *B=qobject_cast<BaseObject *>(O);
        if (B) B->updateAfterLoad();
    }
    if (errorLog!=nullptr) *errorLog=XML.errorLog;
    return O;
}

bool MVP_System::isObjectLoaded(QString fileName) const
{
    return mFileName2QObject.contains(fileName);
}

QString MVP_System::fileName4Object(QObject *O) const
{
    if (mQObject2FileName.contains(O)) return mQObject2FileName[O];
    return QString();
}

IGtBufferInterface *MVP_System::setGetGtBufferInterface(IGtBufferInterface *i)
{
    return setDefaultGetGtBufferInterface(i);
}


