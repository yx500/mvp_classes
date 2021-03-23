#include "m_ukvag.h"


#include "mvp_objectfactory.h"


REGISTERELEMENT(m_UkVag,"Указатель вагонов","MODEL ГОРКА")
m_UkVag::m_UkVag(QObject *parent) :
    m_Base(parent),
    FSTATE_VAL1(0),FSTATE_VAL2(0),FSTATE_VAL3(0)
{

}

void m_UkVag::resetStates()
{
    m_Base::resetStates() ;
    FSTATE_VAL1=0;
    FSTATE_VAL2=0;
    FSTATE_VAL3=0;
}

void m_UkVag::updateStates()
{
    m_Base::updateStates();
    if ((FSIGNAL_DATA.isUsed())&&(!FSIGNAL_DATA.isInnerUse())){
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
