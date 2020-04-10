#include "modelgroupgorka.h"
#include "m_base.h"
#include <QDebug>
#include <QMetaObject>
#include <QMetaProperty>

#include "m_rc_gor_park.h"
#include "m_svet.h"
#include "m_otceps.h"
#include "m_rtds.h"
#include "m_controllerars.h"
#include "m_ris.h"
#include "baseobjecttools.h"
#include "mvp_system.h"



#include "mvp_objectfactory.h"
REGISTERELEMENT(ModelGroupGorka,"Модель горки","MODEL ГОРКА")

ModelGroupGorka::ModelGroupGorka(BaseObject *parent) :
    ModelRootGroup(parent)
{
     setObjectName("Модель горки");
     FSIGNAL_ROSPUSK.clear();
     FSIGNAL_PAUSA.clear();
     FSIGNAL_STOP.clear();
     resetStates();
}

ModelGroupGorka::~ModelGroupGorka()
{
}

void ModelGroupGorka::resetStates()
{
    FSTATE_REGIM=regimUnknow;
}

void ModelGroupGorka::validation(ListObjStr *l) const
{
    ModelRootGroup::validation(l);

    if (FSIGNAL_ROSPUSK.isEmpty()) l->error(this,"Сигнал не задан","РОСПУСК","SIGNAL_ROSPUSK");
    if (FSIGNAL_PAUSA.isEmpty()) l->error(this,"Сигнал не задан","ПАУЗА","SIGNAL_PAUSA");
    if (FSIGNAL_STOP.isEmpty()) l->error(this,"Сигнал не задан","СТОП","SIGNAL_STOP");


    QList <m_RC_Gor_Park*> lrcp=findChildren<m_RC_Gor_Park*>();
    for (int i=0;i<lrcp.size();i++)
        for (int j=1;j<lrcp.size();j++){
            if (i==j) continue;
            if (lrcp[i]->PARK_WAY()==lrcp[j]->PARK_WAY())
                l->error(lrcp[i],QString("2 пути парка с одним номером %1").arg(lrcp[i]->PARK_WAY()));
            if (lrcp[i]->MINWAY()==lrcp[j]->MINWAY())
                l->error(lrcp[i],QString("2 пути парка с одним системным номером %1").arg(lrcp[i]->MINWAY()));
        }
    QList <m_Svet*> lsv=findChildren<m_Svet*>();
    QList <m_RC*> lrc=findChildren<m_RC*>();
    foreach (m_Svet *sv, lsv) {
        bool ex=false;
        foreach (m_RC *rc, lrc) {
            if (rc->getSV_M(0,0)==sv){ex=true;break;}
            if (rc->getSV_M(0,1)==sv){ex=true;break;}
            if (rc->getSV_M(1,0)==sv){ex=true;break;}
            if (rc->getSV_M(1,1)==sv){ex=true;break;}
        }
        if (!ex)
            l->warning(sv,QString("Не используемый светофор"));
    }

    QList <m_Base*> lmb=findChildren<m_Base*>();
    foreach (m_Base*m1, lmb) {
        if (!m1->isStoredXML()) continue;
        bool ex=false;
        foreach (m_Base*m2, lmb) {
            if (m1==m2) continue;
            if (isB1hasLinkToB2(m2,m1)){ex=true;break;}
            if (isB1hasLinkToB2(m1,m2)){ex=true;break;}

        }
        if (!ex)
            l->warning(m1,QString("Модель никому не нужна"));
    }
    QList <m_ControllerARS*> lars=findChildren<m_ControllerARS*>();
    foreach (m_ControllerARS*m1, lars) {
        foreach (m_ControllerARS*m2, lars) {
            if (m1==m2) continue;
            if ((m1->ADDR()==m2->ADDR())&&
                (m1->ADDR_SLOT()==m2->ADDR_SLOT()))
                    l->error(m1,QString("Одинаковый ADDR-SLOT"));
        }
    }

    QList <m_RIS*> lris=findChildren<m_RIS*>();
    foreach (m_RIS*m1, lris) {
        foreach (m_RIS*m2, lris) {
            if (m1==m2) continue;
            if ((m1->controllerARS()==m2->controllerARS()))
                    l->error(m1,QString("Одинаковый контроллер"));
        }
    }



}



void ModelGroupGorka::updateAfterLoad()
{
    ModelRootGroup::updateAfterLoad();

    //Otceps->set_lrc(lrc);
    //Otceps->registerChanels();
}

void ModelGroupGorka::updateStates()
{
    updateRegim();
    ModelRootGroup::updateStates();
}

void ModelGroupGorka::updateRegim()
{
    RegimRospusk r=regimUnknow;
    if (FSIGNAL_ROSPUSK.value_1bit()==1) r=regimRospusk;
    if (FSIGNAL_PAUSA.value_1bit()==1)   r=regimPausa;
    if (FSIGNAL_STOP.value_1bit()==1)    r=regimStop;
    if (r!=FSTATE_REGIM){
        int r1=(int)FSTATE_REGIM;
        int r2=(int)r;
        emit regimChanged(r1,r2);
        setSTATE_REGIM(r);
    }
}






