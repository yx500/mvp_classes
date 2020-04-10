#ifndef MODELROOTGROUP_H
#define MODELROOTGROUP_H

#include "modelgroup.h"

// Класс контейнер для всех моделей
// создает сортированный список для updateStates,work
class LIBSHARED_EXPORT ModelRootGroup : public GroupBaseObjects
{
    Q_OBJECT
public:
    Q_INVOKABLE ModelRootGroup(BaseObject *parent = nullptr);
    virtual ~ModelRootGroup(){}

    virtual bool isCanAddObject(QObject *O) const;
    virtual void addObject(QObject *O) ;
    virtual void updateAfterLoad();

signals:

protected:
//    QList<m_Base *> l_m_Base;
//    void reSortUpdateStates();
};

#endif // MODELROOTGROUP_H
