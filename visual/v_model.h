#ifndef V_MODEL_H
#define V_MODEL_H

#include "v_base.h"
#include "m_base.h"
#include "objectlink.h"
#include <QMetaProperty>


#define SET_STATE_MODEL_BOOL(n) set##n(modelPropertyBool(#n));
#define SET_STATE_MODEL_INT(n) set##n(modelPropertyInt(#n));
#define SET_STATE_MODEL_QREAL(n) set##n(modelPropertyQreal(#n));

#define MODEL_STATE(type,n) Q_PROPERTY(type n READ n WRITE set##n DESIGNABLE true STORED false) \
                       protected: \
                       QMetaProperty metaProperty_##n; \
                       type F##n; \
                       public: \
                       void set##n(const type &p){ \
                            if(F##n!=p){ \
                                F##n=p; \
                                doStateChanged();}} \
                       type n()const {return F##n;}

class LIBSHARED_EXPORT v_Model : public v_Base
{
    Q_OBJECT
    Q_PROPERTY(ObjectLink   LNKMODEL READ LNKMODEL WRITE setLNKMODEL DESIGNABLE true)



public:
    MYSTATE(bool,STATE_33)

    explicit v_Model(QObject *parent = nullptr);
    virtual ~v_Model(){}

    virtual void resetStates();

    virtual void setid(quint64 id){Q_UNUSED(id);}
    virtual quint64 id() const;

    virtual void setidstr(QString p){Q_UNUSED(p)}
    virtual QString  idstr() const ;
    ObjectLink LNKMODEL() const;
    void setLNKMODEL(ObjectLink p);
    virtual void setModelObject(BaseObject *B);
    m_Base *modelObject() const;
    QVariant modelProperty(const QString &propertyName) const;

    virtual void updateAfterLoad();// восстановление параметров после загрузки(линки)

    virtual void updateStates(); // основной жизненный цикл

public slots:
    virtual void slotModelPropertyChanged(QObject *O);  // следим за моделью
    virtual void slotModelStateChanged(QObject *O);  // следим за моделью

    virtual void updateStatesFromModel();

protected:
    ObjectLink FLNKMODEL;
    bool isNotAccepted33(QColor &clrB);
    quint64 _modelStateChangedCount=0;
    bool modelStateChanged();
    bool    modelPropertyBool(QString propertyName,bool defaultValue=false) const;
    int     modelPropertyInt(QString propertyName, int defaultValue=0) const;
    qreal   modelPropertyQreal(QString propertyName, qreal defaultValue=0) const;
    QString modelPropertyesStr(QString propertyName, QString defaultValue) const;
    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option);
    struct _tCommonProperty{
        QMetaProperty MP;
        QMetaProperty VP;
    };

    QVector<_tCommonProperty> vCommonPropertyes;





};

#endif // V_MODEL_H
