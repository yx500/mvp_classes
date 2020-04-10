#include "m_ves.h"

#include <QDebug>
#include "baseobjecttools.h"
#include "mvp_objectfactory.h"
REGISTERELEMENT(m_Ves,"Весомер","MODEL ГОРКА")

m_Ves::m_Ves(QObject *parent) :
    m_Base(parent),FSTATE_OSY_COUNT(),FSTATE_OSY_SREDN_VES()
{
    FSIGNAL_STATE_VES.clear();
    FRC.clear();
    rc=nullptr;
    resetStates();
}

void m_Ves::resetStates()
{
    m_Base::resetStates() ;
    FSTATE_OSY_COUNT=0;
    FSTATE_OSY_SREDN_VES=0;
}

void m_Ves::validation(ListObjStr *l) const
{
    m_Base::validation(l);
    if (FRC.isNull())
        l->warning(this,"Нет привязки к РЦ");

}

void m_Ves::updateAfterLoad()
{
    m_Base::updateAfterLoad();
    rc=qobject_cast<m_RC*>(reLink(this,FRC));
    if (rc==nullptr)
        qWarning() << objectName() << "Весомер ссылается не на РЦ " << FRC.obj()->objectName(); else
        rc->addDevice(this);
}

bool m_Ves::is33()
{
    if (m_Base::is33()) return true;
    return FSIGNAL_STATE_VES.is33();
}

// старя структура
#pragma pack(push,1)
struct tBufVes{
    quint8 osy;
    quint8 ves;
};
#pragma pack(pop)

void m_Ves::updateStates()
{
    m_Base::updateStates();
    if (!FSTATE_33){
        const tBufVes *b=(const tBufVes *)FSIGNAL_STATE_VES.value_data(sizeof(tBufVes));
        if (b) {
            setSTATE_33(false);
            setSTATE_OSY_COUNT(b->osy);
            setSTATE_OSY_SREDN_VES(b->ves/10.);
        }
    }
}
