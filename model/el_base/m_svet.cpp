#include "m_svet.h"

#include "mvp_objectfactory.h"
//REGISTERELEMENT(m_Svet,"Светофор обычный")

m_Svet::m_Svet(QObject *parent) :
    m_Base(parent)
{
    resetStates();
}

void m_Svet::resetStates()
{
    m_Base::resetStates();
    FSTATE_OPEN=false;
}

