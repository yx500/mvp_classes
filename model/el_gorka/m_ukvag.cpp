#include "m_ukvag.h"


#include "mvp_objectfactory.h"


REGISTERELEMENT(m_UkVag,"Указатель вагонов","MODEL ГОРКА")
m_UkVag::m_UkVag(QObject *parent) :
    m_Base(parent),
    FSTATE_D1(0),FSTATE_D2(0),FSTATE_D3(0),
    FSTATE_M1(0),FSTATE_M2(0),FSTATE_M3(0)
{

}

void m_UkVag::resetStates()
{
    m_Base::resetStates() ;
    FSTATE_D1=0;
    FSTATE_D2=0;
    FSTATE_D3=0;
    FSTATE_M1=0;
    FSTATE_M2=0;
    FSTATE_M3=0;
}

void m_UkVag::updateAfterLoad()
{
    if (FSIGNAL_DATA.isUsed())  FSIGNAL_DATA.getBuffer()->setSizeData(sizeof(UkVag));
    if (FSIGNAL_SET.isUsed())  FSIGNAL_SET.getBuffer()->setSizeData(sizeof(UkVag));
}

void m_UkVag::updateStates()
{
    m_Base::updateStates();
    if ((FSIGNAL_DATA.isUsed())&&(!FSIGNAL_DATA.isInnerUse())){
        UkVag *data=(UkVag *)FSIGNAL_DATA.value_data(sizeof(UkVag));
        FSTATE_D1=data->data[0].D;
        FSTATE_D2=data->data[1].D;
        FSTATE_D3=data->data[2].D;
        FSTATE_M1=data->data[0].M;
        FSTATE_M2=data->data[1].M;
        FSTATE_M3=data->data[2].M;
//        t_OsyCell_21 *d=(t_OsyCell_21 *)FSIGNAL_DSODATA.value_data(sizeof(t_OsyCell_21));
//        if (d!=nullptr){
//            setSTATE_OSY_COUNT(d->V);
//            setSTATE_DIRECT(d->D);
//            setSTATE_ERROR_TRACK(d->E);
//            setSTATE_ERROR_CNT(d->EV);
//        } else {
//            setSTATE_OSY_COUNT(0);
//            setSTATE_DIRECT(0);
//            setSTATE_ERROR_TRACK(false);
//            setSTATE_ERROR_CNT(0);
//        }
    }
}
