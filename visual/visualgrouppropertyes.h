#ifndef VISUALGROUPPROPERTYES_H
#define VISUALGROUPPROPERTYES_H

/*
 * контейнер групповых параметров
 */

#include <QMap>

#include "baseobject.h"
#include "visualgroupproperty.h"

class v_Base;

class VisualGroupPropertyes : public BaseObject
{
    Q_OBJECT
public:
    Q_INVOKABLE VisualGroupPropertyes(QObject *parent = nullptr);
    virtual ~VisualGroupPropertyes(){}
    VisualGroupProperty * visualGroupProperty(QString propertyName);
//    qreal viewProperty_qreal(const VisualGroupProperty_qreal &prop, qreal elementValue);
//    int viewProperty_int(const VisualGroupProperty_int &prop, int elementValue);
    VisualGroupProperty *add(QString objectPropName,QString groupPropName,QObject *O);

    virtual void updateAfterLoad();
    void setTarget(v_Base *p);

signals:
    void visualGroupPropertyesChanged(QObject *O);
public slots:
    void slot_propertyChanged(QObject *O);
protected:
    QMap<QString,VisualGroupProperty *> mName2Prop;
    v_Base *target;
    void updateGroupPropertyes();

};

#endif // VISUALGROUPPROPERTYES_H
