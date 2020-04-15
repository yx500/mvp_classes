#include "station.h"

#include <QDebug>
#include <QMetaObject>
#include <QMetaProperty>
#include <QTimer>

#include "baseobjecttools.h"
#include "signaldescription.h"
#include "mvp_system.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(Station,"Станция","БАЗОВЫЕ")


Station::Station(QObject *parent) :
    BaseObject(parent)
{
    timerUPDATE_STATES=new QTimer();
    timerUPDATE_STATES->stop();
    connect(timerUPDATE_STATES,SIGNAL(timeout()),this,SLOT(slot_updateStates()));
}

Station::~Station()
{
    delete timerUPDATE_STATES;
}

void Station::updateAfterLoad()
{
    BaseObject::updateAfterLoad();
}

bool Station::isCanAddObject(QObject *O) const
{
    if (qobject_cast<BaseObject *>(O)) return true;
    return false;
}

void Station::slot_updateStates()
{
    updateStates();
}


int Station::UPDATE_STATES_INTERVAL() const
{
    if (!timerUPDATE_STATES->isActive()) return 0;
    return timerUPDATE_STATES->interval();
}

void Station::setUPDATE_STATES_INTERVAL(int p)
{
    if (p==0) {
        timerUPDATE_STATES->stop();
        return;
    }
    if ((!timerUPDATE_STATES->isActive())||(timerUPDATE_STATES->interval()!=p)){
       resetStates();
       timerUPDATE_STATES->start(p);
    }


}

