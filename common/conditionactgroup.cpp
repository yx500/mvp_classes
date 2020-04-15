#include "conditionactgroup.h"
#include "baseobjecttools.h"
#include <QDebug>

#include "mvp_objectfactory.h"
REGISTERELEMENT(ConditionActGroup,"Скрипт Группа","БАЗОВЫЕ")

void ConditionActGroup::setLNKG(const ObjectLink& p)
{
    if (FLNKG!=p){
        FLNKG=p;
        if (FLNKG.id()!=0){
            BaseObject *B=superFindObjectById(this,FLNKG.id());
            if (B) FLNKG.linkObj(B);
            G=qobject_cast<BaseObject*>(B);
            if (G){
                updateConditionActItems();
            }
        }
    }

}

ConditionActGroup::ConditionActGroup(QObject *parent) : BaseObject(parent)
{
    G=nullptr;
    _updateStatesEntered=false;
}

void ConditionActGroup::updateAfterLoad()
{
    if (FLNKG.id()!=0){
        BaseObject *B=superFindObjectById(this,FLNKG.id());
        FLNKG.linkObj(B);
        G=qobject_cast<BaseObject*>(B);
        if (!G){
            qCritical() << "Ошибочная ссылка LNKG ! " << objectName();
        } else {
            updateConditionActItems();
        }
    }
}

bool ConditionActGroup::isCanAddObject(QObject *O) const
{
    return qobject_cast<ConditionActItem*>(O) != nullptr;
}

void ConditionActGroup::addObject(QObject *O)
{
    auto*item=qobject_cast<ConditionActItem*>(O);
    if (item) {
        item->setParent(this);
        updateConditionActItems();
    }
}
void ConditionActGroup::updateConditionActItems()
{
    QList<ConditionActItem*> l=findChildren<ConditionActItem*>();
    foreach (ConditionActItem*item,l) {
        item->G=G;
        item->updatePropetyes();
        connect(item,SIGNAL(stateChanged(QObject*)),this,SLOT(updateStates()));
    }
}

void ConditionActGroup::add2map(TActStep *actStep)
{
    QString ID=actStep->ID_STR_A+actStep->STATE_A;
    if (mID2AS.contains(ID)){
        TActStepSum &actStepSum=mID2AS[ID];
        actStepSum.V=actStep->V;
    } else {
        TActStepSum actStepSum;
        actStepSum.B_A=actStep->B_A;
        actStepSum.STATE_A=actStep->STATE_A;
        actStepSum.V=actStep->V;
        mID2AS[ID]=actStepSum;
    }
}


void ConditionActGroup::updateStates()
{
    // так как изменеия пропертей в актионах спровоцирут опять сигнал к нам
    if (_updateStatesEntered) return;
    _updateStatesEntered=true;
// все на откуп STATE
//    mID2AS.clear();
//    QList<ConditionActItem*> l=findChildren<ConditionActItem*>();
//    foreach (ConditionActItem*item,l) {
//        foreach (TActStep *actStep, item->vACT_RESET) {
//            if (actStep->ok){
//                actStep->updateV();
//                add2map(actStep);
//            }
//        }
//        if (item->STATE_CONDITION()==1){
//            foreach (TActStep *actStep, item->vACT) {
//                if (actStep->ok){
//                    actStep->updateV();
//                    add2map(actStep);
//                }
//            }
//        } else {
//            foreach (TActStep *actStep, item->vACT_ELSE) {
//                if (actStep->ok){
//                    actStep->updateV();
//                    add2map(actStep);
//                }
//            }
//        }
//    }
//    QMap<QString,TActStepSum>::const_iterator i;
//    for (i = mID2AS.constBegin(); i != mID2AS.constEnd(); ++i){
//        const TActStepSum &actStepSum=i.value();
//        if (actStepSum.B_A)
//            actStepSum.B_A->setProperty(qPrintable(actStepSum.STATE_A),actStepSum.V);
//    }
    _updateStatesEntered=false;
}

