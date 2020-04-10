#include "m_rc.h"

#include <QDebug>
#include "baseobjecttools.h"

#include "m_svet.h"

#include "mvp_objectfactory.h"
REGISTERPROPERTY(m_RC,NEXTDM00,"СЛЕД","Следующая РЦ","СВЯЗИ")
REGISTERPROPERTY(m_RC,NEXTDM10,"ПРЕД","Предыдущая РЦ","СВЯЗИ")
REGISTERPROPERTY(m_RC,NEXTDM01,"СЛЕД_МИНУС","Следующая РЦ по минусу","СВЯЗИ")
REGISTERPROPERTY(m_RC,NEXTDM11,"ПРЕД_МИНУС","Предыдущая РЦ по минусу","СВЯЗИ")

REGISTERPROPERTY(m_RC,SV_M00,"СВ СЛЕД","Светофор отправления на следующую РЦ","СВ ИЗ ЗОНЫ")
REGISTERPROPERTY(m_RC,SV_M10,"СВ ПРЕД","Светофор отправления на предыдущую РЦ","СВ ИЗ ЗОНЫ")
REGISTERPROPERTY(m_RC,SV_M01,"СВ СЛЕД_МИНУС","Светофор отправления на cледующую РЦ по минусу","СВ ИЗ ЗОНЫ")
REGISTERPROPERTY(m_RC,SV_M11,"СВ ПРЕД_МИНУС","Светофор отправления на предыдущую РЦ по минусу","СВ ИЗ ЗОНЫ")

REGISTERPROPERTY(m_RC,LEN,"ДЛИНА","Длина мм","")
REGISTERPROPERTY(m_RC,SIGNAL_BUSY,"СИГНАЛ ЗАНЯТИЕ","ТС Занятие РЦ","")
REGISTERPROPERTY(m_RC,SIGNAL_OTC1,"СИГНАЛ ОТСЕЧКА 1","ТС отсечка занятия","")
REGISTERPROPERTY(m_RC,SIGNAL_OTC2,"СИГНАЛ ОТСЕЧКА 2","ТС отсечка занятия","")


int m_RC::DIRECTM() const
{
    if (getNextCount(1)==2) return 1;
    return 0;
}

m_RC::m_RC(QObject *parent) :
    m_Base(parent) ,FLEN(0),
    FSIGNAL_BUSY(),FSIGNAL_OTC1(),FSIGNAL_OTC2(),
    FSIGNAL_ERR_LS(),FSIGNAL_ERR_LZ(),FSIGNAL_ERR_KZ()
{
    for (int d=0;d<2;d++){
        for (int m=0;m<2;m++){
            Fnext[d][m].clear();
            Fsv_m[d][m].clear();
        }
    }
    FSTATE_BUSY=MVP_Enums::TRCBusy::busy_unknow;
    FSTATE_BLOCK=0;
    Fnext[0][1].setInNotUse(true);
    Fnext[1][1].setInNotUse(true);
    Fsv_m[0][0].setInNotUse(true);
    Fsv_m[0][1].setInNotUse(true);
    Fsv_m[1][0].setInNotUse(true);
    Fsv_m[1][1].setInNotUse(true);
    FSIGNAL_OTC1.setIsNoUse(true);
    FSIGNAL_OTC2.setIsNoUse(true);
    resetStates();

}

void m_RC::resetStates()
{
    m_Base::resetStates();
    FSTATE_BUSY=MVP_Enums::TRCBusy::free;
    FSTATE_BLOCK=false;
    FSTATE_ERR_LS=false;
    FSTATE_ERR_LZ=false;
    FSTATE_ERR_KZ=false;
    next_rc[0]=nullptr;
    next_rc[1]=nullptr;
    dtBusy=QDateTime();
    dtFree=QDateTime();
}


void m_RC::validation(ListObjStr *l) const
{
    m_Base::validation(l);

    if ((getNextCount(0)==2)&&(getNextCount(1)==2)){
        l->error(this,QString("2 связи по минусам"));
    }
    for (int d=0;d<2;d++)
        for (int m=0;m<getNextCount(d);m++)
            if ((!Fnext[d][m].isNotUse()) &&(Fnext[d][m].isNull()))
                l->warning(this,QString("Не задана связь NEXT[%1][%2]").arg(d).arg(m));
    for (int d=0;d<2;d++)
        for (int m=0;m<getNextCount(d);m++)
            if ((!Fsv_m[d][m].isNotUse()) &&(Fsv_m[d][m].isNull()))
                l->warning(this,QString("Не задана светофор отправления SV_M[%1][%2]").arg(d).arg(m));
    if ((!Fsv_m[0][0].isNull())&&(!Fsv_m[1][0].isNull())&&(Fsv_m[0][0]==Fsv_m[1][0]))
        l->warning(this,QString("Один светофор в разные стороны"));

    for (int d=0;d<2;d++){
        if (getNextCount(d)>1)
            if ((!Fnext[d][0].isNull())&&(Fnext[d][0]==Fnext[d][1]))
                l->error(this,QString("2 связи на 1 объект d=%1").arg(d));
    }

    for (int d1=0;d1<2;d1++)
        for (int m1=0;m1<getNextCount(d1);m1++)
            for (int d2=0;d2<2;d2++)
                for (int m2=0;m2<getNextCount(d2);m2++)
                    if (d1!=d2){
                        if ((!Fnext[d1][m1].isNull())&&(Fnext[d1][m1]==Fnext[d2][m2]))
                            l->error(this,QString("1 объект в 2ух направлениях"));
                    }


    for (int d=0;d<2;d++){
        for (int m=0;m<2;m++){
            m_RC *nextRC=getNextRC(d,m);
            if (nextRC!=nullptr){
                bool feedback_exists=false;
                for (int mn=0;mn<nextRC->getNextCount(1-d);mn++){
                    if (nextRC->getNextRC(1-d,mn)==this) feedback_exists=true;
                }
                if (!feedback_exists)
                    l->error(this,QString("Нет обратной связи d=%1,m=%2").arg(d).arg(m));
            }
        }
    }


    if (FLEN<=0)
        l->warning(this,"Не задана длина РЦ");
}

m_RC *m_RC::getNextRC(int d, int m) const
{
    if (m==MVP_Enums::TStrelPol::pol_w) return nullptr;
    if (getNextCount(d)==1) m=0;
    if ((d==0)&&(m==0)) return qobject_cast<m_RC *>(Fnext[0][0].obj());
    if ((d==1)&&(m==0)) return qobject_cast<m_RC *>(Fnext[1][0].obj());
    if ((d==0)&&(m==1)) return qobject_cast<m_RC *>(Fnext[0][1].obj());
    if ((d==1)&&(m==1)) return qobject_cast<m_RC *>(Fnext[1][1].obj());
    return nullptr;
}

m_RC *m_RC::getNextRCcfb(int d,int m) const
{
    m_RC * rcNext=getNextRC(d, m);
    if (rcNext!=nullptr){
        // проверяем обратну связь -отсекаем противошерстные положения
        if (rcNext->getNextRC(1-d,rcNext->STATE_POL())!=this)
            return nullptr;
    }
    return rcNext;
}

m_RC *m_RC::getNextRCpolcfb(int d) const
{
    // если против шерсти - то не рассматриваем положение
    if (getNextCount(d)==1) return getNextRCcfb(d,0);
    return getNextRCcfb(d,STATE_POL());
}

m_Svet *m_RC::getSV_M(int d, int m) const
{
    return qobject_cast<m_Svet *>(Fsv_m[d][m].obj());
}



void m_RC::setLink(ObjectLink &lnk, const ObjectLink &link_new)
{
    lnk=link_new;
    if ((link_new.obj()==nullptr)&&(link_new.storedFid!=0)) {
        const BaseObject *B=superFindObjectById(this,link_new.storedFid);
        if (B) lnk.linkObj(B);
    }
}

void m_RC::setsv_m(ObjectLink p, int d, int m)
{
    ObjectLink old_p=Fsv_m[d][m];
    setLink(Fsv_m[d][m],p);
    if (Fsv_m[d][m]!=old_p) doPropertyChanged();
}

void m_RC::updateAfterLoad()
{
    m_Base::updateAfterLoad();
    for (int d=0;d<2;d++){
        for (int m=0;m<2;m++){
            if (Fnext[d][m].id()!=0){
                const BaseObject *B=superFindObjectById(this,Fnext[d][m].id());
                if (!B){
                    qCritical() << objectName() << QString("Ошибочная ссылка NEXT d=%1 m=%2 ").arg(d).arg(m) <<endl ;
                }
                Fnext[d][m].linkObj(B);
            }

            if (Fsv_m[d][m].id()!=0){
                const BaseObject *B=superFindObjectById(this,Fsv_m[d][m].id());
                if (!B){
                    qCritical() << objectName() << QString("Ошибочная ссылка M_SV d=%1 m=%2 ").arg(d).arg(m) <<endl ;
                }
                Fsv_m[d][m].linkObj(B);
            }
            _sv_m[d][m]=qobject_cast<m_Svet*>(reLink(this,Fsv_m[d][m]));
            l_devices.push_back(_sv_m[d][m]);
        }
    }
}

void m_RC::linkNext(int d, int m, BaseObject *B)
{
    if ((d==0)&&(m==0)) setnext00(ObjectLink(B));
    if ((d==1)&&(m==0)) setnext10(ObjectLink(B));
    if ((d==0)&&(m==1)) setnext01(ObjectLink(B));
    if ((d==1)&&(m==1)) setnext11(ObjectLink(B));
}

bool m_RC::is33()
{
    if (m_Base::is33()) return true;
    return FSIGNAL_BUSY.is33();
}


void m_RC::updateStates()
{
    m_Base::updateStates();

    if (!FSTATE_33){
        MVP_Enums::TRCBusy B=MVP_Enums::TRCBusy::busy_unknow;
        if (FSIGNAL_BUSY.isEmpty()){
            B=MVP_Enums::TRCBusy::busy_not_accepted;
        } else {
            if (FSIGNAL_BUSY.value_1bit()==1){
                if ((FSIGNAL_OTC1.value_1bit()==1)||(FSIGNAL_OTC2.value_1bit()==1)){
                    B=MVP_Enums::TRCBusy::free;
                } else {
                    B=MVP_Enums::TRCBusy::busy;
                }
            }else{
                B=MVP_Enums::TRCBusy::free;
            }
        }
        if (B!=FSTATE_BUSY){
            if (B==MVP_Enums::TRCBusy::busy) dtBusy=FSIGNAL_BUSY.getBuffer()->dataChangedTime();
            if (B==MVP_Enums::TRCBusy::free) dtFree=FSIGNAL_BUSY.getBuffer()->dataChangedTime();
            setSTATE_BUSY(B);
        }

        setSignalState(FSIGNAL_ERR_LS,FSTATE_ERR_LS);
        setSignalState(FSIGNAL_ERR_LZ,FSTATE_ERR_LZ);
        setSignalState(FSIGNAL_ERR_KZ,FSTATE_ERR_KZ);
        next_rc[0]=getNextRCpolcfb(0);
        next_rc[1]=getNextRCpolcfb(1);
    }

}



QList<m_RC *> m_RC::goGrafpolcfb(m_RC *rcStart, int d, int maxRC)
{
    QList<m_RC *> l;
    if (!rcStart) return l;
    if (rcStart->STATE_POL()==MVP_Enums::pol_w) return l;
    l.push_back(rcStart);
    m_RC *rcNext=rcStart->getNextRCpolcfb(d);
    for (int i=0;i<maxRC;i++){
        if (!rcNext) break;
        l.push_back(rcNext);
        rcNext=rcNext->getNextRCpolcfb(d);
    }
    return l;
}



void m_RC::addDevice(m_Base *m)
{
    if (m==nullptr) return;
    if (!l_devices.contains(m)) l_devices.push_back(m);
}


