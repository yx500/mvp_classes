#ifndef VISUALGROUPPROPERTY_H
#define VISUALGROUPPROPERTY_H
/*
 * класс группового параметра
 * */
#include "baseobject.h"

class VisualGroupProperty : public BaseObject
{
    Q_OBJECT
    Q_PROPERTY(QString STORED_VALUE READ STORED_VALUE WRITE setSTORED_VALUE DESIGNABLE true)
public:
    Q_INVOKABLE VisualGroupProperty(QObject *parent = nullptr);
    virtual ~ VisualGroupProperty(){}

    QString STORED_VALUE() const {return V;}
    void setSTORED_VALUE(QString p);
    bool event(QEvent* ev);


signals:

public slots:
protected:
    QString V;
};

#endif // VISUALGROUPPROPERTY_H
