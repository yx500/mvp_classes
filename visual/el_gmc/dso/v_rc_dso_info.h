#ifndef V_RC_DSO_INFO_H
#define V_RC_DSO_INFO_H

#include "v_model.h"

class m_RC;
class m_RC_DSO;
class m_STREL_DSO_DBK;
class m_RC_ZKR_DSO;

class v_RC_DSO_INFO : public v_Base
{
    Q_OBJECT
    Q_PROPERTY(ObjectLink   LNKMODEL READ LNKMODEL WRITE setLNKMODEL DESIGNABLE true)
    Q_PROPERTY(int     TEXTH READ TEXTH WRITE setTEXTH DESIGNABLE true)
public:
    void setTEXTH(int p);
    int TEXTH(){return FTEXTH;}

public:
    Q_INVOKABLE v_RC_DSO_INFO(v_Base *parent = 0);
    virtual ~v_RC_DSO_INFO(){}

    ObjectLink LNKMODEL() const;
    void setLNKMODEL(ObjectLink p);
    virtual void updateAfterLoad();// восстановление параметров после загрузки(линки)


    virtual void d_calculateGeometry(); // перестраивает внтренние координаты

signals:

public slots:
    void slotModelStateChanged(QObject *O);

protected:
    ObjectLink FLNKMODEL;
    int FTEXTH;
    qreal RTEXTH;
    QRectF  boundrect_text;
    QString get_DSO_RC_Text();
    bool get_STATE_DSO_ERROR();
    int getOsyCount() const;
    int lastOsyCount;
    m_RC_DSO *mDSO_RC;
    m_STREL_DSO_DBK *mDSO_Strel;
    m_RC_ZKR_DSO *mRC_ZKR_DSO;
    QDateTime dtStartShowText;

    virtual QRectF d_boundingRect() const;
    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
private:
    QPointF M[1];
    m_RC * DSO_RC;
};

#endif // V_RC_DSO_INFO_H
