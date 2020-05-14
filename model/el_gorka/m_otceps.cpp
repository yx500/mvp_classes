#include "m_otceps.h"
#include "mvp_system.h"
#include "m_controllerars.h"
#include "m_rc_gor_park.h"
#include "modelgroupgorka.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(m_Otceps,"ОТЦЕПЫ","MODEL ГОРКА")

m_Otceps::m_Otceps(QObject *parent) : m_Base(parent)
{
    FTYPE_DESCR=0;
    setObjectName("ОТЦЕПЫ");
    for (int i=0;i<MAXCOUNT_OTCEPS;i++){
        l_otceps.push_back(new m_Otcep(this,i+1));
    }
    memset(&vagons,0,sizeof (vagons));
}

m_Otceps::~m_Otceps()
{
    l_otceps.clear();

}

void m_Otceps::resetStates()
{
    m_Base::resetStates();
    memset(&vagons,0,sizeof (vagons));
}

void m_Otceps::validation(ListObjStr *l) const
{
    m_Base::validation(l);

    foreach (m_RC *rc1, l_rc) {
        foreach (m_RC *rc2, l_rc) {
            if (rc1==rc2) continue;
            if ((!rc1->SIGNAL_BUSY().isNotUse()) &&
                    (!rc2->SIGNAL_BUSY().isNotUse()) &&
                    (rc1->SIGNAL_BUSY().chanelOffset()==rc2->SIGNAL_BUSY().chanelOffset())){
                l->error(this,"РЦ с одинаковым сигналом занятия",QString("%1 %2").arg(rc1->objectName(),rc2->objectName()));
            }
            if (rc1->idstr()==rc2->idstr()) {
                l->error(this,"РЦ с одинаковым IDS",QString("%1 %2").arg(rc1->objectName(),rc2->objectName()));
            }
        }
        m_RC_Gor_Park*rcp=qobject_cast<m_RC_Gor_Park*>(rc1);
        if ((rcp) && (rcp->PARK_WAY()<=0)) l->error(this,"РЦ без номера пути парка",QString("%1").arg(rc1->objectName()));
        if ((rcp) && (rcp->MINWAY()<=0)) l->error(this,"РЦ без номера маршрута",QString("%1").arg(rc1->objectName()));
        if ((rcp) && (rcp->MAXWAY()<=0)) l->error(this,"РЦ без номера маршрута",QString("%1").arg(rc1->objectName()));
        if ((rcp) && (rcp->MINWAY()!=rcp->MAXWAY())) l->error(this,"РЦ c разным номера маршрута",QString("%1").arg(rc1->objectName()));
    }

}

void m_Otceps::updateAfterLoad()
{
    m_Base::updateAfterLoad();
    modelGroupGorka=qobject_cast<ModelGroupGorka*>(parent());
    foreach (m_Otcep*otcep, l_otceps) {
        if (FTYPE_DESCR==0){
            QString packetName=QString("descr%1").arg(otcep->NUM());
            otcep->setSIGNAL_DATA(SignalDescription(9,packetName,0));
        }
        if (FTYPE_DESCR==1){
            QString packetName=QString("descr%1").arg(otcep->NUM());
            otcep->setSIGNAL_DATA(SignalDescription(109,packetName,0));
        }
        otcep->updateAfterLoad();

    }
    for (int i=0;i<MaxVagon;i++){
        chanelVag[i]=SignalDescription::getBuffer(15,QString("vag%1").arg(i+1));
        connect(chanelVag[i],&GtBuffer::bufferChanged,this,&m_Otceps::updateVagons);
    }
    if (modelGroupGorka){
        l_rc=parent()->findChildren<m_RC*>();
        foreach (m_RC *rc, l_rc) {
            if ((!rc->SIGNAL_BUSY().isNotUse()) &&(rc->SIGNAL_BUSY().chanelOffset()!=0)){
                mOffset2Rc[rc->SIGNAL_BUSY().chanelOffset()]=rc;
                mIDS2Rc[rc->idstr()]=rc;
            }
        }

    }
}

QList<m_Otcep *> m_Otceps::otceps() const
{
    return l_otceps;

}


m_Otcep *m_Otceps::otcepByNum(int n)
{
    foreach (m_Otcep*otcep, l_otceps)
        if (otcep->NUM()==n) return otcep;
    return nullptr;
}

m_Otcep *m_Otceps::otcepOnRc(m_RC *rc)
{
    foreach (m_Otcep*otcep, l_otceps)
        if ((otcep->STATE_LOCATION()!=m_Otcep::locationUnknow)&& (otcep->vBusyRc.indexOf(rc)>=0)) return  otcep;
    return 0;
}

m_RC *m_Otceps::find_RC(int chanelOffset)
{
    if (!mOffset2Rc.contains(chanelOffset)) return nullptr;
    return mOffset2Rc[chanelOffset];
}

QList<m_Otcep *> m_Otceps::enabledOtceps() const
{
    QList<m_Otcep *> l;
    foreach (m_Otcep*otcep, l_otceps)
        if (otcep->STATE_ENABLED()) l.push_back(otcep);
    return  l;
}
QVariantHash tSlVagon2Map(const tSlVagon &v)
{
    QVariantHash m;
    m["Id"]=v.Id;
    m["NO"]=v.NO;
    m["IV"]=v.IV;
    m["NumV"]=v.NumV;
    m["MassG"]=v.MassG;
    m["MassV"]=v.MassV;
    m["Ln"]=v.Ln;
    m["Rod"]=v.Rod;
    m["aDb"]=v.aDb;
    m["aNg"]=v.aNg;
    m["ktp"]=v.ktp;
    m["OSO"]=v.OSO;
    m["Ur"]=v.Ur;
    m["Vnadv"]=v.Vnadv;
    return m;

}

tSlVagon Map2tSlVagon(const QVariantHash &m)
{
    tSlVagon v;
    memset(&v,0,sizeof(v));
    v.Id=m["Id"].toInt();
    v.NO=m["NO"].toInt();
    v.IV=m["IV"].toInt();
    v.NumV=m["NumV"].toInt();
    v.MassG=m["MassG"].toInt();
    v.MassV=m["MassV"].toInt();
    v.Ln=m["Ln"].toInt();
    v.Rod=m["Rod"].toInt();
    v.aDb=m["aDb"].toInt();
    v.aNg=m["aNg"].toInt();
    v.ktp=m["ktp"].toInt();
    v.OSO=m["OSO"].toInt();
    v.Ur=m["Ur"].toInt();
    v.Vnadv=m["Vnadv"].toInt();
    return v;
}

void m_Otceps::updateVagons()
{
    if (disableUpdateStates) return;
    bool changed=false;
    for (int j=0;j<MaxVagon;j++){
        if (FTYPE_DESCR==0){
            tSlVagon * SlVagon=(tSlVagon *)chanelVag[j]->A.data();
            if (memcmp(&vagons[j],SlVagon,sizeof(tSlVagon))!=0){
                vagons[j]=*SlVagon;
                changed=true;
            }
        }
        if (FTYPE_DESCR==1){
            if (_storedVagonsA[j]!=chanelVag[j]->A){
                _storedVagonsA[j]=chanelVag[j]->A;
                changed=true;
                QString S=QString::fromUtf8(chanelVag[j]->A);
                QVariantHash m=MVP_System::QStringToQVariantHash(S);
                tSlVagon SlVagon=Map2tSlVagon(m);
                //            if ((v.IV>0) &&(v.IV<MaxVagon)
                vagons[j]=SlVagon;
            }
        }
    }
    if (!changed) return;
    tSlVagon & SlVagon0=vagons[0];
    auto idrosp=SlVagon0.Id;
    foreach (m_Otcep*otcep, l_otceps) {
        quint32 idr=0;
        int sp=0;
        if (otcep->disableUpdateStates) continue;
        otcep->vVag.clear();
        int lenvag=0;
        for (int j=0;j<MaxVagon;j++){
            tSlVagon & SlVagon=vagons[j];
            if (SlVagon.NO==0) break;
            if (SlVagon.Id!=idrosp)break;
            if (SlVagon.NO<0) continue;
            if (otcep->NUM()==SlVagon.NO){
                idr=SlVagon.Id;
                sp=SlVagon.SP;
                otcep->vVag.push_back(SlVagon);
                int Ln=0;
                if (SlVagon.Ln==0) Ln=15;
                if (SlVagon.Ln>1000) Ln=SlVagon.Ln/1000;
                lenvag+=Ln;
            }
        }
        otcep->setSTATE_ID_ROSP_VAG(idr);
        otcep->setSTATE_SP_VAG(sp);
        otcep->setSTATE_LEN_BY_VAGON(lenvag);
    }
}


m_Otcep *m_Otceps::otcepADDR_SLOT(int ADDR_SLOT,int ADDR,int NTP)
{
    if ((NTP>=1)&&(NTP<=3)){
        foreach (m_Otcep*otcep, l_otceps) {
            if (otcep->STATE_LOCATION()!=m_Otcep::locationUnknow){
                int a=(ADDR_SLOT-1)*100+ADDR;
                if (otcep->stored_Descr.addr_tp[NTP-1]==a)return  otcep;
            }
        }
    }
    return nullptr;
}

m_Otcep *m_Otceps::topOtcep() const
{
    // возвращаем первый в парке отпр
    foreach (auto otcep, l_otceps) {
        if ((otcep->STATE_ENABLED())&&(otcep->STATE_LOCATION()==m_Otcep::locationOnPrib)) return otcep;
    }
    return nullptr;
}

void m_Otceps::updateStates()
{
    if (disableUpdateStates) return;
    //    m_Base::updateStates();
    //    updateVagons();
}
