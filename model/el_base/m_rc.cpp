#include "m_rc.h"

#include <QDebug>
#include "baseobjecttools.h"

#include "m_svet.h"
#include "m_dso.h"

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
    FSIGNAL_BUSY(),
    FSIGNAL_ERR_LS(),FSIGNAL_ERR_LZ(),FSIGNAL_ERR_KZ(),FSIGNAL_BUSY_DSO(),FSIGNAL_BUSY_DSO_ERR(),FSIGNAL_INFO_DSO()
{
    for (int d=0;d<2;d++){
        Fnext_link[d][1].setInNotUse(true);
        for (int m=0;m<2;m++){
            Fnext_link[d][m].clear();
            Fnext[d][m]=nullptr;
            Fsv_m_links[d][m].clear();
            Fsv_m_links[d][m].setInNotUse(true);
        }
    }
    FSTATE_BUSY=MVP_Enums::TRCBusy::busy_unknow;
    FSTATE_BLOCK=0;
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
    FSTATE_BUSY_DSO=MVP_Enums::TRCBusy::free;
    next_rc[0]=nullptr;
    next_rc[1]=nullptr;
    FSTATE_BUSY_DSO_ERR=false;
    FSTATE_OSY_COUNT=0;

    //    dtBusy=QDateTime();
    //    dtFree=QDateTime();
}

void m_RC::updateAfterLoad()
{
    m_Base::updateAfterLoad();
    for (int d=0;d<2;d++){
        for (int m=0;m<2;m++){

            updateLink(Fnext_link[d][m]);
            Fnext[d][m]=qobject_cast<m_RC*>(Fnext_link[d][m].baseObject());

            if ((!Fnext_link[d][m].isEmpty())&&(Fnext[d][m]==nullptr)){
                qCritical() << objectName() << QString("Ошибочная ссылка NEXT d=%1 m=%2 ").arg(d).arg(m) <<endl ;
            }
            updateLink(Fsv_m_links[d][m]);
            Fsv_m[d][m]=qobject_cast<m_Svet*>(Fsv_m_links[d][m].baseObject());


            if ((!Fsv_m_links[d][m].isEmpty())&&(Fsv_m[d][m]==nullptr)){
                qCritical() << objectName() << QString("Ошибочная ссылка M_SV d=%1 m=%2 ").arg(d).arg(m) <<endl ;
            }

            l_devices.push_back(Fsv_m[d][m]);
        }
    }
}

void m_RC::validation(ListObjStr *l) const
{
    m_Base::validation(l);

    if ((getNextCount(0)==2)&&(getNextCount(1)==2)){
        l->error(this,QString("2 связи по минусам"));
    }
    for (int d=0;d<2;d++)
        for (int m=0;m<getNextCount(d);m++)
            if ((!Fnext_link[d][m].isNotUse()) &&(Fnext_link[d][m].isNull()))
                l->warning(this,QString("Не задана связь NEXT[%1][%2]").arg(d).arg(m));
    for (int d=0;d<2;d++)
        for (int m=0;m<getNextCount(d);m++)
            if ((!Fsv_m_links[d][m].isNotUse()) &&(Fsv_m_links[d][m].isNull()))
                l->warning(this,QString("Не задана светофор отправления SV_M[%1][%2]").arg(d).arg(m));
    if ((!Fsv_m_links[0][0].isNull())&&(!Fsv_m_links[1][0].isNull())&&(Fsv_m_links[0][0]==Fsv_m_links[1][0]))
        l->warning(this,QString("Один светофор в разные стороны"));

    for (int d=0;d<2;d++){
        if (getNextCount(d)>1)
            if ((!Fnext_link[d][0].isNull())&&(Fnext_link[d][0]==Fnext_link[d][1]))
                l->error(this,QString("2 связи на 1 объект d=%1").arg(d));
    }

    for (int d1=0;d1<2;d1++)
        for (int m1=0;m1<getNextCount(d1);m1++)
            for (int d2=0;d2<2;d2++)
                for (int m2=0;m2<getNextCount(d2);m2++)
                    if (d1!=d2){
                        if ((!Fnext_link[d1][m1].isNull())&&(Fnext_link[d1][m1]==Fnext_link[d2][m2]))
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

    if (FSIGNAL_INFO_DSO.chanelOffset()>DSO_Data_Max){
        l->error(this,"Выход за пределы буфера","","SIGNAL_INFO_DSO");
    }
}

m_RC *m_RC::getNextRC(int d, int m) const
{
    if (m==MVP_Enums::TStrelPol::pol_w) return nullptr;
    if (getNextCount(d)==1) m=0;
    if ((d>=0)&&(m>=0)&&(d+m<=2))   return Fnext[d][m];
    return nullptr;
}
m_Svet *m_RC::getSV_M(int d, int m) const
{
    if ((d>=0)&&(m>=0)&&(d+m<=2))   return (Fsv_m[d][m]);
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





void m_RC::setnextLink(const ObjectLink &p, int d, int m)
{
    if ((d>=0)&&(m>=0)&&(d+m<=2)){
        Fnext[d][m]=qobject_cast<m_RC*>(Fnext_link[d][m].baseObject());
        SETPROP(Fnext_link[d][m]);
    }

}
void m_RC::linkNext(int d, int m, BaseObject *B)
{
    setnextLink(ObjectLink(B),d,m);
}



void m_RC::setsv_mLink(ObjectLink p, int d, int m)
{
    if ((d>=0)&&(m>=0)&&(d+m<=2)){
        Fsv_m[d][m]=qobject_cast<m_Svet*>(Fsv_m_links[d][m].baseObject());
        SETPROP(Fsv_m_links[d][m]);
    }
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
        setSignalState(FSIGNAL_BUSY_DSO_ERR,FSTATE_BUSY_DSO_ERR);

        MVP_Enums::TRCBusy B=MVP_Enums::TRCBusy::busy_unknow;
        MVP_Enums::TRCBusy B_DSO=MVP_Enums::TRCBusy::busy_unknow;
        if ((FSIGNAL_BUSY.isEmpty())&&(FSIGNAL_BUSY_DSO.isEmpty())){
            B=MVP_Enums::TRCBusy::busy_not_accepted;
        } else {
            if((!FSIGNAL_BUSY.isEmpty())&&(FSIGNAL_BUSY.value_1bit()==1)) {
                B=MVP_Enums::TRCBusy::busy;
            }else{
                B=MVP_Enums::TRCBusy::free;
            }
        }
        if (!FSIGNAL_BUSY_DSO.isInnerUse()){
            if ((!FSIGNAL_BUSY_DSO.isEmpty())&&(FSIGNAL_BUSY_DSO.value_1bit()==1))
                setSTATE_BUSY_DSO(MVP_Enums::TRCBusy::busy); else
                setSTATE_BUSY_DSO(B_DSO=MVP_Enums::TRCBusy::free);
        }

        if ((B==MVP_Enums::TRCBusy::busy) || (FSTATE_BUSY_DSO==MVP_Enums::TRCBusy::busy)){
            B=MVP_Enums::TRCBusy::busy;
        }

        if(FSTATE_BUSY_DSO_ERR){
            B=MVP_Enums::TRCBusy::busy_unknow;
            B_DSO=MVP_Enums::TRCBusy::busy_unknow;
        }

        setSTATE_BUSY(B);

        setSignalState(FSIGNAL_ERR_LS,FSTATE_ERR_LS);
        setSignalState(FSIGNAL_ERR_LZ,FSTATE_ERR_LZ);
        setSignalState(FSIGNAL_ERR_KZ,FSTATE_ERR_KZ);
        next_rc[0]=getNextRCpolcfb(0);
        next_rc[1]=getNextRCpolcfb(1);

        if (!FSIGNAL_INFO_DSO.isEmpty()){
            DSO_Data *d=(DSO_Data *)FSIGNAL_INFO_DSO.value_data(sizeof(DSO_Data));
            if (d!=nullptr){
                setSTATE_OSY_COUNT(d->V);
            } else {
                setSTATE_OSY_COUNT(0);
            }
        } else setSTATE_OSY_COUNT(0);
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


