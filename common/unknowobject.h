#ifndef UNKNOWOBJECT_H
#define UNKNOWOBJECT_H

/*
 * объект - заглушка
 * */

#include "groupbaseobjects.h"

class UnknowObject : public GroupBaseObjects
{
    Q_OBJECT
public:
    explicit UnknowObject(QObject *parent = nullptr): GroupBaseObjects(parent){}
    QString storedType;

signals:

public slots:
};

#endif // UNKNOWOBJECT_H
