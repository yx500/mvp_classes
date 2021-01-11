#include "m_rc_gor_zam.h"

#include <QDebug>
#include "baseobjecttools.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(m_RC_Gor_Zam,"РЦ замедлителя","MODEL ГОРКА")

REGISTERPROPERTY(m_RC_Gor_Zam,KZP,"ЗАМ","Ссылка на модель ТП","")

m_RC_Gor_Zam::m_RC_Gor_Zam(QObject *parent) : m_RC_Gor(parent)
{
    _zam=nullptr;
}

void m_RC_Gor_Zam::validation(ListObjStr *l) const
{
    m_RC_Gor::validation(l);
    if ((!FZAM.isNotUse())&&(FZAM.isNull()))
        l->error(this,"Не задан ЗАМЕДЛИТЕЛЬ");

}

void m_RC_Gor_Zam::updateAfterLoad()
{
    m_RC_Gor::updateAfterLoad();
    updateLink(FZAM);
    _zam=qobject_cast<m_Zam*>(FZAM.baseObject());
    l_devices.push_back(_zam);
}
