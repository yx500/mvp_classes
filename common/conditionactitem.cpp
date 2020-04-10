#include "conditionactitem.h"
#include "baseobjecttools.h"
#include "mvp_system.h"
#include "mvp_objectfactory.h"
#include "signalsmanager.h"
REGISTERELEMENT(ConditionActItem,"Скрипт Условие","БАЗОВЫЕ")

TActStep::TActStep()
{
    clear();
}

void TActStep::clear()
{
    ID_STR_A.clear();
    STATE_A.clear();
    V.clear();
    ID_STR_V.clear();
    STATE_V.clear();
    B_A=nullptr;
    B_V=nullptr;
    ok=false;
}

void TActStep::fromString(const QString& S, QObject *G, QStringList &slerrors)
{
    // формат действий:
    // IDSTR.STATE=IDSTR2.STATE2
    // IDSTR.STATE=V;
    clear();

    QStringList sl=S.split('=');
    if (sl.size()!=2) {
        slerrors.push_back(QString("ошибка в выражении %1").arg(S));
        return;
    }
    QStringList sl_A=sl[0].split('.');
    if (sl_A.size()!=2){
        slerrors.push_back(QString("ошибка в выражении %1 %2").arg(S).arg(sl[0]));
        return;
    }
    ID_STR_A=sl_A[0];STATE_A=sl_A[1];
    QStringList sl_V=sl[1].split('.');
    if (sl_V.size()>2){
        slerrors.push_back(QString("ошибка в выражении %1 %2").arg(S).arg(sl[1]));
        return;
    }
    if (sl_V.size()==1){
        V=sl_V[0];
    }
    if (sl_V.size()==2){
        ID_STR_A=sl_V[0];STATE_A=sl_V[1];
    }
    QList<BaseObject *> l;
    // цепляем объекты
    if (G){
        l=findObjectByIdstr(G, ID_STR_A);
        if (l.empty()){
            slerrors.push_back(QString("объект не найден %1 %2").arg(S).arg(ID_STR_A));
            return;
        }
        if (l.size()>1){
            slerrors.push_back(QString("%3 объеков %1 %2").arg(S).arg(ID_STR_A).arg(l.size()));
            return;
        }
        B_A=l.first();
        QVariant value = B_A->property(qPrintable(STATE_A));
        if (!value.isValid()){
            slerrors.push_back(QString("нет свойства %1 %2.%3").arg(S).arg(ID_STR_A).arg(STATE_A));
            return;
        }
        if (!ID_STR_V.isEmpty()){
            l=findObjectByIdstr(G, ID_STR_V);
            if (l.empty()){
                slerrors.push_back(QString("объект не найден %1 %2").arg(S).arg(ID_STR_V));
                return;
            }
            if (l.size()>1){
                slerrors.push_back(QString("%3 объектов %1 %2").arg(S).arg(ID_STR_V).arg(l.size()));
                return;
            }
            B_V=l.first();
            QVariant value = B_V->property(qPrintable(STATE_V));
            if (!value.isValid()){
                slerrors.push_back(QString("нет свойства %1 %2.%3").arg(S).arg(ID_STR_V).arg(STATE_V));
                return;
            }
            V.clear();
        }
        ok=true;
    }
}

void TActStep::updateV()
{
    if (!B_V.isNull()){
        V.clear();
        QVariant value = B_V->property(qPrintable(STATE_V));
        if (value.isValid()){
            V=value.toString();
        } else {
            ok=false;
        }
    }
}

//========================================================================

ConditionActItem::ConditionActItem(QObject *parent) : BaseObject(parent)
{
    FACT_RESET.clear();FACT.clear();FACT_ELSE.clear();G=nullptr;
    FCONDITION.clear();FSTATE_CONDITION=0;
    FGROUP_RESET=false;_updateStatesEntered=false;
}

void ConditionActItem::validation(ListObjStr *l) const
{
    foreach (QString S, slerrors) {
        l->error(this,S);
    }
    foreach (QString S, condition.errorLog) {
        l->error(this,S);
    }
}

void ConditionActItem::setACT_RESET(const QString& p)
{
    if (FACT_RESET!=p){
        FACT_RESET=p;
        updatePropetyes();doPropertyChanged();
    }
}

void ConditionActItem::setCONDITION(const QString& p)
{
    if (FCONDITION!=p){
        FCONDITION=p;
        updatePropetyes();doPropertyChanged();
    }
}

void ConditionActItem::setACT(const QString& p)
{
    if (FACT!=p){
        FACT=p;
        updatePropetyes();doPropertyChanged();
    }
}

void ConditionActItem::setACT_ELSE(const QString& p)
{
    if (FACT_ELSE!=p){
        FACT_ELSE=p;
        updatePropetyes();doPropertyChanged();
    }
}

void ConditionActItem::setSTATE_CONDITION(int p)
{
    if (FSTATE_CONDITION!=p){
        FSTATE_CONDITION=p;
        doStateChanged();
    }
}

void ConditionActItem::clear()
{
    QList<BaseObject *>l;
    foreach (QObject *O, condition.usedObjects()) {
        if (qobject_cast<BaseObject *>(O)) l.push_back(qobject_cast<BaseObject *>(O));
    }
    foreach (BaseObject *B, l) {
        disconnect(B,SIGNAL(propertyChanged(QObject*)),this,SLOT(slot_formulaPropertyChanged(QObject*)));
        disconnect(B,SIGNAL(stateChanged(QObject*)),this,SLOT(slot_formulaPropertyChanged(QObject*)));
    }
    condition.clear();
    foreach (TActStep * actStep, vACT_RESET) {
        delete actStep;
    }
    foreach (TActStep * actStep, vACT) {
        delete actStep;
    }
    foreach (TActStep * actStep, vACT_ELSE) {
        delete actStep;
    }

    vACT_RESET.clear();
    vACT.clear();
    vACT_ELSE.clear();
    slerrors.clear();
    condition.errorLog.clear();
}


void ConditionActItem::updatePropetyes()
{
    clear();
    condition.fromString(FCONDITION);
    condition.elementsAccept(&SM,nullptr,G);
    vACT_RESET=VfromS(FACT_RESET);
    vACT=VfromS(FACT);
    vACT_ELSE=VfromS(FACT_ELSE);

    // реакция
    QList<BaseObject *>l;
    foreach (QObject *O, condition.usedObjects()) {
        if (qobject_cast<BaseObject *>(O)) l.push_back(qobject_cast<BaseObject *>(O));
    }
    foreach (BaseObject *B, l) {
        connect(B,SIGNAL(propertyChanged(QObject*)),this,SLOT(slot_formulaPropertyChanged(QObject*)));
        connect(B,SIGNAL(stateChanged(QObject*)),this,SLOT(slot_formulaPropertyChanged(QObject*)));
    }


}

QVector<TActStep *> ConditionActItem::VfromS(QString S)
{
    QVector<TActStep *>v;
    S=S.simplified();
    S.replace("(","");
    S.replace(")","");
    S.replace("&",";");
    S.replace("|",";");
    QStringList sl;
    sl=S.split(';');

    foreach (QString S, sl) {
        if (!S.isEmpty()){
            auto *actStep=new TActStep();
            actStep->clear();
            actStep->fromString(S,G,slerrors);
            v.push_back(actStep);
        }
    }
    return v;
}

bool ConditionActItem::isOk_V(QVector<TActStep *> &V)
{
    foreach (TActStep * actStep, V) {
        if (!actStep->ok) return false;
    }
    return true;
}

void ConditionActItem::slot_formulaPropertyChanged(QObject *)
{
    // так как изменеия пропертей в актионах спровоцирут опять сигнал к нам
    if (_updateStatesEntered) return;
    _updateStatesEntered=true;
    int r=condition.result();
    if (FSTATE_CONDITION!=r){
        FSTATE_CONDITION=r;
        doStateChanged();
    }
    if (FSTATE_CONDITION==1){
        foreach (TActStep *actStep, vACT) {
            if (actStep->ok){
                actStep->updateV();
                if (actStep->B_A) actStep->B_A->setProperty(qPrintable(actStep->STATE_A),actStep->V);
            }
        }
    } else {
        foreach (TActStep *actStep, vACT_ELSE) {
            if (actStep->ok){
                actStep->updateV();
                if (actStep->B_A) actStep->B_A->setProperty(qPrintable(actStep->STATE_A),actStep->V);
            }
        }
    }
    doStateChanged();

    _updateStatesEntered=false;
}
