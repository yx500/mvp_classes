#include "m_otcep.h"
#include "m_otceps.h"
#include "gtbuffer.h"
#include "mvp_system.h"
#include "modelgroupgorka.h"



void m_Otcep::setSTATE_LOCATION(const int &p)
{
    if (FSTATE_LOCATION!=p){
        if (FSTATE_ZKR_PROGRESS==1) {
            for(m_Vagon &v:vVag){
                v.setSTATE_LOCATION(FSTATE_LOCATION);
            }
        }
        FSTATE_LOCATION=p;
        FSTATE_TICK++;
        doStateChanged();
    }
}

QString m_Otcep::USER_NUM() const
{
    if (FSTATE_EXTNUM!=0){
        if (FSTATE_EXTNUMPART!=0)  return QString("%1-%2").arg(FSTATE_EXTNUM).arg(FSTATE_EXTNUMPART);
        return QString("%1").arg(FSTATE_EXTNUM);
    }
    return QString("%1").arg(FNUM);
}

int m_Otcep::STATE_SP() const
{
    if (FSTATE_MAR==0) return 0;
    if ((otceps) && (otceps->modelGroupGorka))
        if (otceps->modelGroupGorka->mMAR2SP.contains(FSTATE_MAR)) return otceps->modelGroupGorka->mMAR2SP[FSTATE_MAR];
    return 0;
}

void m_Otcep::setSTATE_SP(int p)
{
    if (otceps->modelGroupGorka->mSP2MAR.contains(p)) setSTATE_MAR(otceps->modelGroupGorka->mSP2MAR[p]);else
        setSTATE_MAR(0);
}
int m_Otcep::STATE_SP_F() const
{
    if (FSTATE_MAR_F==0) return 0;
    if ((otceps) && (otceps->modelGroupGorka))
        if (otceps->modelGroupGorka->mMAR2SP.contains(FSTATE_MAR_F)) return otceps->modelGroupGorka->mMAR2SP[FSTATE_MAR_F];
    return 0;
}

void m_Otcep::setSTATE_SP_F(int p)
{
    if (otceps->modelGroupGorka->mSP2MAR.contains(p)) setSTATE_MAR_F(otceps->modelGroupGorka->mSP2MAR[p]);else
        setSTATE_MAR_F(0);
}

int m_Otcep::STATE_SL_VAGON_CNT() const
{
    return FSTATE_SL_VAGON_CNT;
}

void m_Otcep::setSTATE_SL_VAGON_CNT(int p)
{
    if (FSTATE_SL_VAGON_CNT!=p){
        changeVagonsCnt(p);
        FSTATE_SL_VAGON_CNT=p;
        doStateChanged();
    }
}

void m_Otcep::changeVagonsCnt(int p)
{
    if (p<vVag.size()){
        while (vVag.size()>p) vVag.removeLast();
    } else {
        if (p>vVag.size()){
            while (vVag.size()<p){
                m_Vagon v;
                v.setSTATE_SP(STATE_SP());
                v.setSTATE_NUM_OTCEP(FNUM);
                v.setSTATE_N_IN_OTCEP(vVag.size());
                vVag.push_back(v);
            }
        }
    }
}

void m_Otcep::setSTATE_V_IN_1(const qreal &p){setSTATE_V_INOUT(0,0,p);}
void m_Otcep::setSTATE_V_IN_2(const qreal &p){setSTATE_V_INOUT(0,1,p);}
void m_Otcep::setSTATE_V_IN_3(const qreal &p){setSTATE_V_INOUT(0,2,p);}
void m_Otcep::setSTATE_V_OUT_1(const qreal &p){setSTATE_V_INOUT(1,0,p);}
void m_Otcep::setSTATE_V_OUT_2(const qreal &p){setSTATE_V_INOUT(1,1,p);}
void m_Otcep::setSTATE_V_OUT_3(const qreal &p){setSTATE_V_INOUT(1,2,p);}
const  qreal & m_Otcep::STATE_V_IN_1() const {return STATE_V_INOUT(0,0);}
const  qreal & m_Otcep::STATE_V_IN_2() const {return STATE_V_INOUT(0,1);}
const  qreal & m_Otcep::STATE_V_IN_3() const {return STATE_V_INOUT(0,2);}
const  qreal & m_Otcep::STATE_V_OUT_1() const {return STATE_V_INOUT(1,0);}
const  qreal & m_Otcep::STATE_V_OUT_2() const {return STATE_V_INOUT(1,1);}
const  qreal & m_Otcep::STATE_V_OUT_3() const {return STATE_V_INOUT(1,2);}

void m_Otcep::setSTATE_V_INOUT(int io,int n, qreal p)
{
    if (FSTATE_V_INOUT[io][n]!=p){
        FSTATE_V_INOUT[io][n]=p;
        doStateChanged();
    }
}
void m_Otcep::setSTATE_ADDR_TP_1(const int &p){setSTATE_ADDR_TP(0,p);}
void m_Otcep::setSTATE_ADDR_TP_2(const int &p){setSTATE_ADDR_TP(1,p);}
void m_Otcep::setSTATE_ADDR_TP_3(const int &p){setSTATE_ADDR_TP(2,p);}
const  int & m_Otcep::STATE_ADDR_TP_1() const {return STATE_ADDR_TP(0);}
const  int & m_Otcep::STATE_ADDR_TP_2() const {return STATE_ADDR_TP(1);}
const  int & m_Otcep::STATE_ADDR_TP_3() const {return STATE_ADDR_TP(2);}
void m_Otcep::setSTATE_ADDR_TP(int n, int p)
{
    if (FSTATE_ADDR_TP[n]!=p){
        FSTATE_ADDR_TP[n]=p;
        doStateChanged();
    }
}
void m_Otcep::setSTATE_OT_1(const int &p){setSTATE_OT_RA(0,0,p);}
void m_Otcep::setSTATE_OT_2(const int &p){setSTATE_OT_RA(0,1,p);}
void m_Otcep::setSTATE_OT_3(const int &p){setSTATE_OT_RA(0,2,p);}
void m_Otcep::setSTATE_RA_1(const int &p){setSTATE_OT_RA(1,0,p);}
void m_Otcep::setSTATE_RA_2(const int &p){setSTATE_OT_RA(1,1,p);}
void m_Otcep::setSTATE_RA_3(const int &p){setSTATE_OT_RA(1,2,p);}
const  int & m_Otcep::STATE_OT_1() const {return STATE_OT_RA(0,0);}
const  int & m_Otcep::STATE_OT_2() const {return STATE_OT_RA(0,1);}
const  int & m_Otcep::STATE_OT_3() const {return STATE_OT_RA(0,2);}
const  int & m_Otcep::STATE_RA_1() const {return STATE_OT_RA(1,0);}
const  int & m_Otcep::STATE_RA_2() const {return STATE_OT_RA(1,1);}
const  int & m_Otcep::STATE_RA_3() const {return STATE_OT_RA(1,2);}

void m_Otcep::setSTATE_OT_RA(int ot_ra, int n, int p)
{
    if (FSTATE_OT_RA[ot_ra][n]!=p){
        FSTATE_OT_RA[ot_ra][n]=p;
        doStateChanged();
    }
}
void m_Otcep::setSTATE_V_ZAD_1(const qreal &p){setSTATE_V_ZAD(0,p);}
void m_Otcep::setSTATE_V_ZAD_2(const qreal &p){setSTATE_V_ZAD(1,p);}
void m_Otcep::setSTATE_V_ZAD_3(const qreal &p){setSTATE_V_ZAD(2,p);}
const  qreal & m_Otcep::STATE_V_ZAD_1() const {return STATE_V_ZAD(0);}
const  qreal & m_Otcep::STATE_V_ZAD_2() const {return STATE_V_ZAD(1);}
const  qreal & m_Otcep::STATE_V_ZAD_3() const {return STATE_V_ZAD(2);}
void m_Otcep::setSTATE_V_ZAD(int n, qreal p)
{
    if (FSTATE_V_ZAD[n]!=p){
        FSTATE_V_ZAD[n]=p;
        doStateChanged();
    }
}

int m_Otcep::STATE_VAGON_CNT() const
{
    if (FSTATE_ZKR_VAGON_CNT==0) return FSTATE_SL_VAGON_CNT;
    return FSTATE_ZKR_VAGON_CNT;
}
int m_Otcep::STATE_OSY_CNT() const
{
    if (FSTATE_SL_OSY_CNT==0) return FSTATE_ZKR_OSY_CNT;
    return FSTATE_SL_OSY_CNT;
}

qreal m_Otcep::STATE_VES() const
{
    if (FSTATE_ZKR_VES==0) return FSTATE_SL_VES;
    return FSTATE_ZKR_VES;
}
int m_Otcep::STATE_BAZA() const
{
    if ((FSTATE_SL_BAZA!=0) || (FSTATE_ZKR_BAZA!=0)) return 1;
    return 0;
}
qreal m_Otcep::STATE_LEN() const
{
    if (FSTATE_SL_LEN==0) return STATE_VAGON_CNT()*14;
    return FSTATE_SL_LEN;
}
m_Otcep::m_Otcep(m_Otceps *parent,int num) : m_Base(parent)
{
    otceps=qobject_cast<m_Otceps*>(parent);
    FNUM=num;
    setObjectName(QString("Отцеп %1").arg(FNUM));

    resetStates();
    vVag.reserve(MaxVagon);
    vBusyRc.reserve(32);
}

void m_Otcep::resetStates()
{

    FSTATE_ENABLED=false;
    FSTATE_TICK=0;
    FSTATE_LOCATION=locationUnknow;
    FSTATE_MAR=0;
    FSTATE_MAR_F=0;
    FSTATE_MAR_R=0;
    FSTATE_DIRECTION=0;
    FSTATE_NAGON=0;
    FSTATE_ERROR=0;
    FSTATE_ID_ROSP=0;
    FSTATE_SL_UR=0;
    FSTATE_ERROR_TRACK=0;
    FSTATE_IS_CURRENT=0;

    FSTATE_GAC_ACTIVE=0;
    FSTATE_GAC_W_STRA=0;
    FSTATE_GAC_W_STRB=0;
    FSTATE_ARS_ACTIVE=0;

    FSTATE_SL_VAGON_CNT=0;
    FSTATE_SL_OSY_CNT=0;
    FSTATE_SL_LEN=0;
    FSTATE_SL_VES=0;
    FSTATE_SL_BAZA=0;
    FSTATE_SL_UR=0;
    FSTATE_SL_OSO=0;
    FSTATE_SL_VAGON_CNT_PRED=0;

    FSTATE_ZKR_PROGRESS=0;
    FSTATE_ZKR_S_IN=0;
    FSTATE_ZKR_VAGON_CNT=0;
    FSTATE_ZKR_OSY_CNT=0;
    FSTATE_ZKR_BAZA=0;
    FSTATE_ZKR_VES=0;
    FSTATE_PUT_NADVIG=0;
    FSTATE_ZKR_TLG=0;


    FSTATE_KZP_OS=kzpUnknow;
    FSTATE_KZP_D=0;


    FSTATE_V=_undefV_;
    FSTATE_V_RC=_undefV_;
    FSTATE_V_ARS=_undefV_;
    FSTATE_V_KZP=_undefV_;
    FSTATE_V_DISO=_undefV_;

    FSTATE_D_RCS_XOFFSET=-1;
    FSTATE_D_RCF_XOFFSET=-1;
    FSTATE_D_ORDER_RC=0;

    FSTATE_EXTNUM=0;
    FSTATE_EXTNUMPART=0;


    memset(FSTATE_V_INOUT,0,sizeof(FSTATE_V_INOUT));
    memset(FSTATE_OT_RA,0,sizeof(FSTATE_OT_RA));
    memset(FSTATE_V_ZAD,0,sizeof(FSTATE_V_ZAD));
    memset(FSTATE_ADDR_TP,0,sizeof(FSTATE_ADDR_TP));
    for (int io=0;io<2;io++){
        for (int n=0;n<3;n++) {FSTATE_V_INOUT[io][n]=_undefV_;FSTATE_V_ZAD[n]=_undefV_;}
    }



    FSTATE_TICK=0;
    RCS=nullptr;
    RCF=nullptr;
    vVag.clear();
    vBusyRc.clear();


}

void m_Otcep::resetTrackingStates()
{
    //снимаем только параметры TOS

    if (FSTATE_LOCATION==m_Otcep::locationOnPrib){
         FSTATE_MAR_F=0;
         FSTATE_ERROR=0;
    } else {
        FSTATE_LOCATION=m_Otcep::locationUnknow;
    }

    FSTATE_ZKR_PROGRESS=0;
    FSTATE_ZKR_S_IN=0;
    FSTATE_ERROR_TRACK=0;

    //otcep->setSTATE_GAC_ACTIVE(0);
    FSTATE_GAC_W_STRA=0;
    FSTATE_GAC_W_STRB=0;

    FSTATE_KZP_OS=m_Otcep::kzpUnknow;
    FSTATE_V=_undefV_;
    FSTATE_V_RC=_undefV_;
    FSTATE_V_ARS=_undefV_;
    FSTATE_V_KZP=_undefV_;
    FSTATE_V_DISO=_undefV_;

    RCS=nullptr;
    RCF=nullptr;
    vBusyRc.clear();
    for (m_Vagon &v:vVag){
        v.resetTrackingStates();
    }
}

void m_Otcep::resetZKRStates()
{
    FSTATE_MAR_F=0;
    FSTATE_ZKR_TLG=0;
    FSTATE_ZKR_VES=0;
    FSTATE_ZKR_BAZA=0;
    FSTATE_ZKR_S_IN=0;
    FSTATE_ZKR_OSY_CNT=0;
    FSTATE_ZKR_PROGRESS=0;
    FSTATE_ZKR_VAGON_CNT=0;
    FSTATE_DIRECTION=0;
    FSTATE_ERROR=0;
    memset(FSTATE_ADDR_TP,0,sizeof(FSTATE_ADDR_TP));
    memset(FSTATE_OT_RA,0,sizeof(FSTATE_OT_RA));
    memset(FSTATE_ADDR_TP,0,sizeof(FSTATE_ADDR_TP));
    for (int io=0;io<2;io++){
        for (int n=0;n<3;n++) {FSTATE_V_INOUT[io][n]=_undefV_;FSTATE_V_ZAD[n]=_undefV_;}
    }
    for (m_Vagon &v:vVag){
        v.resetZKRStates();
    }

}

void m_Otcep::acceptSLStates(const m_Otcep *o)
{
    FSTATE_MAR=o->STATE_MAR();
    FSTATE_ID_ROSP=o->STATE_ID_ROSP();

    FSTATE_SL_OSY_CNT=o->STATE_SL_OSY_CNT();
    FSTATE_SL_LEN=o->STATE_SL_LEN();
    FSTATE_SL_VES=o->STATE_SL_VES();
    FSTATE_SL_BAZA=o->STATE_SL_BAZA();
    FSTATE_SL_UR=o->STATE_SL_UR();
    FSTATE_SL_OSO=o->STATE_SL_OSO();
    for (int i=0;i<3;i++) FSTATE_V_ZAD[i]=o->STATE_V_ZAD(i);
    FSTATE_SL_VAGON_CNT_PRED=o->STATE_SL_VAGON_CNT_PRED();
    vVag.clear();
    for (int i=0;i<o->vVag.size();i++){
        auto v=o->vVag[i];
        v.setSTATE_NUM_OTCEP(FNUM);
        vVag.push_back(v);
    }
    setSTATE_SL_VAGON_CNT(o->STATE_SL_VAGON_CNT());

}

void m_Otcep::setVagon(m_Vagon *v)
{
    if ((v->STATE_NUM_OTCEP()==FNUM)/*&&(v->STATE_ID_ROSPUSK()==FSTATE_ID_ROSP)*/){
        if ((v->STATE_N_IN_OTCEP()>0)&&(v->STATE_N_IN_OTCEP()<=vVag.size())){
            vVag[v->STATE_N_IN_OTCEP()-1].assign(v);
        }
    }
}



void m_Otcep::updateAfterLoad()
{
    m_Base::updateAfterLoad();
    QString packetName=QString("descr%1").arg(FNUM);
    FSIGNAL_DATA=SignalDescription(9,packetName,0);
    FSIGNAL_DATA.acceptGtBuffer();
    FSIGNAL_DATA.getBuffer()->setSizeData(sizeof(t_NewDescr));
    if (!FSIGNAL_DATA.isInnerUse())
        connect(FSIGNAL_DATA.getBuffer(),&GtBuffer::bufferChanged,this,&m_Otcep::slotChanged);
}





void m_Otcep::setBusyRC()
{
    vBusyRc.clear();
    m_RC * RC;
    if ((RCS!=nullptr)&&(RCF!=nullptr)){
        RC=RCS;
        while (RC!=nullptr){
            vBusyRc.push_back(RC);
            if (RC==RCF) break;
            RC=RC->getNextRCpolcfb(1);
            if (vBusyRc.size()>40) {
                vBusyRc.clear();
                vBusyRc.push_back(RCS);vBusyRc.push_back(RCF);
                break;
            }
        }
    }else {
        if (RCS!=nullptr) vBusyRc.push_back(RCS);
        if (RCF!=nullptr) vBusyRc.push_back(RCF);
    }
}

bool m_Otcep::is33()
{
    if (m_Base::is33()) return true;
    return FSIGNAL_DATA.is33();
}

void m_Otcep::updateStates()
{
    if (disableUpdateStates) return;
    if (FSIGNAL_DATA.isInnerUse()) return;
    m_Base::updateStates();
    if (FSTATE_33) return ;
    updateStates_0();
    //    if (FSIGNAL_DATA.chanelType()==9) updateStates_0();
    //    if (FSIGNAL_DATA.chanelType()==109) updateStates_1();

}

qreal VD2STATE(uint16 v){
    if (v==0xFFFF) return _undefV_;
    return 1.*v/10.;
}

uint16 STATE2VD(qreal v){
    if (v==_undefV_) return 0xFFFF;
    return v*10.;
}
void m_Otcep::states2descr_ext(t_NewDescr &D) const
{
    memset(&D,0,sizeof(D));
    D.E.sz_struct=sizeof(D.E);
    if (!FSTATE_ENABLED) return;
    D.D.num=FNUM   ; // Номер отцепа 1-255 Живет в течении роспуска одного
    D.D.mar    =FSTATE_MAR; // Резерв
    D.D.mar_f  =FSTATE_MAR_F; // Фактический маршрут(==Заданному либо 0 либо реализов)
    if (RCS!=nullptr) D.D.start  =RCS->SIGNAL_BUSY().chanelOffset()+1; // Местоположение Головы отцепа |    N Эл РЦ в ТС     |
    if (RCF!=nullptr) D.D.end    =RCF->SIGNAL_BUSY().chanelOffset()+1; // Местоположение Хвоста отцепа |номер сигнала занятия|
    D.D.ves    =FSTATE_ZKR_VES; // Вес отцепа в тоннах
    D.D.osy    =FSTATE_ZKR_OSY_CNT; // Длинна ( в осях)
    D.D.len    =FSTATE_ZKR_VAGON_CNT; // Длинна ( в вагонах)
    D.D.baza   =FSTATE_ZKR_BAZA; // Признак длиннобазности
    D.D.nagon  =FSTATE_NAGON; // Признак нагона
    if (FSTATE_LOCATION==m_Otcep::locationUnknow) D.D.end_slg=1; // Признак конца слежения (по последней РЦ на путях)
    D.D.err    =FSTATE_ERROR; // Признак неперевода стрелки
    D.D.dir    =FSTATE_DIRECTION; // Направление
    D.D.V_rc   =STATE2VD(FSTATE_V_RC); // Скорость по РЦ
    D.D.V_zad  =FSTATE_V_ZAD[0]; // Скорость заданная
    D.D.Stupen =FSTATE_SL_STUPEN; // Ступень торможения
    D.D.osy1   =0; // Длинна ( в осях)
    D.D.osy2   =0; // Длинна ( в осях)
    D.D.V_zad2 =FSTATE_V_ZAD[1]; // Скорость заданная 2TP
    D.D.V_zad3 =FSTATE_V_ZAD[2]; // Скорость заданная  3TP
    // D.D.pricel ;
    //D.D.old_num;
    //D.D.old_mar;
    D.D.U_len  =FSTATE_SL_LEN;
    D.D.vagon  =FSTATE_SL_VAGON_CNT;
    D.D.V_out  =STATE2VD(FSTATE_V_INOUT[1][0]);
    D.D.V_in2  =STATE2VD(FSTATE_V_INOUT[0][1]);
    D.D.V_out2 =STATE2VD(FSTATE_V_INOUT[1][1]);;
    D.D.V_in3  =STATE2VD(FSTATE_V_INOUT[0][2]);
    D.D.V_out3 =STATE2VD(FSTATE_V_INOUT[1][2]);
    D.D.Id     =FSTATE_ID_ROSP;
    D.D.st     =FSTATE_STUPEN;
    D.D.ves_sl =FSTATE_SL_VES;
    D.D.r_mar  =FSTATE_MAR_R;
    for (int i=0;i<3;i++) D.D.t_ot[i]=FSTATE_OT_RA[0][i];// 0- растарможка 1-4 ступени максимал ступень работы замедлителя
    for (int i=0;i<3;i++) D.D.r_a[i] =FSTATE_OT_RA[1][i]; // 0-автомат режим ручного вмешательсва
    D.D.V_in   =STATE2VD(FSTATE_V_INOUT[0][0]); // Cкорость входа 1 ТП
    //D.D.Kzp    ; // КЗП по расчету Антона
    //D.D.v_rosp ; // Скорость расформирования   - норма/быстро/медленно - 0/1/2
    //D.D.flag_ves; // Работоспособность весомера - да/нет/ - 0/1
    //D.D.flag_r  ; // Признак ручной установки скорости
    // D.D.FirstVK ;
    //D.D.LastVK  ;

    for (int i=0;i<3;i++) D.D.addr_tp[i]=STATE_ADDR_TP(i); // Занятый замедлитель

    //D.D.v_rosp1 ; // Скорость расформирования   - норма/быстро/медленно - 0/1/2
    //D.D.p_rzp   ; // Признак выше ПТП
    //       D.D.VrospZ;
    //       D.D.VrospF;
    //       D.D.V_zad2_S ; // Скорость заданная 2TP


    D.E.STATE_LOCATION=       FSTATE_LOCATION;
    if (RCS!=nullptr) D.E.STATE_ID_RCS=RCS->id();
    if (RCF!=nullptr) D.E.STATE_ID_RCF=RCF->id();
    D.E.STATE_ERROR_TRACK=       FSTATE_ERROR_TRACK;
    D.E.STATE_IS_CURRENT=        FSTATE_IS_CURRENT;

    D.E.STATE_GAC_ACTIVE=       FSTATE_GAC_ACTIVE;
    D.E.STATE_GAC_W_STRA=       FSTATE_GAC_W_STRA;
    D.E.STATE_GAC_W_STRB=       FSTATE_GAC_W_STRB;
    D.E.STATE_ARS_ACTIVE=       FSTATE_ARS_ACTIVE;
    D.E.STATE_SL_BAZA=       FSTATE_SL_BAZA;
    D.E.STATE_SL_UR=       FSTATE_SL_UR;
    D.E.STATE_SL_OSO=       FSTATE_SL_OSO;
    D.E.STATE_ZKR_PROGRESS=       FSTATE_ZKR_PROGRESS;
    D.E.STATE_ZKR_S_IN=       FSTATE_ZKR_S_IN;
    D.E.STATE_PUT_NADVIG=       FSTATE_PUT_NADVIG;
    D.E.STATE_KZP_OS=       FSTATE_KZP_OS;
    D.E.STATE_KZP_D=       FSTATE_KZP_D;
    D.E.STATE_V=       STATE2VD(FSTATE_V);
    D.E.STATE_V_ARS=       STATE2VD(FSTATE_V_ARS);
    D.E.STATE_V_KZP=       STATE2VD(FSTATE_V_KZP);
    D.E.STATE_V_DISO=       STATE2VD(FSTATE_V_DISO);
    D.E.STATE_D_RCS_XOFFSET=       FSTATE_D_RCS_XOFFSET;
    D.E.STATE_D_RCF_XOFFSET=       FSTATE_D_RCF_XOFFSET;
    D.E.STATE_D_ORDER_RC=       FSTATE_D_ORDER_RC;

    D.E.STATE_TICK=       FSTATE_TICK;

    D.E.STATE_EXTNUM=   FSTATE_EXTNUM;
    D.E.STATE_EXTNUMPART=   FSTATE_EXTNUMPART;

    D.E.STATE_ZKR_TLG =FSTATE_ZKR_TLG;
    D.E.STATE_SL_VAGON_CNT_PRED =FSTATE_SL_VAGON_CNT_PRED;




}

void m_Otcep::descr_ext2states(const t_NewDescr &D)
{
    resetStates();
    if (D.D.num==0) FSTATE_ENABLED=false;else FSTATE_ENABLED=true;
    //    FNUM=               D.D.num    ; // Номер отцепа 1-255 Живет в течении роспуска одного
    // STATE_LOCATION
    FSTATE_ID_ROSP=          D.D.Id     ;
    FSTATE_MAR=         D.D.mar    ; // Резерв
    FSTATE_MAR_F=       D.D.mar_f  ; // Фактический маршрут(==Заданному либо 0 либо реализов)
    FSTATE_MAR_R=            D.D.r_mar  ; //if(k_min==k_max)  {r_mar = 1;}
    // основные слежение
    FSTATE_DIRECTION=   D.D.dir    ; // Направление
    FSTATE_NAGON=       D.D.nagon  ; // Признак нагона
    FSTATE_ERROR=       D.D.err    ; // Признак неперевода стрелки
    //FSTATE_ERROR_TRACK= D.E.STATE_ERROR_TRACK;

    //FSTATE_GAC_ACTIVE=D.E.STATE_GAC_ACTIVE;
    //FSTATE_ARS_ACTIVE=D.E.STATE_ARS_ACTIVE;
    // CЛ
    FSTATE_SL_VAGON_CNT=D.D.vagon  ;
    changeVagonsCnt(FSTATE_SL_VAGON_CNT);
    FSTATE_ZKR_OSY_CNT=     D.D.osy    ; // Длинна ( в осях)
    FSTATE_SL_VES=           D.D.ves_sl ;
    FSTATE_SL_LEN =         D.D.U_len  ;
    //FSTATE_SL_BAZA=D.E.STATE_SL_BAZA;
    //FSTATE_SL_UR=D.E.STATE_SL_UR;
    //FSTATE_SL_OSO=D.E.STATE_SL_OSO
    FSTATE_SL_STUPEN=   D.D.Stupen ; // Ступень торможения

    // ЗКР
    //FSTATE_ZKR_PROGRESS=D.E.STATE_ZKR_PROGRESS;
    //FSTATE_ZKR_S_IN=D.E.STATE_ZKR_S_IN;
    FSTATE_ZKR_VAGON_CNT=   D.D.len    ; // Длинна ( в вагонах)
    FSTATE_ZKR_OSY_CNT=     D.D.osy;
    FSTATE_ZKR_VES=         D.D.ves    ; // Вес отцепа в тоннах
    FSTATE_ZKR_BAZA=        D.D.baza   ; // Признак длиннобазности
    //FSTATE_PUT_NADVIG=D.E.STATE_PUT_NADVIG;
    // KZP
    //FSTATE_KZP_OS=D.E.STATE_KZP_OS;
    //FSTATE_KZP_D=D.E.STATE_KZP_D;


    // журнал для протокола);
    FSTATE_V_INOUT[0][0]=    VD2STATE(D.D.V_in); // Cкорость входа 1 ТП
    FSTATE_V_INOUT[1][0]=    VD2STATE(D.D.V_out);
    FSTATE_V_INOUT[0][1]=    VD2STATE(D.D.V_in2);
    FSTATE_V_INOUT[1][1]=    VD2STATE(D.D.V_out2);
    FSTATE_V_INOUT[0][2]=    VD2STATE(D.D.V_in3);
    FSTATE_V_INOUT[1][2]=    VD2STATE(D.D.V_out3);

    for (int i=0;i<3;i++){
        FSTATE_OT_RA[0][i]=D.D.t_ot[i]; // 0- растарможка 1-4 ступени максимал ступень работы замедлителя
        FSTATE_OT_RA[1][i]=D.D.r_a[i];// 0-автомат режим ручного вмешательсва
        FSTATE_ADDR_TP[i]=D.D.addr_tp[i];
    }

    // АРС
    FSTATE_V_ZAD[0]=    D.D.V_zad  ; // Скорость заданная
    FSTATE_V_ZAD[1]=D.D.V_zad2 ; // Скорость заданная 2TP
    FSTATE_V_ZAD[2]=D.D.V_zad3 ; // Скорость заданная  3TP

    FSTATE_STUPEN=           D.D.st     ;

    if (D.E.sz_struct!=0){
        FSTATE_LOCATION=D.E.STATE_LOCATION;
    }


    if (D.E.sz_struct==0){
        descr_RCS=otceps->find_RC(D.D.start-1);
        descr_RCF=otceps->find_RC(D.D.end-1);
    } else {
        descr_RCS=otceps->find_RC_ID(D.E.STATE_ID_RCS);
        descr_RCF=otceps->find_RC_ID(D.E.STATE_ID_RCF);
        FSTATE_ERROR_TRACK= D.E.STATE_ERROR_TRACK;
        FSTATE_IS_CURRENT= D.E.STATE_IS_CURRENT;
        FSTATE_GAC_ACTIVE=D.E.STATE_GAC_ACTIVE;
        FSTATE_GAC_W_STRA=D.E.STATE_GAC_W_STRA;
        FSTATE_GAC_W_STRB=D.E.STATE_GAC_W_STRB;
        FSTATE_ARS_ACTIVE=D.E.STATE_ARS_ACTIVE;
        FSTATE_SL_BAZA=D.E.STATE_SL_BAZA;
        FSTATE_SL_UR=D.E.STATE_SL_UR;
        FSTATE_SL_OSO=D.E.STATE_SL_OSO;
        FSTATE_ZKR_PROGRESS=D.E.STATE_ZKR_PROGRESS;
        FSTATE_ZKR_S_IN=D.E.STATE_ZKR_S_IN;
        FSTATE_PUT_NADVIG=D.E.STATE_PUT_NADVIG;
        FSTATE_KZP_OS=D.E.STATE_KZP_OS;
        FSTATE_KZP_D=D.E.STATE_KZP_D;
        //динамика
        FSTATE_V=VD2STATE(D.E.STATE_V);
        FSTATE_V_RC=        VD2STATE(D.D.V_rc); // Скорость по РЦ
        FSTATE_V_ARS=VD2STATE(D.E.STATE_V_ARS);
        FSTATE_V_KZP=VD2STATE(D.E.STATE_V_KZP);
        FSTATE_V_DISO=VD2STATE(D.E.STATE_V_DISO);
        // смещение головы, хвоста в сторону direct [0..LEN]
        FSTATE_D_RCS_XOFFSET=D.E.STATE_D_RCS_XOFFSET;
        FSTATE_D_RCF_XOFFSET=D.E.STATE_D_RCF_XOFFSET;
        FSTATE_D_ORDER_RC=D.E.STATE_D_ORDER_RC;

        FSTATE_EXTNUM=D.E.STATE_EXTNUM;
        FSTATE_EXTNUMPART=D.E.STATE_EXTNUMPART;



        FSTATE_ZKR_TLG=D.E.STATE_ZKR_TLG;
        FSTATE_SL_VAGON_CNT_PRED=D.E.STATE_SL_VAGON_CNT_PRED;
        FSTATE_TICK=D.E.STATE_TICK;
    }
}


void m_Otcep::update_descr()
{
    const t_NewDescr *Descr=(const t_NewDescr *)FSIGNAL_DATA.value_data(sizeof(t_Descr));
    if (Descr==nullptr){
        setSTATE_ENABLED(false);
        return ;
    }
    auto sz=sizeof(t_NewDescr);
    if ((unsigned int)FSIGNAL_DATA.getBuffer()->sizeData()<sz){
        sz=(unsigned int)FSIGNAL_DATA.getBuffer()->sizeData();
    }
    ;
    bool b_new=false;
    if (memcmp(&stored_Descr,Descr,sz)!=0){
        memcpy(&stored_Descr,Descr,sz);
        b_new=true;
    }
    if (b_new){
        descr_ext2states(stored_Descr);
        if (otceps!=nullptr) otceps->vagons2Otceps();
        setBusyRC();
        doStateChanged();

    }

}

void m_Otcep::updateStates_0()
{
    // стандартная обработка
    update_descr();
}



//void m_Otcep::updateStates_1()
//{
//    if (FSIGNAL_DATA.getBuffer()==nullptr) return;
//    if (FSIGNAL_DATA.getBuffer()->A==_storedA) return;
//    _storedA=FSIGNAL_DATA.getBuffer()->A;
//    QString S=QString::fromUtf8(FSIGNAL_DATA.getBuffer()->A);
//    QVariantHash m=MVP_System::QStringToQVariantHash(S);
//    if (m["NUM"].toInt()!=FNUM){
//        resetStates();
//        return;
//    }
//    foreach (const QString &state, m.keys()) {
//        if (state=="NUM") continue;
//        QString stateName="STATE_"+state;
//        setProperty(qPrintable(stateName),m[state]);
//    }
//    if (otceps->mID2Rc.contains(FSTATE_ID_RCS))
//            RCS=otceps->mID2Rc[FSTATE_ID_RCS]; else RCS=nullptr;
//    if (otceps->mID2Rc.contains(FSTATE_ID_RCF))
//            RCF=otceps->mID2Rc[FSTATE_ID_RCF]; else RCF=nullptr;
//    setBusyRC();
//}


