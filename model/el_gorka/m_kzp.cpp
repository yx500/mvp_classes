#include "m_kzp.h"

#include "baseobjecttools.h"
#include "mvp_system.h"
#include "tGorka.h"
#include <QDebug>
#include <math.h>

//#pragma pack(push, 1)
//#define Max_KzpCells 50
//struct  t_KzpCell {
//    qint16 D;   // Çíà÷åíèå
//    quint16 E;   // Îøèáêà
//};
//struct  t_KzpCell_V {
//    qint16  V;   // Скорость*10
//    quint16 P;   // Признак, 0- нормально
//};

//struct  t_KzpCellsPacket {
//    quint32   tick;
//    t_KzpCell Cells[Max_KzpCells];
//    t_KzpCell_V Cells_V[Max_KzpCells];
//};
//#pragma pack(pop)

#include "mvp_objectfactory.h"
REGISTERELEMENT(m_KZP,"КЗП","MODEL ГОРКА")

m_KZP::m_KZP(QObject *parent) : m_Base(parent)
{
    FSIGNAL_ADDR.clear();
    resetStates();
}

void m_KZP::resetStates()
{
    m_Base::resetStates();
    FSTATE_D=0;
    FSTATE_E=0;
    FSTATE_V=0;
    FSTATE_P=0;
    l_td.clear();
}

void m_KZP::validation(ListObjStr *l) const
{
    m_Base::validation(l);

    if (objectCountLinked2Object(this)==0){
        l->warning(this,"Не используется");
    }
}

void m_KZP::updateAfterLoad()
{
    m_Base::updateAfterLoad();
    FSIGNAL_ADDR.acceptGtBuffer();
    FSIGNAL_ADDR.getBuffer()->setMsecPeriodLive(10*1000);

}

bool m_KZP::is33()
{
    if (m_Base::is33()) return true;
    return FSIGNAL_ADDR.is33();
}

void m_KZP::updateStates()
{
    m_Base::updateStates();
    if (FSTATE_33) return;
    int D = 0;
    int E = 1;
    int V=0;
    int P=0;
    {
        t_KzpCellsPacket * KzpCellsPacket = (t_KzpCellsPacket *) FSIGNAL_ADDR.value_data(0,sizeof(t_KzpCellsPacket));

        if (KzpCellsPacket){
            D = KzpCellsPacket->Cells[FSIGNAL_ADDR.chanelOffset()].D;
            E = KzpCellsPacket->Cells[FSIGNAL_ADDR.chanelOffset()].E;
            V = KzpCellsPacket->Cells_V[FSIGNAL_ADDR.chanelOffset()].V;
            P = KzpCellsPacket->Cells_V[FSIGNAL_ADDR.chanelOffset()].P;
            E = E & 0x8000;
            if (D < 0) E = 1;
        }
    }

    setSTATE_D(D);
    setSTATE_E(E);
    setSTATE_V(V/10.);
    setSTATE_P(P);
}

//void m_KZP::work(const QDateTime &T)
//{
//    if (!FSTATE_33)
//        if ((signal_kzp.isUsed()) && (signal_kzp.is33())) setSTATE_33(true);
//    int D = 0;
//    int E = 1;
//    if (FADDR>0){
//        t_KzpCellsPacket * KzpCellsPacket = (t_KzpCellsPacket *) signal_kzp.value_data(sizeof(t_KzpCellsPacket));

//        if (KzpCellsPacket){
//            D = KzpCellsPacket->Cells[FADDR-1].D;
//            E = KzpCellsPacket->Cells[FADDR-1].E;
//            E = E & 0x8000;
//            if (D < 0) E = 1;
//        }
//    }

//    setSTATE_D(D);
//    setSTATE_E(E);
//    update_V(T);
//}

const int _period_=5000;
void m_KZP::update_V(QDateTime T)
{
    if (!T.isValid()){
        l_td.clear();
        return;
    }

    bool bnew=false;
    if (FSTATE_33){
        l_td.clear();
    } else {

        if (l_td.isEmpty()){
            l_td.push_front(Ttd(T,FSTATE_D));
        } else {
            Ttd &td=l_td.front();
            if (td.d!=FSTATE_D){
                bnew=true;
                l_td.push_front(Ttd(T,FSTATE_D));
                if (l_td.size()>100) l_td.removeLast();
            }
        }
    }

    if (l_td.isEmpty()){
        setSTATE_V(_undefV_);
        return;
    }
    if (FSTATE_33){
        setSTATE_V(_undefV_);
        return;
    }
    Ttd td0=l_td.first();
    qint64 ms=0;
    if (bnew){
        // не вносим значения меньше вагона
        if (FSTATE_D<=15){
            l_td.clear();
            setSTATE_V(_undefV_);
            return;
        }
        // если за 2 с показания сильно упаои - сброс
        bool breset=false;
        for (int i=1;i<l_td.size();i++){
            const Ttd &td1=l_td.at(i);
            if ((td1.d-td0.d)>12){
                breset=true;
                break;
            }
            ms=td1.t.msecsTo(td0.t);
            if (ms>2000) break;
        }
        if (breset){
            l_td.clear();
            setSTATE_V(_undefV_);
            return;
        }
        int xx=0;
        // считаем изменения за период
        for (int i=1;i<l_td.size();i++){
            const Ttd &td1=l_td.at(i);
            ms=td1.t.msecsTo(td0.t);
            if (ms>_period_) {
                xx=td0.d-td1.d;
                break;
            }
        }
        if (ms>=_period_) {
            qreal V=_undefV_;
            V=3600.*xx/ms;
            if (fabs(V)<1) V=0;
            if (fabs(V)>80) setSTATE_V(_undefV_); else
                setSTATE_V(V);
            return;
        }
    }
    // если не было изменений
    qint64 ms0=td0.t.msecsTo(T);
    if (ms0>3600) {  // 1км/ч
        setSTATE_V(0);
    }
}

