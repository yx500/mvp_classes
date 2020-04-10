#include "m_marsh_str.h"

#include <QDebug>
#include "baseobjecttools.h"


#include "mvp_objectfactory.h"
REGISTERELEMENT(m_Marsh_Str,"Маршрут стрелка","Элементы модели ГОРКА")

m_Marsh_Str::m_Marsh_Str(QObject *parent) :
    m_Base(parent)
{
    FPOL=MVP_Enums::TStrelPol::pol_plus;
    FRC=0;
    FLINKRC.clear();
}

void m_Marsh_Str::updateAfterLoad()
{
    m_Base::updateAfterLoad();
    const BaseObject *B=0;
    if (FLINKRC.id().num()!=0){
        B=superFindObjectById(this,FLINKRC.id());
    }
    if (!B){
            qCritical() << objectName() << QString("Ошибочная ссылка LINKRC ") <<endl ;
    }
    FLINKRC.linkObj(B);
    setLINKRC(FLINKRC);
}

bool m_Marsh_Str::MPOL()
{
    if (FRC) return FRC->STATE_POL()==FPOL;
    return false;
}

void m_Marsh_Str::setMPOL()
{
    if (FRC)  FRC->setSTATE_POL(FPOL);
}

void m_Marsh_Str::setIN_MARSH(bool p)
{
    //if (FRC)  FRC->setSTATE_IN_MARSH(p);
}



void m_Marsh_Str::updateStates()
{

}

void m_Marsh_Str::setLINKRC(ObjectLink p)
{
    if (FLINKRC!=p){
        FLINKRC=p;
    }
    FRC=qobject_cast<m_RC *>(p.obj());
    if (FRC) {
        setObjectName(FRC->idstr());
        setidstr(QString("СТРМ_%1").arg(FRC->idstr()));
    }
}
