#ifndef CONDITIONACTITEM_H
#define CONDITIONACTITEM_H
/*
 *  Класс реализации скрипта <Условие> действие
*/



#include <QPointer>
#include "baseobject.h"
#include "formulaobjts.h"

struct TActStep{
    TActStep();
    QString ID_STR_A;
    QString STATE_A;
    QString V;
    QString ID_STR_V;
    QString STATE_V;
    QPointer<BaseObject> B_A;
    QPointer<BaseObject> B_V;
    bool ok{};
    void clear();
    void fromString(const QString& S,QObject* G, QStringList &lerrors);
    void updateV();

};

class ConditionActItem : public BaseObject
{
    Q_OBJECT
    Q_PROPERTY(QString  CONDITION READ CONDITION WRITE setCONDITION DESIGNABLE true)
    Q_PROPERTY(QString  ACT_RESET READ ACT_RESET WRITE setACT_RESET DESIGNABLE true)
    Q_PROPERTY(QString  ACT READ ACT WRITE setACT DESIGNABLE true)
    Q_PROPERTY(QString  ACT_ELSE READ ACT_ELSE WRITE setACT_ELSE DESIGNABLE true)

    Q_PROPERTY(int  STATE_CONDITION READ STATE_CONDITION WRITE setSTATE_CONDITION DESIGNABLE true STORED false)
    MYPROP(bool,GROUP_RESET)
public:
    // формат условия:
    // (IDSTR.STATE=V)&(IDSTR1.STATE1=IDSTR2.STATE2)
    QString CONDITION()const {return FCONDITION;}
    QString ACT_RESET()const {return FACT_RESET;}
    QString ACT()const {return FACT;}
    QString ACT_ELSE()const {return FACT_ELSE;}
    void setCONDITION(const QString& p);
    void setACT_RESET(const QString& p);
    void setACT(const QString& p);
    void setACT_ELSE(const QString& p);
    int STATE_CONDITION() const {return FSTATE_CONDITION;}
    void setSTATE_CONDITION(int p);
    void clear();
    bool error_CONDITION() {return condition.error;}
    bool error_ACT() {return !isOk_V(vACT);}
    bool error_ACT_RESET() {return !isOk_V(vACT_RESET);}
    bool error_ACT_ELSE() {return !isOk_V(vACT_ELSE);}


public:
    Q_INVOKABLE ConditionActItem(QObject *parent = nullptr);
    virtual ~ConditionActItem(){}
    virtual void validation(ListObjStr *l) const;
    QObject* G;
    QVector<TActStep*> vACT_RESET;
    QVector<TActStep*> vACT;
    QVector<TActStep*> vACT_ELSE;
    FormulaObjTs condition;

    void updatePropetyes();

protected:
    QString FCONDITION;
    QString FACT_RESET;
    QString FACT;
    QString FACT_ELSE;
    int  FSTATE_CONDITION;
    QVector<TActStep*> VfromS(QString S);
    bool isOk_V(QVector<TActStep*> &V);
    bool _updateStatesEntered;

    QStringList slerrors;

signals:


public slots:
    void slot_formulaPropertyChanged(QObject *);
};

#endif // CONDITIONACTITEM_H
