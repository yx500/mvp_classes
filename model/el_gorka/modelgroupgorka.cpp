#include "modelgroupgorka.h"
#include "m_base.h"
#include <QDebug>
#include <QMetaObject>
#include <QMetaProperty>

#include "m_rc_gor_park.h"
#include "m_rc_gor_zkr.h"
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
    if ((!FSIGNAL_RRC.isNotUse())&&(FSIGNAL_RRC.isEmpty())) l->error(this,"Сигнал не задан","РРС","SIGNAL_RRC");
    if ((!FSIGNAL_RRC_TU.isNotUse())&&(FSIGNAL_RRC_TU.isEmpty())) l->error(this,"Сигнал не задан","РРС ТУ","SIGNAL_RRC_TU");
    if (!FSIGNAL_RRC_TU.isEmpty() && FSIGNAL_RRC.isEmpty()) l->error(this,"Сигнал РРС не задан при  заданном ТУ РРС","РРС ТУ","SIGNAL_RRC_TU");


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

    QList <const m_Base*> lmb=findChildren<const m_Base*>();
    lmb.push_back(this);
    foreach (const m_Base*m1, lmb) {
        if (!m1->isStoredXML()) continue;
        if(QString(m1->metaObject()->className())=="ModelGroup")  continue;
        bool ex=false;
        foreach (const m_Base*m2, lmb) {
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
    auto l_stry=findChildren<m_Strel_Gor_Y*>();
    foreach (auto m1, l_stry) {
        //if (abs(m1->TU_PRP().chanelOffset()-m1->TU_PRM().chanelOffset())
        foreach (auto m2, l_stry){
            if (m1==m2) continue;
            if ((!m1->TU_PRP().isNotUse())&&(m1->TU_PRP()==m2->TU_PRP())) l->error(m1,"Одинаковый код ТУ ПРП c"+m2->idstr());
            if ((!m1->TU_PRP().isNotUse())&&(m1->TU_PRM()==m2->TU_PRP())) l->error(m1,"Одинаковый код ТУ ПРП c"+m2->idstr());
            if ((!m1->TU_PRM().isNotUse())&&(m1->TU_PRP()==m2->TU_PRP())) l->error(m1,"Одинаковый код ТУ ПРМ c"+m2->idstr());
            if ((!m1->TU_PRM().isNotUse())&&(m1->TU_PRM()==m2->TU_PRP())) l->error(m1,"Одинаковый код ТУ ПРМ c"+m2->idstr());
        }
    }

}



void ModelGroupGorka::updateAfterLoad()
{
    ModelRootGroup::updateAfterLoad();
    // расставим значения
    QList<m_RC_Gor_Park*> l_rcp=findChildren<m_RC_Gor_Park*>();
    foreach (m_RC_Gor_Park*rcp, l_rcp) {
        mSP2MAR[rcp->PARK_WAY()]=rcp->MINWAY();
        mMAR2SP[rcp->MINWAY()]=rcp->PARK_WAY();

    }
    lzkr=findChildren<m_RC_Gor_ZKR*>();
    foreach (auto rczkr, lzkr) {

        for (int m=rczkr->MINWAY();m<=rczkr->MAXWAY();m++){
            QList<m_RC_Gor*> lm=marshrut(rczkr->PUT_NADVIG(),m);
            int n=0;
            qreal absx=0;
            foreach (auto rc, lm) {
                rc->m_PN_M2N[rczkr->PUT_NADVIG()][m]=n;
                rc->m_PN_M2X[rczkr->PUT_NADVIG()][m]=absx;
                n++;
                absx+=rc->LEN();
            }
        }
    }
    auto lo=findChildren<m_Otceps*>();
    if (!lo.isEmpty())
        FLNK_OTCEPS.linkObj(lo.first());
}

bool ModelGroupGorka::is33()
{
    return FSIGNAL_ROSPUSK.is33();
}

void ModelGroupGorka::updateStates()
{
    updateRegim();
    int p=0;
    foreach (auto zkr, lzkr) {
//        if (zkr->SIGNAL_ROSPUSK().value_1bit()==1) return zkr->PUT_NADVIG();
        if (zkr->STATE_ROSPUSK()==1) p= zkr->PUT_NADVIG();
    }
    setSTATE_PUT_NADVIG(p);
    setSignalState(FSIGNAL_RRC,FSTATE_RRC);
    ModelRootGroup::updateStates();
}



void ModelGroupGorka::updateRegim()
{
    if  (FSIGNAL_ROSPUSK.isInnerUse())return;
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
bool inway(int way,int minway,int maxway)
{
    if ((way>0)&&(way>=minway)&&(way<=maxway)) return true;
    return false;
}
QList<m_RC_Gor *> ModelGroupGorka::marshrut(int put_nadvig,int m)
{
    QList<m_RC_Gor *> l;
    QList<m_RC_Gor_ZKR*> lzkr=findChildren<m_RC_Gor_ZKR*>();
    m_RC_Gor*rcs=nullptr;
    foreach (auto rczkr, lzkr) {
        if (rczkr->PUT_NADVIG()==put_nadvig) rcs=rczkr;


    }
    l.push_back(rcs);
    while (rcs){
        auto rcplus=qobject_cast<m_RC_Gor*>(rcs->getNextRC(0,0));
        if ((rcplus!=nullptr)&&(inway(m,rcplus->MINWAY(),rcplus->MAXWAY()))) {
            rcs=rcplus;  l.push_back(rcs);
        } else {
            auto rcmnus=qobject_cast<m_RC_Gor*>(rcs->getNextRC(0,1));
            if ((rcmnus!=nullptr)&&(inway(m,rcmnus->MINWAY(),rcmnus->MAXWAY()))) {
                rcs=rcmnus;  l.push_back(rcs);
            } else {
                break;
            }
        }
    }
    return l;
}






