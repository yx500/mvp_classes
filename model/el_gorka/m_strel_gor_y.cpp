#include "m_strel_gor_y.h"

#include "baseobjecttools.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(m_Strel_Gor_Y,"Горочная стрелка c ТУ","MODEL ГОРКА")
REGISTERPROPERTY(m_Strel_Gor_Y,SIGNAL_PRP,"СИГНАЛ ТУ В ПЛЮС","Сигнал ТУ плюсового положения","")
REGISTERPROPERTY(m_Strel_Gor_Y,SIGNAL_PRM,"СИГНАЛ ТУ В МИНУС","Сигнал ТУ минусового положения","")
REGISTERPROPERTY(m_Strel_Gor_Y,SIGNAL_A,"СИГНАЛ АВТОМАТ","Сигнал рукояти в среднем положении","")

m_Strel_Gor_Y::m_Strel_Gor_Y(QObject *parent) : m_Strel_Gor(parent)
{
    FSIGNAL_PRP.clear();FSIGNAL_PRM.clear();FSIGNAL_A.clear();
    FSIGNAL_UVK_PRP.clear();FSIGNAL_UVK_PRM.clear();FSIGNAL_UVK_AV.clear();
    FTU_PRP.clear();FTU_PRM.clear();FNEGAB_RC_CNT_P=0;FNEGAB_RC_CNT_M=0;
    FIPD.clear();
    FRTDS.clear();

    FNEGAB_VGRAN_P=0;
    FNEGAB_VGRAN_M=0;

    ipd=nullptr;
    rtds=nullptr;
    resetStates();
}

void m_Strel_Gor_Y::validation(ListObjStr *l) const
{
    m_Strel_Gor::validation(l);
    if ((!FSIGNAL_PRP.isNotUse()) &&(FSIGNAL_PRP.isEmpty()))
        l->warning(this,"Не задан сигнал ПРП");
    if ((!FSIGNAL_PRM.isNotUse()) &&(FSIGNAL_PRM.isEmpty()))
        l->warning(this,"Не задан сигнал ПРМ");
    if ((!FSIGNAL_A.isNotUse()) &&(FSIGNAL_A.isEmpty()))
        l->warning(this,"Не задан сигнал А");
    if ((!FTU_PRP.isNotUse()) &&(FTU_PRP.isEmpty()))
        l->warning(this,"Не задан код ТУ ПРП");
    if ((!FTU_PRM.isNotUse()) &&(FTU_PRM.isEmpty()))
        l->warning(this,"Не задан код ТУ ПРМ");
    if ((!FTU_PRP.isNotUse()) &&(FTU_PRP==FTU_PRM))
        l->error(this,"Одинаковый код ТУ ПРМ");
    if ((!FIPD.isNotUse())&&(FIPD.isNull()))
        l->error(this,"Не задан ИПД");
    if ((!FRTDS.isNotUse())&&(FRTDS.isNull()))
        l->error(this,"Не задан РТДС");
}

void m_Strel_Gor_Y::updateAfterLoad()
{
    m_Strel_Gor::updateAfterLoad();
    ipd=qobject_cast<m_IPD*>(updateLink(FIPD));
    if (!FIPD.isNotUse()){
        if (ipd==nullptr)
            qCritical() << objectName() << "Ошибочная ссылка IPD" <<endl ; else
            addDevice(ipd);
    }
    rtds=qobject_cast<m_RTDS*>(updateLink(FRTDS));
    if (!FRTDS.isNotUse()){
        if (rtds==nullptr)
            qCritical() << objectName() << "Ошибочная ссылка RTDS" <<endl ; else
            addDevice(rtds);
    }
    m_RC *_rc=this;
    l_ngb_rc[0].clear();
    for (int i=0;i<NEGAB_RC_CNT_P();i++){
        _rc=_rc->getNextRC(0,0);
        if (_rc!=nullptr) {
           l_ngb_rc[0].push_back(_rc) ;
        } else {
            break;
        }
    }
    _rc=this;
    l_ngb_rc[1].clear();
    for (int i=0;i<NEGAB_RC_CNT_M();i++){
        _rc=_rc->getNextRC(0,1);
        if (_rc!=nullptr) {
           l_ngb_rc[1].push_back(_rc) ;
        } else {
            break;
        }
    }





}

void m_Strel_Gor_Y::resetStates()
{
    m_Strel_Gor::resetStates();
    FSTATE_PRP=false;
    FSTATE_PRM=false;
    FSTATE_A=false;
    FSTATE_NEGAB_RC=false;
    FSTATE_UVK_PRP=false;
    FSTATE_UVK_PRM=false;
    FSTATE_UVK_AV=false;

    FSTATE_UVK_TLG=false;
    FSTATE_UVK_NGBSTAT_PL=false;
    FSTATE_UVK_NGBSTAT_MN=false;
    FSTATE_UVK_NGBDYN_PL=false;
    FSTATE_UVK_NGBDYN_MN=false;

    FSTATE_UVK_WSTRA=false;
    FSTATE_UVK_WSTRB=false;

}


void m_Strel_Gor_Y::updateStates()
{
    m_Strel_Gor::updateStates();
    setSTATE_PRP(FSIGNAL_PRP.value_1bit());
    setSTATE_PRM(FSIGNAL_PRM.value_1bit());
    setSTATE_A(FSIGNAL_A.value_1bit());
    bool bNEGAB_RC=false;
    m_RC *rc=this;
    for (int irc=0;irc<FNEGAB_RC_CNT_P;irc++){
        if (!rc) break;
        rc=rc->getNextRC(0,0);
        if ((rc)&&(rc->STATE_BUSY()!=MVP_Enums::TRCBusy::free)) {bNEGAB_RC=true;break;}
    }
    rc=this;
    for (int irc=0;irc<FNEGAB_RC_CNT_M;irc++){
        if (!rc) break;
        rc=rc->getNextRC(0,1);
        if ((rc)&&(rc->STATE_BUSY()!=MVP_Enums::TRCBusy::free)) {bNEGAB_RC=true;break;}
    }
    setSTATE_NEGAB_RC(bNEGAB_RC);
    setSignalState(FSIGNAL_UVK_PRP,FSTATE_UVK_PRP);
    setSignalState(FSIGNAL_UVK_PRM,FSTATE_UVK_PRM);
    setSignalState(FSIGNAL_UVK_AV,FSTATE_UVK_AV);



    setSignalState(FSIGNAL_UVK_TLG,FSTATE_UVK_TLG);
//    setSignalState(FSIGNAL_UVK_NGBSTAT,FSTATE_UVK_NGBSTAT);
    setSignalState(FSIGNAL_UVK_NGBDYN_PL,FSTATE_UVK_NGBDYN_PL);
    setSignalState(FSIGNAL_UVK_NGBDYN_MN,FSTATE_UVK_NGBDYN_MN);

    setSignalState(FSIGNAL_UVK_WSTRA,FSTATE_UVK_WSTRA);
    setSignalState(FSIGNAL_UVK_WSTRA,FSTATE_UVK_WSTRB);

//    qDebug()<< "setSTATE_A(true);";
//    setSTATE_A(true);


}

