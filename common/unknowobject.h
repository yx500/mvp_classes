#ifndef UNKNOWOBJECT_H
#define UNKNOWOBJECT_H

/*
 * объект - заглушка
 * */

#include "baseobject.h"

class UnknowObject : public BaseObject
{
    Q_OBJECT
public:
    explicit UnknowObject(QObject *parent = nullptr): BaseObject(parent){}
    QString storedType;

signals:

public slots:
};

#endif // UNKNOWOBJECT_H
