#include "m_marsh_list.h"

#include <QDebug>
#include "baseobjecttools.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(m_Marsh_List,"Маршрут список","Элементы модели ГОРКА")


m_Marsh_List::m_Marsh_List(BaseObject *parent):
    ModelGroup(parent)
{
    setObjectName("Список маршрутов");
}

void m_Marsh_List::updateAfterLoad()
{
    ModelGroup::updateAfterLoad();
    reBuildlMarsh_Str();
}


QList<m_Marsh *> m_Marsh_List::marshruts() const
{
    QList<m_Marsh*> lMarsh=findChildren<m_Marsh *>();
    return lMarsh;
}


void m_Marsh_List::clear_STATE_IN_MARSH()
{
    foreach (m_RC*RC, lRC) {
        //RC->setSTATE_IN_MARSH(false);
    }
}


void m_Marsh_List::updateStates()
{
    //сбросим новые
     QMap<m_RC*, bool > m_RCR2USTM1;
    foreach (m_RC*RC, lRC) {
        m_RCR2USTM1[RC]=false;
    }
    foreach (m_Marsh*M, marshruts()) {
        if (M->USTANOVLEN()){
            foreach (m_Marsh_Str*MSTR, M->marsh_Str()) {
                m_RCR2USTM1[MSTR->RC()]=true;
            }
        }
    }
    // запишем изменения

    foreach (m_RC*RC, lRC) {
        //RC->setSTATE_IN_MARSH(m_RCR2USTM1[RC]);
    }
}


void m_Marsh_List::reBuildlMarsh_Str()
{
    // собираем все уникальные стрелки в общий список
    lRC.clear();
    QList<m_Marsh_Str*> lMarsh_Str1;
    foreach (m_Marsh*M, marshruts()) {
        lMarsh_Str1.clear();
        lMarsh_Str1=M->findChildren<m_Marsh_Str*>();
        foreach (m_Marsh_Str*MSTR, lMarsh_Str1) {
            if (lRC.indexOf(MSTR->RC())<0)
                    lRC.push_back(MSTR->RC());
        }
    }
}
