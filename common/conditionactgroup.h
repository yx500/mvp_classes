#ifndef CONDITIONACTGROUP_H
#define CONDITIONACTGROUP_H

/*
 * Класс -контейнер скриптов
 *
*/

#include "baseobject.h"
#include "conditionactitem.h"
#include "objectlink.h"

struct TActStepSum{
    BaseObject *B_A;
    QString STATE_A;
    QString V;
};

class ConditionActGroup : public BaseObject
{
    Q_OBJECT
    Q_PROPERTY(ObjectLink   LNKG READ LNKG WRITE setLNKG DESIGNABLE true)
public:
    ObjectLink LNKG() const {return FLNKG;}
    void setLNKG(const ObjectLink& p);
public:
    Q_INVOKABLE ConditionActGroup(QObject *parent = nullptr);
    virtual ~ConditionActGroup(){}
    virtual void updateAfterLoad();
    virtual bool isCanAddObject(QObject *O) const;
    virtual void addObject(QObject *O);
    void updateConditionActItems();
    virtual void updateStates(); // основной жизненный цикл

signals:

public slots:

protected:
    BaseObject *G;
    ObjectLink FLNKG;
    void add2map(TActStep *actStep);
    QMap<QString,TActStepSum> mID2AS;
    bool _updateStatesEntered;

};

#endif // CONDITIONACTGROUP_H
