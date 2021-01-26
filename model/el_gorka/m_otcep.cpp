#include "m_otcep.h"
#include "m_otceps.h"
#include "gtbuffer.h"
#include "mvp_system.h"
#include "modelgroupgorka.h"



void m_Otcep::setSTATE_LOCATION(const int &p)
{
    if (FSTATE_LOCATION!=p){
        //if (p==locationUnknow) resetStates();
        FSTATE_LOCATION=p;
        doStateChanged();
    }
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
bool m_Otcep::STATE_BAZA() const
{
    return FSTATE_SL_BAZA | FSTATE_ZKR_BAZA;
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
    memset(&stored_Descr,0,sizeof(stored_Descr));

    FSTATE_ENABLED=false;
    FSTATE_CHANGE_COUNTER=0;
    FSTATE_LOCATION=locationUnknow;
    FSTATE_MAR=0;
    FSTATE_MAR_F=0;
    FSTATE_DIRECTION=0;
    FSTATE_NAGON=0;
    FSTATE_ERROR=0;
    FSTATE_ID_ROSP=0;
    FSTATE_SL_UR=0;
    FSTATE_ERROR_TRACK=false;

    FSTATE_SL_VAGON_CNT=0;
    FSTATE_SL_OSY_CNT=0;
    FSTATE_SL_LEN=0;
    FSTATE_SL_VES=0;
    FSTATE_SL_BAZA=0;
    FSTATE_SL_UR=0;
    FSTATE_SL_OSO=0;

    FSTATE_ZKR_PROGRESS=false;
    FSTATE_ZKR_VAGON_CNT=0;
    FSTATE_ZKR_OSY_CNT=0;
    FSTATE_ZKR_BAZA=0;
    FSTATE_ZKR_VES=0;
    FSTATE_PUT_NADVIG=0;


    FSTATE_KZP_OS=kzpUnknow;
    FSTATE_KZP_D=0;


    FSTATE_V=_undefV_;
    FSTATE_V_RC=_undefV_;
    FSTATE_V_ARS=_undefV_;
    FSTATE_V_KZP=_undefV_;
    FSTATE_V_DISO=_undefV_;

    FSTATE_D_RCS_XOFFSET=-1;
    FSTATE_D_RCF_XOFFSET=-1;

    memset(FSTATE_V_INOUT,0,sizeof(FSTATE_V_INOUT));
    memset(FSTATE_OT_RA,0,sizeof(FSTATE_OT_RA));
    memset(FSTATE_V_ZAD,0,sizeof(FSTATE_V_ZAD));



    FSTATE_CHANGE_COUNTER=0;
    FSTATE_IDS_RCS.clear();
    FSTATE_IDS_RCF.clear();
    RCS=nullptr;
    RCF=nullptr;
    vVag.clear();
    vBusyRc.clear();


}

void m_Otcep::acceptSLStates(const m_Otcep *o)
{
    FSTATE_MAR=o->STATE_MAR();
    FSTATE_SL_VAGON_CNT=o->STATE_SL_VAGON_CNT();
    FSTATE_SL_OSY_CNT=o->STATE_SL_OSY_CNT();
    FSTATE_SL_LEN=o->STATE_SL_LEN();
    FSTATE_SL_VES=o->STATE_SL_VES();
    FSTATE_SL_BAZA=o->STATE_SL_BAZA();
    FSTATE_SL_UR=o->STATE_SL_UR();
    FSTATE_SL_OSO=o->STATE_SL_OSO();
    for (int i=0;i<3;i++) FSTATE_V_ZAD[i]=o->STATE_V_ZAD(i);

    vVag.clear();
    vVag=o->vVag;
}



void m_Otcep::updateAfterLoad()
{
    m_Base::updateAfterLoad();
    FSIGNAL_DATA.acceptGtBuffer();
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
    QString S;
    if (RCS!=nullptr) S=RCS->idstr();else S="";
    setSTATE_IDS_RCS(S);
    if (RCF!=nullptr) S=RCF->idstr();else S="";
    setSTATE_IDS_RCF(S);
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
    if (FSIGNAL_DATA.chanelType()==9) updateStates_0();
    if (FSIGNAL_DATA.chanelType()==109) updateStates_1();

}

void m_Otcep::update_descr()
{
    const t_Descr *Descr=(const t_Descr *)FSIGNAL_DATA.value_data(sizeof(t_Descr));
    if (Descr==nullptr){
        setSTATE_ENABLED(false);
        return ;
    }

    bool b_new=false;
    if (memcmp(&stored_Descr,Descr,sizeof(t_Descr))!=0){
        memcpy(&stored_Descr,Descr,sizeof(t_Descr));
        b_new=true;
    }
    if (b_new){
        if ((Descr->num==0) || (Descr->num>=MAXCOUNT_OTCEPS)) {
            resetStates();
            return;
        }
        FSTATE_ENABLED=true;
        FNUM=               stored_Descr.num    ; // Номер отцепа 1-255 Живет в течении роспуска одного
        FSTATE_MAR=         stored_Descr.mar    ; // Резерв
        FSTATE_MAR_F=       stored_Descr.mar_f  ; // Фактический маршрут(==Заданному либо 0 либо реализов)
        //stored_Descr.start  ; // Местоположение Головы отцепа |    N Эл РЦ в ТС     |
        //stored_Descr.end    ; // Местоположение Хвоста отцепа |номер сигнала занятия|
        FSTATE_ZKR_VES=         stored_Descr.ves    ; // Вес отцепа в тоннах
        FSTATE_ZKR_OSY_CNT=     stored_Descr.osy    ; // Длинна ( в осях)
        FSTATE_ZKR_VAGON_CNT=   stored_Descr.len    ; // Длинна ( в вагонах)
        FSTATE_ZKR_BAZA=        stored_Descr.baza   ; // Признак длиннобазности
        FSTATE_NAGON=       stored_Descr.nagon  ; // Признак нагона



        FSTATE_ERROR=       stored_Descr.err    ; // Признак неперевода стрелки
        FSTATE_DIRECTION=   stored_Descr.dir    ; // Направление
        FSTATE_V_RC=        stored_Descr.V_rc/10.   ; // Скорость по РЦ
        FSTATE_V_ZAD[0]=   stored_Descr.V_zad  ; // Скорость заданная
        FSTATE_SL_STUPEN=stored_Descr.Stupen ; // Ступень торможения
        //            stored_Descr.osy1   ; // Длинна ( в осях)
        //            stored_Descr.osy2   ; // Длинна ( в осях)
        FSTATE_V_ZAD[1]=stored_Descr.V_zad2 ; // Скорость заданная 2TP
        FSTATE_V_ZAD[2]=stored_Descr.V_zad3 ; // Скорость заданная  3TP
        //            stored_Descr.pricel ;
        //            stored_Descr.old_num;
        //            stored_Descr.old_mar;
        FSTATE_SL_LEN =        stored_Descr.U_len  ;
        FSTATE_SL_VAGON_CNT=stored_Descr.vagon  ;

        FSTATE_V_INOUT[1][0]=    stored_Descr.V_out==65535 ? _undefV_:  stored_Descr.V_out/10.  ;
        FSTATE_V_INOUT[0][1]=      stored_Descr.V_in2==65535 ? _undefV_:stored_Descr.V_in2/10.  ;
        FSTATE_V_INOUT[1][1]=     stored_Descr.V_out2==65535 ? _undefV_:stored_Descr.V_out2/10. ;
        FSTATE_V_INOUT[0][2]=      stored_Descr.V_in3==65535 ? _undefV_:stored_Descr.V_in3/10.  ;
        FSTATE_V_INOUT[1][2]=     stored_Descr.V_out3==65535 ? _undefV_:stored_Descr.V_out3/10. ;
        FSTATE_ID_ROSP=     stored_Descr.Id     ;
        FSTATE_STUPEN=           stored_Descr.st     ;
        FSTATE_SL_VES=      stored_Descr.ves_sl ;
        //            stored_Descr.r_mar  ; if(k_min==k_max)  {r_mar = 1;}

        for (int i=0;i<3;i++){
            FSTATE_OT_RA[0][i]=stored_Descr.t_ot[i]; // 0- растарможка 1-4 ступени максимал ступень работы замедлителя
            FSTATE_OT_RA[1][i]=stored_Descr.r_a[i];// 0-автомат режим ручного вмешательсва
        }
        FSTATE_V_INOUT[0][0]=      stored_Descr.V_in==65535 ? _undefV_:stored_Descr.V_in/10.   ; // Cкорость входа 1 ТП
        //            stored_Descr.Kzp    ; // КЗП по расчету Антона
        //            stored_Descr.v_rosp ; // Скорость расформирования   - норма/быстро/медленно - 0/1/2
        //            stored_Descr.flag_ves; // Работоспособность весомера - да/нет/ - 0/1
        //            stored_Descr.flag_r  ; // Признак ручной установки скорости
        //            stored_Descr.FirstVK ;
        //            stored_Descr.LastVK  ;
        //            stored_Descr.addr_tp[3]; // Занятый замедлитель
        //            stored_Descr.v_rosp1 ; // Скорость расформирования   - норма/быстро/медленно - 0/1/2
        //            stored_Descr.p_rzp   ; // Признак выше ПТП
        //                   stored_Descr.VrospZ;
        //                   stored_Descr.VrospF;
        //                   stored_Descr.V_zad2_S ; // Скорость заданная 2TP



        descr_RCS=otceps->find_RC(Descr->start-1);
        descr_RCF=otceps->find_RC(Descr->end-1);
        setSTATE_CHANGE_COUNTER(FSTATE_CHANGE_COUNTER+1);

    }

}

void m_Otcep::updateStates_0()
{
    // стандартная обработка
    update_descr();
    if (stored_Descr.end_slg!=0) // Признак конца слежения (по последней РЦ на путях)
        setSTATE_LOCATION(locationUnknow); else
        setSTATE_LOCATION(locationOnSpusk);
    if ((RCS!=descr_RCS)||(RCF!=descr_RCF)) {

        RCS=descr_RCS;
        RCF=descr_RCF;
        setBusyRC();
        m_RC_Gor_ZKR * zkr=qobject_cast<m_RC_Gor_ZKR*>(RCF);
        if (zkr!=nullptr){
            setSTATE_ZKR_PROGRESS(true);
            setSTATE_PUT_NADVIG(zkr->PUT_NADVIG());
        } else {
            setSTATE_ZKR_PROGRESS(false);
        }
        doStateChanged();

    }
}

void m_Otcep::updateStates_1()
{
    if (FSIGNAL_DATA.getBuffer()==nullptr) return;
    if (FSIGNAL_DATA.getBuffer()->A==_storedA) return;
    _storedA=FSIGNAL_DATA.getBuffer()->A;
    QString S=QString::fromUtf8(FSIGNAL_DATA.getBuffer()->A);
    QVariantHash m=MVP_System::QStringToQVariantHash(S);
    if (m["NUM"].toInt()!=FNUM){
        resetStates();
        return;
    }
    foreach (const QString &state, m.keys()) {
        if (state=="NUM") continue;
        QString stateName="STATE_"+state;
        setProperty(qPrintable(stateName),m[state]);
    }
    if (otceps->mIDS2Rc.contains(FSTATE_IDS_RCS))
            RCS=otceps->mIDS2Rc[FSTATE_IDS_RCS]; else RCS=nullptr;
    if (otceps->mIDS2Rc.contains(FSTATE_IDS_RCF))
            RCF=otceps->mIDS2Rc[FSTATE_IDS_RCF]; else RCF=nullptr;
    setBusyRC();
}


