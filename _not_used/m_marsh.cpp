#include "m_marsh.h"

#include <QDebug>
#include "baseobjecttools.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(m_Marsh,"Маршрут","Элементы модели ГОРКА")

m_Marsh::m_Marsh(QObject *parent) :
    m_Base(parent)
{
    setObjectName("Маршрут..");
    FGROUP=0;
    FSV1.clear();
    FSV2.clear();
}

bool m_Marsh::USTANOVLEN()
{
    if (lMarsh_Str.size()==0) return false;
    for (int i=0;i<lMarsh_Str.size();i++){
        if (!lMarsh_Str[i]->MPOL()) return false;
    }
    return true;
}

void m_Marsh::setUSTANOVLEN(bool p)
{
    if (p){
        for (int i=0;i<lMarsh_Str.size();i++){
            lMarsh_Str[i]->setMPOL();
            //lMarsh_Str[i]->setIN_MARSH(true);
        }
    } else {
        for (int i=0;i<lMarsh_Str.size();i++){
            //lMarsh_Str[i]->setIN_MARSH(false);
        }
    }
}

void m_Marsh::updateAfterLoad()
{
    m_Base::updateAfterLoad();
    const BaseObject *B=0;
    if (FSV1.id().num()!=0){
        B=superFindObjectById(this,FSV1.id());
        if (!B) qCritical() << objectName() << QString("Ошибочная ссылка SV1") <<endl ; else
        FSV1.linkObj(B);
    }
    if (FSV2.id().num()!=0){
        B=superFindObjectById(this,FSV2.id());
        if (!B) qCritical() << objectName() << QString("Ошибочная ссылка SV2") <<endl ; else
        FSV2.linkObj(B);
    }
    lMarsh_Str=findChildren<m_Marsh_Str*>();
}

void m_Marsh::addMSTR(m_RC *rc, MVP_Enums::TStrelPol pol)
{
    m_Marsh_Str * MSTR=new m_Marsh_Str(this);
    MSTR->setLINKRC(rc);
    MSTR->setPOL(pol);
    lMarsh_Str=findChildren<m_Marsh_Str*>();
}


void m_Marsh::updateStates()
{
    if (USTANOVLEN())
        setUSTANOVLEN(true);
}
