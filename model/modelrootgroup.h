#ifndef MODELROOTGROUP_H
#define MODELROOTGROUP_H

#include "modelgroup.h"

// Класс контейнер для всех моделей
class LIBSHARED_EXPORT ModelRootGroup : public ModelGroup
{
    Q_OBJECT
public:
    Q_INVOKABLE ModelRootGroup(BaseObject *parent = nullptr);
    virtual ~ModelRootGroup(){}

    virtual bool isCanAddObject(QObject *O) const;
    virtual void addObject(QObject *O) ;

signals:

protected:
};

#endif // MODELROOTGROUP_H
