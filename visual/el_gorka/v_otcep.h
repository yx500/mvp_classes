#ifndef V_OTCEP_H
#define V_OTCEP_H

#include "v_model.h"

// отображение отцепа
// жестко прицеплен к m_otcep
class m_Otcep;
class v_RC;
class v_Otcep : public v_Model
{
    Q_OBJECT
public:
public:
    explicit v_Otcep(QObject *parent = nullptr);
    virtual ~v_Otcep(){}
    virtual bool isStoredXML() const{return false;}
    virtual void setModelObject(BaseObject *B);
    virtual void validation(ListObjStr *l) const;

    virtual void resetStates();
    virtual void updateStates();
    virtual void calculateGeometry(); // перестраивает внтренние координаты

    int number;
    bool exists;
    MVP_Enums::TDirect direct;
    QColor color;
    int width;
    int height;


public slots:
protected:
    m_Otcep *mOtcep;
    v_RC* target;
    QPolygonF targetLine;

    QPointF M[1];
    QPolygonF P;
    QRectF R;
    QRectF textR;
    QPolygonF PP;
    QList<QPolygonF> lPP;

    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void updateStatesFromModel();
};

#endif // V_OTCEP_H
