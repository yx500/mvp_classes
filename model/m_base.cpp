#include "m_base.h"
#include "modelrootgroup.h"



void m_Base::setSTATE_33(bool p)
{
    if (!FSTATE_33 && p)
        resetStates();
    if (FSTATE_33!=p){
        FSTATE_33=p;
         doStateChanged();
    }
}

bool m_Base::setSignalState(const SignalDescription &s,bool &state)
{
    if (s.isInnerUse()) return state;
    bool newstate=0;
    if (s.isUsed()) newstate=s.value_1bit();
    if (state!=newstate){
        state=newstate;
        doStateChanged();
    }
    return state;
}



m_Base::m_Base(QObject *parent) :
    BaseObject(parent)
{
    FEXTFLAGSRC.clear();
    FSTATE_33=false;
    resetStates();
}

void m_Base::validation(ListObjStr *l) const
{
    validationEmptySignals(l);
    validationEmptyLinks(l);
}

void m_Base::resetStates()
{
    BaseObject::resetStates();
}

QObject *m_Base::modelRootGroup(m_Base *m)
{
    QObject *mm=m;
    while (mm!=nullptr){
        ModelRootGroup *mrg=qobject_cast<ModelRootGroup *>(mm);
        if (mrg) return mrg;
        mm=mm->parent();
    }
    return nullptr;
}

void m_Base::updateStates()
{
    BaseObject::updateStates();
    setSTATE_33(is33());
}

