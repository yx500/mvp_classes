#include "modelrootgroup.h"
#include "m_base.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(ModelRootGroup,"Корневая группа моделей","БАЗОВЫЕ")

ModelRootGroup::ModelRootGroup(BaseObject *parent) : ModelGroup(parent)
{

}

bool ModelRootGroup::isCanAddObject(QObject *O) const
{
    if (qobject_cast<m_Base*>(O)) return true;
    if (qobject_cast<ModelGroup*>(O)) return true;
    return false;
}

void ModelRootGroup::addObject(QObject *O)
{
    // кладем в нужную группу
    m_Base*M=qobject_cast<m_Base*>(O);
    if (M) {
        QString group=M->defaultGroupName();
        if (!group.isEmpty()){
            ModelGroup *MG=findChild<ModelGroup *>(group, Qt::FindDirectChildrenOnly);
            if (!MG){
                MG=new ModelGroup(this);
                MG->setObjectName(group);
            }
            M->setParent(MG);
        }
    } else {
        ModelGroup*MG=qobject_cast<ModelGroup*>(O);
        if (MG) {
            MG->setParent(this);
        }
    }
}




//void ModelRootGroup::reSortUpdateStates()
//{

//    // раскидываем по моделям и не модели
//    l_m_Base.clear();
//    l_m_Base=findChildren<m_Base *>();
//    // сортируем по workLevel
//    qSort(l_m_Base.begin(), l_m_Base.end(), workLevelLessThan);
//}


//void ModelRootGroup::work(const QDateTime &T, int step)
//{
//    if (l_m_Base.isEmpty()) reSortUpdateStates();
//    // убираем множественное срабатывание если изменилось более одного состояния
//    foreach(m_Base *M,l_m_Base){
//        M->setStateChangedEmit(false);
//        M->setStateChanged(false);
//        M->work(T,step);
//        M->setStateChangedEmit(true);
//        if (M->isStateChanged()) M->emitStateChanged();
//    }
//}

