#ifndef MVP_SYSTEM_H
#define MVP_SYSTEM_H

/*
 * корневой интерфейс MVP
 * */

#include "mvp_objectfactory.h"


class IGtBufferInterface;

class LIBSHARED_EXPORT MVP_System : public QObject
{
    Q_OBJECT
public:
    explicit MVP_System();
    ~MVP_System();
    static bool QVariantToQString(const QVariant &v,QString &s);
    static bool QVariantFromQString(QVariant &v,const QString &s);
    static bool QVariantToQString(int metaTypeId,const QVariant &v,QString &s);
    static bool QVariantFromQString(int &metaTypeId,QVariant &v,const QString &s);
    static QString QVariantHashToQString(const QVariantHash &h);
    static QVariantHash QStringToQVariantHash(const QString &s);

    QObject *loadObject(QString fileName, QStringList *errorLog=nullptr);
    bool isObjectLoaded(QString fileName) const;
    QString fileName4Object(QObject *O) const;
    MVP_ObjectFactory *OFM()const {return MVP_ObjectFactory::instance();}
    IGtBufferInterface *setGetGtBufferInterface(IGtBufferInterface *i);

signals:

public slots:
protected:
    QMap<QString,QObject*>mFileName2QObject;
    QMap<QObject*,QString>mQObject2FileName;


};

extern LIBSHARED_EXPORT MVP_System MVP;


#endif // MVP_SYSTEM_H
