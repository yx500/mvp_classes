#ifndef V_MARSH_LIST_H
#define V_MARSH_LIST_H

#include "m_marsh_list.h"
#include "v_model.h"
#include "v_marsh.h"

class v_Marsh_List : public v_Model
{
    Q_OBJECT
public:
    Q_INVOKABLE v_Marsh_List(v_Base *parent = 0);
    virtual ~v_Marsh_List(){}

    virtual void updateAfterLoad();
    virtual void calculateGeometry();
    v_Marsh *addMarsh();
    void delMarsh(v_Marsh *M);
    void clear();
    void updateModel();
    QList<v_Marsh *> marshruts() const ;
    void clearSELECTED();
    m_Marsh_List * marsh_List(){return qobject_cast<m_Marsh_List *>(modelObject());}



signals:

public slots:
protected:
    QRectF R;
    virtual QRectF d_boundingRect() const;
    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    void updateFromModel();

private:

};

#endif // V_MARSH_LIST_H
