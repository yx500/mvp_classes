#include "m_vagon.h"
#include "m_otcep.h"
#include "m_otceps.h"

m_Vagon::m_Vagon(): m_Base(nullptr)
{
    FNUM=0;
    resetStates();
}

m_Vagon::m_Vagon(const m_Vagon &v): m_Base(nullptr)
{
    assign(&v);FNUM=NUM();
}

m_Vagon::m_Vagon(m_Otceps *parent, int num): m_Base(parent)
{
    FNUM=num;
    otceps=qobject_cast<m_Otceps*>(parent);

}


void m_Vagon::resetStates()
{

    FSTATE_ENABLED=false;
    FSTATE_LOCATION=m_Otcep::locationUnknow;
    FSTATE_ZKR_PROGRESS=0;
    FSTATE_IS_CURRENT=0;

    FSTATE_TICK=0;

    FSTATE_ID_ROSPUSK=0;
    FSTATE_NUM_OTCEP=0;
    FSTATE_N_IN_OTCEP=0;
    FSTATE_IV=0;
    FSTATE_SP=0;

    FSTATE_NUMV=0;
    FSTATE_MASSG=0;
    FSTATE_MASSV=0;
    FSTATE_LN=0;
    FSTATE_OSY=0;

    FSTATE_ROD=0;
    FSTATE_DB=0;
    FSTATE_KTP=0;
    FSTATE_NG=0;
    FSTATE_OSO=0;

    FSTATE_UR=0;
}

void m_Vagon::updateAfterLoad()
{
    m_Base::updateAfterLoad();
    QString packetName=QString("vag%1").arg(FNUM);
    FSIGNAL_DATA=SignalDescription(15,packetName,0);
    FSIGNAL_DATA.acceptGtBuffer();
    FSIGNAL_DATA.getBuffer()->setSizeData(sizeof(tSlVagon));
    if (!FSIGNAL_DATA.isInnerUse())
        connect(FSIGNAL_DATA.getBuffer(),&GtBuffer::bufferChanged,this,&m_Vagon::slotChanged);

}

void m_Vagon::resetTrackingStates()
{
    if (FSTATE_LOCATION!=m_Otcep::locationOnPrib){
        FSTATE_LOCATION=m_Otcep::locationUnknow;
    }
    FSTATE_ZKR_PROGRESS=0;
}

void m_Vagon::resetZKRStates()
{
    FSTATE_ZKR_PROGRESS=0;
}



tSlVagon m_Vagon::toSlVagon() const
{
    tSlVagon slv;
    slv._tick=FSTATE_TICK;

    slv.Id=FSTATE_ID_ROSPUSK;
    slv.NO=FSTATE_NUM_OTCEP;

    slv.IV=FSTATE_IV;
    slv.SP=FSTATE_SP;

    slv.NumV=FSTATE_NUMV;
    slv.MassG=FSTATE_MASSG;
    slv.MassV=FSTATE_MASSV;
    slv.Ln=FSTATE_LN;
    slv.Osy=FSTATE_OSY;
//

    slv.Rod=FSTATE_ROD;
    slv.aDb=FSTATE_DB;
    slv.aNg=FSTATE_NG;
    slv.ktp=FSTATE_KTP;
    slv.OSO=FSTATE_OSO;

    slv.Ur=FSTATE_UR;
    slv.Vnadv=0;

    slv.STATE_LOCATION=FSTATE_LOCATION;
    slv.STATE_N_IN_OTCEP=FSTATE_N_IN_OTCEP;
    slv.STATE_ZKR_PROGRESS=FSTATE_ZKR_PROGRESS;
    slv.STATE_IS_CURRENT=FSTATE_IS_CURRENT;

    if (FSTATE_ENABLED==0) slv.NO=0;

    return slv;
}

void m_Vagon::fromSlVagon(tSlVagon &slv)
{

    FSTATE_TICK=slv._tick;

    FSTATE_ID_ROSPUSK=slv.Id;
    FSTATE_NUM_OTCEP=slv.NO;

    FSTATE_IV=slv.IV;
    FSTATE_SP=slv.SP;

    FSTATE_NUMV=slv.NumV;
    FSTATE_MASSG=slv.MassG;
    FSTATE_MASSV=slv.MassV;
    FSTATE_LN=slv.Ln;
    FSTATE_OSY=slv.Osy;
//

    FSTATE_ROD=slv.Rod;
    FSTATE_DB=slv.aDb;
    FSTATE_NG=slv.aNg;
    FSTATE_KTP=slv.ktp;
    FSTATE_OSO=slv.OSO;

    FSTATE_UR=slv.Ur;
    FSTATE_LOCATION=slv.STATE_LOCATION;
    FSTATE_N_IN_OTCEP=slv.STATE_N_IN_OTCEP;
    FSTATE_ZKR_PROGRESS=slv.STATE_ZKR_PROGRESS;
    FSTATE_IS_CURRENT=slv.STATE_IS_CURRENT;
    if (FSTATE_NUM_OTCEP==0) FSTATE_ENABLED=0; else FSTATE_ENABLED=1;
}

void m_Vagon::assign(const m_Vagon *v)
{
    FSTATE_ENABLED=v->STATE_ENABLED();


    FSTATE_TICK=v->STATE_TICK();

    FSTATE_ID_ROSPUSK=v->STATE_ID_ROSPUSK();
    FSTATE_NUM_OTCEP=v->STATE_NUM_OTCEP();

    FSTATE_IV=v->STATE_IV();
    FSTATE_SP=v->STATE_SP();

    FSTATE_NUMV=v->STATE_NUMV();
    FSTATE_MASSG=v->STATE_MASSG();
    FSTATE_MASSV=v->STATE_MASSV();
    FSTATE_LN=v->STATE_LN();
    FSTATE_OSY=v->STATE_OSY();

    FSTATE_ROD=v->STATE_ROD();
    FSTATE_DB=v->STATE_DB();
    FSTATE_KTP=v->STATE_KTP();
    FSTATE_NG=v->STATE_NG();

    FSTATE_UR=v->STATE_UR();

    FSTATE_LOCATION=v->STATE_LOCATION();
    FSTATE_N_IN_OTCEP=v->STATE_N_IN_OTCEP();
    FSTATE_ZKR_PROGRESS=v->STATE_ZKR_PROGRESS();
    FSTATE_IS_CURRENT=v->STATE_IS_CURRENT();
}

bool m_Vagon::is33()
{
    if (m_Base::is33()) return true;
    return FSIGNAL_DATA.is33();
}

void m_Vagon::updateStates()
{
    if (FSIGNAL_DATA.isInnerUse()) return;
    const tSlVagon *v=(const tSlVagon *)FSIGNAL_DATA.value_data(sizeof(tSlVagon));
    if (v==nullptr){
        setSTATE_ENABLED(false);
        return ;
    }
    auto sz=sizeof(tSlVagon);
    if ((unsigned int)FSIGNAL_DATA.getBuffer()->sizeData()<sz){
        sz=(unsigned int)FSIGNAL_DATA.getBuffer()->sizeData();
    }
;
    bool b_new=false;
    if (memcmp(&stored_SlVagon,v,sz)!=0){
        memcpy(&stored_SlVagon,v,sz);
        b_new=true;
    }
    if (b_new){
        fromSlVagon(stored_SlVagon);
        if (otceps!=nullptr) otceps->vagons2Otceps();
        doStateChanged();

    }
}


QVariantHash tSlVagon2Map(const tSlVagon &v)
{
    QVariantHash m;
    m["ID_ROSPUSK"]=v.Id;
    m["NUM_OTCEP"]=v.NO;
    m["IV"]=v.IV;
    m["SP"]=v.SP;
    m["NUMV"]=v.NumV;
    m["MASSG"]=v.MassG;
    m["MASSV"]=v.MassV;
    m["LN"]=v.Ln;
    m["ROD"]=v.Rod;
    m["DB"]=v.aDb;
    m["NG"]=v.aNg;
    m["KTP"]=v.ktp;
    m["OSO"]=v.OSO;
    m["UR"]=v.Ur;
//    m["Vnadv"]=v.Vnadv;
    return m;

}

//tSlVagon Map2tSlVagon(const QVariantHash &m)
//{
//    tSlVagon v;
//    memset(&v,0,sizeof(v));
//    v.Id=m["Id"].toInt();
//    v.NO=m["NO"].toInt();
//    v.IV=m["IV"].toInt();
//    v.NumV=m["NumV"].toInt();
//    v.MassG=m["MassG"].toInt();
//    v.MassV=m["MassV"].toInt();
//    v.Ln=m["Ln"].toInt();
//    v.Rod=m["Rod"].toInt();
//    v.aDb=m["aDb"].toInt();
//    v.aNg=m["aNg"].toInt();
//    v.ktp=m["ktp"].toInt();
//    v.OSO=m["OSO"].toInt();
//    v.Ur=m["Ur"].toInt();
//    v.Vnadv=m["Vnadv"].toInt();
//    return v;
//}
