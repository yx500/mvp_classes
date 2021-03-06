#ifndef M_OTCEPS_H
#define M_OTCEPS_H

#include "m_otcep.h"


// контоллирует список отцепов
// увязывает отцепы с моделью поля

class m_ControllerARS;
class ModelGroupGorka;
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
    const QList<m_Otcep *> &otceps() const;
    m_Otcep * otcepByNum(int n);
    m_Otcep * otcepOnRc(m_RC* rc);
    int otcepCountOnRc(m_RC* rc);
    QList<m_Otcep *>otcepsOnRc(m_RC* rc);
    m_Otcep *otcepADDR_SLOT(int ADDR_SLOT,int ADDR,int NTP);
    m_RC* find_RC(int chanelOffset) const ;
    m_RC* find_RC_ID(quint64  id) const;

    QList<m_Otcep *> l_otceps;
    QList<m_Vagon *> l_vagons;

    QList<m_Otcep *> enabledOtceps() const;

    m_Otcep *topOtcep() const;
    bool isFirstOtcep(m_Otcep *otcep);



    virtual void updateStates();

    void otceps2Vagons();
    void vagons2Otceps();


    QMap<int,m_RC *> mOffset2Rc;
    QMap<quint64,m_RC *> mID2Rc;
    ModelGroupGorka *modelGroupGorka=nullptr;

public slots:



protected:

    QByteArray _storedVagonsA[MaxVagon];
    QList<m_RC *> l_rc;




};


#endif // M_OTCEPS_H
