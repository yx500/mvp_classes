#include "m_strel_dso_dbk.h"

#include <QDebug>
#include "baseobjecttools.h"


#include "mvp_objectfactory.h"
REGISTERMODEL(m_STREL_DSO_DBK,"Cтрелка c ДСО ДБК")

const int _dso_cnt=3;

m_STREL_DSO_DBK::m_STREL_DSO_DBK(QObject *parent):
    m_Strel(parent)
{
    for (int i=0;i<maxdso2cnt;i++)
        for (int j=0;j<2;j++){
            flnkdso[i][j].clear();
            fDSO[i][j]=0;
        }
    FSIGNAL_SRED_POL.clear();
    FSIGNAL_PLUS_PEREVOD.clear();
    FSIGNAL_MINUS_PEREVOD.clear();
    FSIGNAL_SRED_PEREVOD.clear();
    FDSOCNT=dso8;
    FSIGNAL_DSO_STATE.clear();
    FSIGNAL_DSO_OSY_COUNT.clear();
    FSTATE_DSO_BUSY=false;
    FSTATE_DSO_DIRECT=false;
    FSTATE_DSO_ERR_D1=false;
    FSTATE_DSO_ERR_D2=false;
    FSTATE_DSO_ERR_D3=false;
    FSTATE_DSO_OSY_COUNT=0;
    for (int i=0;i<maxdso2cnt;i++)  {
        fDSO2[i] = 0;
        setSTATE_DSO_ERR_D(i+1,false);
    }

}

m_STREL_DSO_DBK::~m_STREL_DSO_DBK()
{
    for (int i=0;i<maxdso2cnt;i++)
        if (fDSO2[i]) delete fDSO2[i];
}

void m_STREL_DSO_DBK::setDSO(ObjectLink p, int n, int m)
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


void m_STREL_DSO_DBK::setSTATE_DSO_ERR_D(int i, bool p)
{
    // номер с 1!
    if (i==0) qCritical() << "m_RC_DSO::setSTATE_DSO_ERR_D(int i, bool p) номер с 1 !" ;
    switch (i) {
    case 1:setSTATE_DSO_ERR_D1(p);break;
    case 2:setSTATE_DSO_ERR_D2(p);break;
    case 3:setSTATE_DSO_ERR_D3(p);break;
    case 4:setSTATE_DSO_ERR_D4(p);break;
    }
}



void m_STREL_DSO_DBK::validation(ListObjStr *l) const
{
    m_Strel::validation(l);
    for (int i=0;i<FDSOCNT/2;i++){
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

void m_STREL_DSO_DBK::updateAfterLoad()
{
    m_Strel::updateAfterLoad();
    BaseObject *B;
    for (int i=0;i<FDSOCNT/2;i++){
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
    fDSO2[0]=new m_DSO2(fDSO[0][0],fDSO[0][1]);
    fDSO2[1]=new m_DSO2(fDSO[1][0],fDSO[1][1]);
    fDSO2[2]=new m_DSO2(fDSO[2][0],fDSO[2][1]);
    fDSO2[3]=new m_DSO2(fDSO[3][0],fDSO[3][1]);
    FSIGNAL_DSO_STATE.setValueType(SignalDescription::val1byte);
    FSIGNAL_DSO_OSY_COUNT.setValueType(SignalDescription::val1byte);
}


void m_STREL_DSO_DBK::setName(const QString &n)
{
    m_Strel::setName(n);
    for (int i=0;i<FDSOCNT/2;i++)
        for (int j=0;j<2;j++)
            if (!flnkdso[i][j].isNull())
                flnkdso[i][j].obj()->setObjectName(objectName()+QString("ДСО%1%2").arg(i).arg(j));

}

bool m_STREL_DSO_DBK::is_33() const
{
    return m_Strel::is_33() |
            FSIGNAL_DSO_OSY_COUNT.is33() | FSIGNAL_DSO_STATE.is33();
}



void m_STREL_DSO_DBK::updateStates()
{
    setSTATE_33(is_33());
    // положение и блокировка
    m_Strel::updateStates();
    // получаем состояния ДСО
    setSTATE_DSO_OSY_COUNT(FSIGNAL_DSO_OSY_COUNT.value_1byte());
    quint8 dsostate=FSIGNAL_DSO_STATE.value_1byte();
    setSTATE_DSO_BUSY(dsostate &   m_RC_DSO::dso_state_busy);
    setSTATE_DSO_ERROR(dsostate &  m_RC_DSO::dso_state_error);
    setSTATE_DSO_DIRECT(dsostate & m_RC_DSO::dso_state_direct);
    setSTATE_DSO_ERR_D1(dsostate & m_RC_DSO::dso_state_err1);
    setSTATE_DSO_ERR_D2(dsostate & m_RC_DSO::dso_state_err2);
    setSTATE_DSO_ERR_D3(dsostate & m_RC_DSO::dso_state_err3);
    setSTATE_DSO_ERR_D4(dsostate & m_RC_DSO::dso_state_err4);

    // замещаем состояние занятия
    setSTATE_BUSY(FSTATE_DSO_BUSY);
    if (STATE_DSO_ERROR()) setSTATE_BUSY(m_RC::busy_unknow);
}

void m_STREL_DSO_DBK::work6()
{
    // состояния ДСО долджны уже просчитаться заранее
    int count=FSTATE_DSO_OSY_COUNT;

    setDSO2_0(fDSO2[0]->GetState());
    setDSO2_1(fDSO2[1]->GetState());
    setDSO2_2(fDSO2[2]->GetState());


    setSTATE_DSO_OSY_COUNT(count);
    // запишем результат
    quint8 dsostate=0;
    if (FSTATE_DSO_ERR_D1) dsostate=dsostate | m_RC_DSO::dso_state_err1;
    if (FSTATE_DSO_ERR_D2) dsostate=dsostate | m_RC_DSO::dso_state_err2;
    if (FSTATE_DSO_ERR_D3) dsostate=dsostate | m_RC_DSO::dso_state_err3;

    FSIGNAL_DSO_STATE.setValue_1byte(dsostate);
    FSIGNAL_DSO_OSY_COUNT.setValue_1byte(FSTATE_DSO_OSY_COUNT);
}

void m_STREL_DSO_DBK::work8()
{
    // состояния ДСО долджны уже просчитаться заранее
    m_DSO2::TState_DSO2 state;
    int count=FSTATE_DSO_OSY_COUNT;
    setDSO2_0(fDSO2[0]->GetState());
    setDSO2_1(fDSO2[1]->GetState());
    setDSO2_2(fDSO2[2]->GetState());
    setDSO2_3(fDSO2[3]->GetState());

    for(int i=0;i<3;i++)  {
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
                if(count<7) count++;                 // вход от горба
                else setSTATE_DSO_ERR_D(2);                 // ошибка второго датчика
            }
            if(i==1)  {
                if(count>0) count--;                 // выход по плюсу
                else setSTATE_DSO_ERR_D(1);                 // ошибка первого датчика
            }
            if(i==2)  {
                if(count>0) count--;                 // выход по минусу
                else setSTATE_DSO_ERR_D(1);                 // ошибка первого датчика
            }
            break;
        case  m_DSO2::os2gorb:
            if(i==0)  {
                if(count>0) count--;                 // выход в сторону горба
                else setSTATE_DSO_ERR_D(2);;                 // ошибка второго датчика
            }
            if(i==1)  {
                if(count<7) count++;                 // вход по плюсу
                else setSTATE_DSO_ERR_D(1);                 // ошибка первого датчика
            }
            if(i==2)  {
                if(count<7) count++;                 // вход по минусу
                else setSTATE_DSO_ERR_D(1);                 // ошибка первого датчика
            }
            break;

            // третий датчик ???


        }
    }
    setSTATE_DSO_OSY_COUNT(count);
    // запишем результат
    quint8 dsostate=0;
    if (FSTATE_DSO_ERR_D1) dsostate=dsostate | m_RC_DSO::dso_state_err1;
    if (FSTATE_DSO_ERR_D2) dsostate=dsostate | m_RC_DSO::dso_state_err2;
    if (FSTATE_DSO_ERR_D3) dsostate=dsostate | m_RC_DSO::dso_state_err3;
    if (FSTATE_DSO_ERR_D4) dsostate=dsostate | m_RC_DSO::dso_state_err4;

    FSIGNAL_DSO_STATE.setValue_1byte(dsostate);
    FSIGNAL_DSO_OSY_COUNT.setValue_1byte(FSTATE_DSO_OSY_COUNT);
}


void m_STREL_DSO_DBK::work()
{
    setSTATE_33(m_Strel::is_33());
    // положение и блокировка
    m_Strel::updateStates();
    if (FDSOCNT==dso6){
        work6();
    } else {
        work8();
    }

}



