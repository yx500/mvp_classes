#include "m_rc_dso.h"

#include <QDebug>
#include "baseobjecttools.h"


#include "mvp_objectfactory.h"
REGISTERMODEL(m_RC_DSO,"Горочная РЦ c ДСО")
REGISTERPROPERTY(m_RC_DSO,D0U,"ДСО_0А","Объект ДСО","ДСО")
REGISTERPROPERTY(m_RC_DSO,D0D,"ДСО_0Б","Объект ДСО","ДСО")
REGISTERPROPERTY(m_RC_DSO,D1U,"ДС1_0А","Объект ДСО","ДСО")
REGISTERPROPERTY(m_RC_DSO,D1D,"ДС1_0Б","Объект ДСО","ДСО")


m_RC_DSO::m_RC_DSO(QObject *parent):
    m_RC(parent)
{
    for (int i=0;i<_dso2_cnt;i++)
        for (int j=0;j<2;j++){
            flnkdso[i][j].clear();
            fDSO[i][j]=0;
        }
    FSIGNAL_DSO_STATE.clear();
    FSIGNAL_DSO_OSY_COUNT.clear();
    FSIGNAL_DSO_STATE.setValueType(SignalDescription::val1byte);
    FSIGNAL_DSO_OSY_COUNT.setValueType(SignalDescription::val1byte);
    FSTATE_DSO_BUSY=false;
    FSTATE_DSO_DIRECT=false;
    FSTATE_DSO_ERR_D1=false;
    FSTATE_DSO_ERR_D2=false;
    FSTATE_DSO_OSY_COUNT=0;
    for (int i=0;i<_dso2_cnt;i++)  {
         fDSO2[i] = 0;
         setSTATE_DSO_ERR_D(i+1,false);
    }


}

m_RC_DSO::~m_RC_DSO()
{
    for (int i=0;i<_dso2_cnt;i++)
        if (fDSO2[i]) delete fDSO2[i];
}

void m_RC_DSO::setDSO(ObjectLink p, int n, int m)
{
    if(flnkdso[n][m]!=p){
        flnkdso[n][m]=p;
        if (p.obj()){
            // проставим имя насильно!
            p.obj()->setObjectName(objectName()+QString("ДСО%1%2").arg(n).arg(m));
        }
        emit propertyChanged(this);
    }
}

void m_RC_DSO::setSTATE_DSO_ERR_D(int i, bool p)
{
    // номер с 1!
    if (i==0) qCritical() << "m_RC_DSO::setSTATE_DSO_ERR_D(int i, bool p) номер с 1 !" ;
    switch (i) {
    case 1:setSTATE_DSO_ERR_D1(p);break;
    case 2:setSTATE_DSO_ERR_D2(p);break;
    }
}


void m_RC_DSO::validation(ListObjStr *l) const
{
    m_RC::validation(l);
    for (int i=0;i<_dso2_cnt;i++){
        for (int j=0;j<2;j++){
            if (flnkdso[i][j].isNull())
                l->error(this,QString("Не задана связь ДСО[%1][%2]").arg(i).arg(j));
        }
    }

    if ((FSIGNAL_DSO_STATE.isEmpty()))
        l->warning(this,"Не задан сигнал ДСО состояние");
    if ((FSIGNAL_DSO_OSY_COUNT.isEmpty()))
        l->warning(this,"Не задан сигнал ДСО оси");

}

void m_RC_DSO::updateAfterLoad()
{
    m_RC::updateAfterLoad();
    BaseObject *B;
    for (int i=0;i<_dso2_cnt;i++){
        for (int j=0;j<2;j++){
            B=superFindObjectById(this,flnkdso[i][j].id());
            fDSO[i][j]=qobject_cast<m_DSO*>(B);
            if (!fDSO[i][j]){
                qCritical() << objectName() << QString(" Ошибочная ссылка ДСО[%1][%2]").arg(i).arg(j);
            } else{
                flnkdso[i][j].linkObj(B);
            }
        }
    }
    FSIGNAL_DSO_STATE.setValueType(SignalDescription::val1byte);
    FSIGNAL_DSO_OSY_COUNT.setValueType(SignalDescription::val1byte);
    fDSO2[0]=new m_DSO2(fDSO[0][0],fDSO[0][1]);
    fDSO2[1]=new m_DSO2(fDSO[1][0],fDSO[1][1]);
}


void m_RC_DSO::setName(const QString &n)
{
    m_RC::setName(n);
    for (int i=0;i<_dso2_cnt;i++)
        for (int j=0;j<2;j++)
            if (!flnkdso[i][j].isNull())
                flnkdso[i][j].obj()->setObjectName(objectName()+QString("ДСО%1%2").arg(i).arg(j));

}


bool m_RC_DSO::is_33() const
{
    if (m_RC::is_33()) return true;
    return  FSIGNAL_DSO_OSY_COUNT.is33() | FSIGNAL_DSO_STATE.is33();
}

void m_RC_DSO::updateStates()
{
    // получаем состояния по ТС
    m_RC::updateStates();
    // получаем состояния ДСО
    setSTATE_DSO_OSY_COUNT(FSIGNAL_DSO_OSY_COUNT.value_1byte());
    quint8 dsostate=(FSIGNAL_DSO_STATE.value_1byte());
    setSTATE_DSO_BUSY(dsostate & dso_state_busy);
    setSTATE_DSO_ERROR(dsostate & dso_state_error);
    setSTATE_DSO_DIRECT(dsostate & dso_state_direct);
    setSTATE_DSO_ERR_D1(dsostate & dso_state_err1);
    setSTATE_DSO_ERR_D2(dsostate & dso_state_err2);

    // замещаем состояние занятия

    if (FSTATE_DSO_BUSY) setSTATE_BUSY(MVP_Enums::TRCBusy::busy); else
                         setSTATE_BUSY(MVP_Enums::TRCBusy::free);
    if (STATE_DSO_ERROR()) setSTATE_BUSY(MVP_Enums::TRCBusy::busy_unknow);


}

void m_RC_DSO::work()
{
    // состояния ДСО долджны уже просчитаться заранее
    setDSO2_0(fDSO2[0]->GetState());
    setDSO2_1(fDSO2[1]->GetState());
    m_DSO2::TState_DSO2 state;
    int count=FSTATE_DSO_OSY_COUNT;
    for(int i=0;i<2;i++)  {
        fDSO2[i]->work();
        state = fDSO2[i]->GetState();
        switch(state)  {
        case m_DSO2::error:
            setSTATE_DSO_ERR_D(i+1);
            break;
        case m_DSO2::nothing:
            setSTATE_DSO_ERR_D(i+1,false);
            break;
        case m_DSO2::os2park:
            if(i==0)  {
                if(count<7) count++;
                else setSTATE_DSO_ERR_D(2);                 // ошибка второго датчика
            }
            if(i==1)  {
                if(count>0) count--;
                else setSTATE_DSO_ERR_D(1);                 // ошибка первого датчика
            }
            break;
        case m_DSO2::os2gorb:
            if(i==0)  {
                if(count>0) count--;
                else setSTATE_DSO_ERR_D(2);                 // ошибка второго датчика
            }
            if(i==1)  {
                if(count<7) count++;
                else setSTATE_DSO_ERR_D(1);                 // ошибка первого датчика
            }
            break;
        }
    }
    setSTATE_DSO_OSY_COUNT(count);
    // запишем результат
    quint8 dsostate=0;
    if (FSTATE_DSO_ERR_D1) dsostate=dsostate | dso_state_err1;
    if (FSTATE_DSO_ERR_D2) dsostate=dsostate | dso_state_err2;

    FSIGNAL_DSO_STATE.setValue_1byte(dsostate);
    FSIGNAL_DSO_OSY_COUNT.setValue_1byte(FSTATE_DSO_OSY_COUNT);

    // Признак ошибки на любой из 2-х пар датчиков - неопределенное состояние - защитный отказ.
}



