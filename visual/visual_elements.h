#ifndef VISUAL_ELEMENTS_H
#define VISUAL_ELEMENTS_H
/*
 * список элементов отображения
 * */

#include "baseobject.h"
#include "v_base.h"

class v_Model;
class  Visual_Elements : public v_Base
{
    Q_OBJECT
public:

    Q_INVOKABLE Visual_Elements(QObject *parent = nullptr);
    virtual ~Visual_Elements(){}

    virtual void updateAfterLoad();
    void reCalculateGeometry();
    QList<v_Model *> findElementsByModel(BaseObject *M);
    virtual bool isCanAddObject(QObject *O) const;

protected:
public slots:
    virtual void updateStates(); // основной жизненный цикл
};

#endif // VISUAL_ELEMENTS_H
