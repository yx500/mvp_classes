#ifndef M_MARSH_LIST_H
#define M_MARSH_LIST_H

#include "modelgroup.h"
#include "m_marsh.h"

class m_Marsh_List :public ModelGroup
{
        Q_OBJECT
public:
    Q_INVOKABLE m_Marsh_List(BaseObject *parent = 0);
    virtual ~m_Marsh_List(){}
    virtual QString defaultGroupName() const {return "";}
    virtual QString defaultVisualElement() const {return "v_Marsh_List";}
    virtual void updateAfterLoad();

    QList<m_Marsh *> marshruts() const ;
    void clear_STATE_IN_MARSH();
    void reBuildlMarsh_Str();



signals:

public slots:
    virtual void updateStates(); // основной жизненный цикл
protected:
    QList<m_RC*> lRC;
};

#endif // M_MARSH_LIST_H
