#ifndef VISUALROOTGROUP_H
#define VISUALROOTGROUP_H

#include "visual_screen.h"
#include "visual_elements.h"

/*
 * Контейнер для классов визуализации
 * */

class LIBSHARED_EXPORT VisualRootGroup : public BaseObject
{
    Q_OBJECT
public:
    MYPROP(QString,  PALETTE_FILENAME)

public:
    Q_INVOKABLE VisualRootGroup(QObject *parent = nullptr);
    virtual ~VisualRootGroup(){}
    virtual void updateAfterLoad();

    virtual bool isCanAddObject(QObject *O) const;

signals:

public slots:

protected:
};

#endif // VISUALROOTGROUP_H
