#ifndef M_RC_GOR_ZAM_H
#define M_RC_GOR_ZAM_H

#include "m_rc_gor.h"
#include "m_zam.h"
class m_RC_Gor_Zam : public m_RC_Gor
{
    Q_OBJECT
    MYPROP(ObjectLink, ZAM)
public:
    Q_INVOKABLE m_RC_Gor_Zam(QObject *parent = nullptr);
    virtual ~m_RC_Gor_Zam(){}
    virtual void validation(ListObjStr *l) const;
    virtual void updateAfterLoad();
    m_Zam * zam()const {return _zam;}
signals:
public slots:
protected:

    m_Zam * _zam;
};

#endif // M_RC_GOR_ZAM_H
