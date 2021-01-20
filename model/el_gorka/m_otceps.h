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
    MYPROP(SignalDescription,SIGNAL_DATA_VAGON_0)

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
    m_Otcep *otcepADDR_SLOT(int ADDR_SLOT,int ADDR,int NTP);
    m_RC* find_RC(int chanelOffset);

    QList<m_Otcep *> l_otceps;

    QList<m_Otcep *> enabledOtceps() const;

    m_Otcep *topOtcep() const;
    bool isFirstOtcep(m_Otcep *otcep);



    virtual void updateStates();


    QMap<int,m_RC *> mOffset2Rc;
    QMap<QString,m_RC *> mIDS2Rc;
    ModelGroupGorka *modelGroupGorka=nullptr;
    SignalDescription chanelVag[MaxVagon];
    tSlVagon vagons[MaxVagon];

public slots:
    void updateVagons();


protected:

    QByteArray _storedVagonsA[MaxVagon];
    QList<m_RC *> l_rc;




};
QVariantHash tSlVagon2Map(const tSlVagon &v);
tSlVagon Map2tSlVagon(const QVariantHash &m);

#endif // M_OTCEPS_H
