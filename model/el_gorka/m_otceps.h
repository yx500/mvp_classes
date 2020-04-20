#ifndef M_OTCEPS_H
#define M_OTCEPS_H

#include "m_otcep.h"


// контоллирует список отцепов
// увязывает отцепы с моделью поля

class m_ControllerARS;
class m_Otceps : public m_Base
{
    Q_OBJECT
    MYPROP(int,TYPE_DESCR)

public:
    Q_INVOKABLE m_Otceps(QObject *parent = nullptr);
    virtual ~m_Otceps();
    virtual void resetStates();
    virtual QString defaultGroupName() const {return "ОТЦЕПЫ";}
    virtual void validation(ListObjStr *l) const;
    virtual bool isStoredXML() const{return true;}
    virtual void updateAfterLoad();
    QList<m_Otcep*> otceps() const;
    m_Otcep * otcepByNum(int n);
    m_Otcep * otcepOnRc(m_RC* rc);
    m_Otcep *otcepADDR_SLOT(int ADDR_SLOT,int ADDR,int NTP);
    m_RC* find_RC(int chanelOffset);
    void set_lrc(QList<m_RC*> &l);

    QList<m_Otcep *> l_otceps;

    m_Otcep *topOtcep() const;
    QMap<int,int> mSP2MAR;
    QMap<int,int> mMAR2SP;

    virtual void updateStates();




public slots:
    void updateVagons();


protected:
    GtBuffer * chanelVag[MaxVagon];

    QList<m_RC *> l_rc;
    QMap<int,m_RC *> mOffset2Rc;
    QMap<QString,m_RC *> mIDS2Rc;




};

#endif // M_OTCEPS_H
