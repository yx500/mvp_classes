#include "v_marsh.h"

#include "basevisualtools.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_Marsh,"VISUAL: Маршут","Стандартные")

v_Marsh::v_Marsh(v_Base *parent) : v_Model(parent)
{
    FSELECTED=false;
    setObjectName("Маршрут");
}

void v_Marsh::setSELECTED(bool p)
{
    QMapIterator<v_RC*,m_Marsh_Str*> i(mVRC2MSTR);
    while (i.hasNext()) {
        i.next();
        if (p){
            if (i.value()->POL()==MVP_Enums::TStrelPol::pol_plus)  {i.key()->setSTATE_SHTRIH_PLUS(true);i.key()->setSTATE_SHTRIH_MINUS(false);}
            if (i.value()->POL()==MVP_Enums::TStrelPol::pol_minus) {i.key()->setSTATE_SHTRIH_PLUS(false);i.key()->setSTATE_SHTRIH_MINUS(true);}
            if (i.value()->POL()==MVP_Enums::TStrelPol::pol_w)     {i.key()->setSTATE_SHTRIH_PLUS(true);i.key()->setSTATE_SHTRIH_MINUS(true);}
        } else {
            i.key()->setSTATE_SHTRIH_PLUS(false);i.key()->setSTATE_SHTRIH_MINUS(false);
        }
    }
}

void v_Marsh::updateAfterLoad()
{
    // создаются динамически
    //    v_Model::updateAfterLoad();
//    update_rcList();
}


void v_Marsh::update_rcList()
{
    mVRC2MSTR.clear();
    QList<m_Marsh_Str*> lmstr=marsh()->marsh_Str();
    foreach (m_Marsh_Str*mstr, lmstr) {
        QList<v_Model *> lvm=stationFindElementsByModel(mstr->RC());
        v_RC *vrc=0;
        foreach (v_Model *vm, lvm) {
            vrc=qobject_cast<v_RC*>(vm);
            if (vrc) break;
        }
        if (!vrc){
            qCritical() << objectName() << QString("Нет РЦ для стрелки маршрута ") <<mstr->RC()->idstr() << endl ;
        } else {
            mVRC2MSTR[vrc]=mstr;
        }
    }
}

