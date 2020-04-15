#ifndef QOBJECT2XML_H
#define QOBJECT2XML_H

/*
 * чтение/запись XML
 * */

#include <QVariant>
#include "libshared_global.h"


class QXmlStreamWriter;
class QXmlStreamReader;
class MVP_ObjectFactory;
class BaseObject;
class BaseObject;

class LIBSHARED_EXPORT QObject2XML
{
public:
    MVP_ObjectFactory *OF;
    QObject2XML();
    QObject2XML(MVP_ObjectFactory *OF);
    QString write(QObject *O);
    QString write_QObject(QObject *O);
    QObject *read(QString string);
    QObject *readFile(QString fn);
    QObject *readFile(QString fn,QString className);
    void writeFile(QObject *O,QString fn);
    void writeQByteArray(QObject *O, QByteArray *array);

    bool isChanged(BaseObject *O, QString fullfn);
    QList<BaseObject *> extendedObjects(QObject *O);
    bool error;
    QStringList errorLog;



protected:
    QString fileName;
    void      _write(QObject *O,QXmlStreamWriter * xml);
    void      _write_QObject(QObject *O,QXmlStreamWriter * xml);
    QObject *_read(QXmlStreamReader * xml);
    bool    _go2class(QXmlStreamReader * xml,QString className);
    void err(QString st);
signals:

public slots:

};



#endif // QOBJECT2XML_H
