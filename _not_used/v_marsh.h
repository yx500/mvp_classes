#ifndef V_MARSH_H
#define V_MARSH_H

#include "m_marsh.h"
#include "v_model.h"
#include "v_rc.h"

class v_Marsh : public v_Model
{
    Q_OBJECT
    Q_PROPERTY(bool SELECTED READ SELECTED WRITE setSELECTED DESIGNABLE true STORED false)
public:
    Q_INVOKABLE v_Marsh(v_Base *parent = 0);
    virtual ~v_Marsh(){}

    bool SELECTED() const {return FSELECTED;}
    void setSELECTED(bool p);

    virtual bool isStoredXML() const{return false;}
    virtual void updateAfterLoad();

    m_Marsh *marsh() const {return qobject_cast<m_Marsh*>(modelObject());}

    QList<v_RC*> rcList() const;
    void update_rcList();

signals:

public slots:
protected:
    bool FSELECTED;
    QRectF R;
    virtual QRectF d_boundingRect() const {return QRectF();}
    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option) {Q_UNUSED(painter);Q_UNUSED(option);}

private:
    QMap<v_RC*,m_Marsh_Str*> mVRC2MSTR;

};

#endif // V_MARSH_H
