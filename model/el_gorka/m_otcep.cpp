#include "m_otcep.h"
#include "m_otceps.h"
#include "gtbuffer.h"
#include "mvp_system.h"



void m_Otcep::setSTATE_LOCATION(const TOtcepLocation &p)
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
    if (otceps->mMAR2SP.contains(FSTATE_MAR)) return otceps->mMAR2SP[FSTATE_MAR];
    return 0;
}

void m_Otcep::setSTATE_SP(int p)
{
    if (otceps->mSP2MAR.contains(p)) setSTATE_MAR(otceps->mSP2MAR[p]);else
    setSTATE_MAR(0);
}

m_Otcep::m_Otcep(m_Otceps *parent,int num) : m_Base(parent)
{
    otceps=qobject_cast<m_Otceps*>(parent);
    FNUM=num;
    setObjectName(QString("Отцеп %1").arg(FNUM));
    QString packetName=QString("descr%1").arg(FNUM);
    FSIGNAL_ADDR=SignalDescription(9,packetName,0);
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

    FSTATE_VAGON_CNT=0;
    FSTATE_OSY_CNT=0;
    FSTATE_VES=0;
    FSTATE_BAZA=0;
    FSTATE_LEN=0;

    FSTATE_SL_VAGON_CNT=0;
    FSTATE_SL_OSY_CNT=0;
    FSTATE_SL_VES=0;
    FSTATE_SL_BAZA=0;

    FSTATE_ZKR_PROGRESS=false;
    FSTATE_ZKR_VAGON_CNT=0;
    FSTATE_ZKR_OSY_CNT=0;
    FSTATE_ZKR_BAZA=0;

    FSTATE_KZP_OS=kzpUnknow;

    FSTATE_LEN_BY_VAGON=0;
    FSTATE_LEN_BY_RC_MIN=0;
    FSTATE_LEN_BY_RC_MAX=0;

    FSTATE_V=_undefV_;
    FSTATE_V_RC=_undefV_;
    FSTATE_V_ARS=_undefV_;
    FSTATE_V_KZP=_undefV_;
    FSTATE_V_DISO=_undefV_;

    FSTATE_RCS_XOFFSET=-1;
    FSTATE_RCF_XOFFSET=-1;

    FSTATE_V_IN_1=_undefV_;
    FSTATE_V_IN_2=_undefV_;
    FSTATE_V_IN_3=_undefV_;
    FSTATE_V_OUT_1=_undefV_;
    FSTATE_V_OUT_2=_undefV_;
    FSTATE_V_OUT_3=_undefV_;

    FSTATE_CHANGE_COUNTER=0;
    FIDS_RC_BUSY.clear();

    FIDS_RC_BUSY.clear();
    RCS=nullptr;
    RCF=nullptr;
    vVag.clear();
    vBusyRc.clear();


}

void m_Otcep::acceptStaticData(m_Otcep *o)
{
    FSTATE_LOCATION=o->STATE_LOCATION();
    FSTATE_MAR=o->STATE_MAR();
    FSTATE_VAGON_CNT=o->STATE_VAGON_CNT();
    FSTATE_OSY_CNT=o->STATE_SL_OSY_CNT();
    FSTATE_VES=o->STATE_VES();
    //FSTATE_BAZA=o->STATE_BAZA();
    FSTATE_LEN=o->STATE_LEN();

    FSTATE_SL_VAGON_CNT=o->STATE_SL_VAGON_CNT();
    FSTATE_SL_OSY_CNT=o->STATE_SL_OSY_CNT();
    FSTATE_SL_VES=o->STATE_SL_VES();
    FSTATE_SL_BAZA=o->STATE_SL_BAZA();

    vVag.clear();
    vVag=o->vVag;
}

void m_Otcep::updateAfterLoad()
{
    m_Base::updateAfterLoad();
    FSIGNAL_ADDR.acceptGtBuffer();
    connect(FSIGNAL_ADDR.getBuffer(),&GtBuffer::bufferChanged,this,&m_Otcep::slotChanged);
}



void m_Otcep::setLenByVagon()
{
    int Len=0;
    for (int i=0;i<vVag.size();i++){
        if (vVag[i].Ln==0) vVag[i].Ln=15;
        if (vVag[i].Ln>1000) vVag[i].Ln=vVag[i].Ln/1000;
        Len+=vVag[i].Ln;
    }
    setSTATE_LEN_BY_VAGON(Len);
}

void m_Otcep::setBusyRC()
{
    vBusyRc.clear();
    m_RC * RC;
    QStringList lIDS_RC_BUSY;
    if ((RCS!=nullptr)&&(RCF!=nullptr)){
        RC=RCS;
        while (RC!=nullptr){
            vBusyRc.push_back(RC);
            lIDS_RC_BUSY.append(QString("%1").arg(RC->id()));
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
    setIDS_RC_BUSY(lIDS_RC_BUSY.join(';'));
}

bool m_Otcep::is33()
{
    if (m_Base::is33()) return true;
    return FSIGNAL_ADDR.is33();
}

void m_Otcep::updateStates()
{
    if (disableUpdateStates) return;
    if (FSIGNAL_ADDR.isInnerUse()) return;
    // стандартная обработка
        m_Base::updateStates();

        if (FSTATE_33) return ;


        const t_Descr *Descr=(const t_Descr *)FSIGNAL_ADDR.value_data(sizeof(t_Descr));
        if (Descr==nullptr){
            setSTATE_ENABLED(false);
            return ;
        }

        bool b_new=false;
        if (memcmp(&stored_Descr,Descr,sizeof(t_Descr))!=0){
            memcpy(&stored_Descr,Descr,sizeof(t_Descr));
            setSTATE_CHANGE_COUNTER(FSTATE_CHANGE_COUNTER+1);
            b_new=true;
        }
        if (b_new){
            if ((Descr->num==0) || (Descr->num>=MAXCOUNT_OTCEPS)) {
                resetStates();
                return;
            }
            FSTATE_CHANGE_COUNTER++;
            setSTATE_ENABLED(true);
            FNUM=               stored_Descr.num    ; // Номер отцепа 1-255 Живет в течении роспуска одного
            FSTATE_MAR=         stored_Descr.mar    ; // Резерв
            FSTATE_MAR_F=       stored_Descr.mar_f  ; // Фактический маршрут(==Заданному либо 0 либо реализов)
            //stored_Descr.start  ; // Местоположение Головы отцепа |    N Эл РЦ в ТС     |
            //stored_Descr.end    ; // Местоположение Хвоста отцепа |номер сигнала занятия|
            FSTATE_VES=         stored_Descr.ves    ; // Вес отцепа в тоннах
            FSTATE_OSY_CNT=     stored_Descr.osy    ; // Длинна ( в осях)
            FSTATE_VAGON_CNT=   stored_Descr.len    ; // Длинна ( в вагонах)
            FSTATE_BAZA=        stored_Descr.baza   ; // Признак длиннобазности
            FSTATE_NAGON=       stored_Descr.nagon  ; // Признак нагона
            if (stored_Descr.end_slg!=0) // Признак конца слежения (по последней РЦ на путях)
                FSTATE_LOCATION=locationUnknow; else
                FSTATE_LOCATION=locationOnSpusk;


            FSTATE_ERROR=       stored_Descr.err    ; // Признак неперевода стрелки
            FSTATE_DIRECTION=   stored_Descr.dir    ; // Направление
            FSTATE_V_RC=        stored_Descr.V_rc/10.   ; // Скорость по РЦ
//            stored_Descr.V_zad  ; // Скорость заданная
//            stored_Descr.Stupen ; // Ступень торможения
//            stored_Descr.osy1   ; // Длинна ( в осях)
//            stored_Descr.osy2   ; // Длинна ( в осях)
//            stored_Descr.V_zad2 ; // Скорость заданная 2TP
//            stored_Descr.V_zad3 ; // Скорость заданная  3TP
//            stored_Descr.pricel ;
//            stored_Descr.old_num;
//            stored_Descr.old_mar;
//            stored_Descr.U_len  ;
            FSTATE_SL_VAGON_CNT=stored_Descr.vagon  ;
            FSTATE_V_OUT_1=     stored_Descr.V_out/10.  ;
            FSTATE_V_IN_2=      stored_Descr.V_in2/10.  ;
            FSTATE_V_OUT_2=     stored_Descr.V_out2/10. ;
            FSTATE_V_IN_3=      stored_Descr.V_in3/10.  ;
            FSTATE_V_OUT_3=     stored_Descr.V_out3/10. ;
            FSTATE_ID_ROSP=     stored_Descr.Id     ;
//            stored_Descr.st     ;
            FSTATE_SL_VES=      stored_Descr.ves_sl ;
//            stored_Descr.r_mar  ;
//            stored_Descr.t_ot[3]; // 0- растарможка 1-4 ступени максимал ступень работы замедлителя
//            stored_Descr.r_a[3] ; // 0-автомат режим ручного вмешательсва
            FSTATE_V_IN_1=      stored_Descr.V_in/10.   ; // Cкорость входа 1 ТП
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

            if ((RCS!=descr_RCS)||(RCF!=descr_RCF)) {

                RCS=descr_RCS;
                RCF=descr_RCF;
                setBusyRC();

                if ((RCF!=nullptr)&&( RCF->metaObject()->className()==qPrintable("m_RC_Gor_ZKR"))) FSTATE_ZKR_PROGRESS=true; else FSTATE_ZKR_PROGRESS=false;
            }
            doStateChanged();
        }
}

