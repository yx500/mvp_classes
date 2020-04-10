#ifndef MODELGROUP_H
#define MODELGROUP_H

#include "groupbaseobjects.h"
#include "m_base.h"


// Класс контейнер для всех моделей определенной группы, сделан для удобства

class ModelGroup : public GroupBaseObjects
{
    Q_OBJECT
public:
    Q_INVOKABLE ModelGroup(BaseObject *parent = nullptr);
    virtual ~ModelGroup(){}
    // рут модель создает сорт список, заглушаем

signals:

public slots:
};

#endif // MODELGROUP_H
