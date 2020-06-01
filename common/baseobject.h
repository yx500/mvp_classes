#ifndef BASEOBJECT_H
#define BASEOBJECT_H


#include <QObject>
#include <QMap>
#include <QVariant>
#include <QVector>
#include <QPointer>

#include "mvp_define.h"
#include "mvp_enums.h"
#include "libshared_global.h"
#include "listobjstr.h"
#include "signaldescription.h"


QString objectId2Str(const quint64 &id);
quint64 str2objectId(const QString &s);

// Базовый объект :
// основа всей библиотеки классов
// имеет ID - уникальный всегда
// IDSTR для использования в скриптах - уникальный в группе
// имеет свойства - сохраняются и загружаются
// и состояния - не сохраняются
// обмен с внешним миром через SignalDescription
// должен однозначно определять свое сотояние функцией updateStates()


class LIBSHARED_EXPORT BaseObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint64 ID READ id WRITE setid DESIGNABLE true)
    Q_PROPERTY(QString  IDSTR READ idstr WRITE setidstr DESIGNABLE true)
    Q_PROPERTY(QString  TYPENAME READ TYPENAME DESIGNABLE true STORED false)


public:

    explicit BaseObject(QObject *parent = nullptr);
    virtual ~BaseObject(){}
    QString TYPENAME() const {return metaObject()->className();}
    // уникальный везде, основа для линков
    void setid(const quint64 &otherId){FId=quint64(otherId);}
    const quint64 &id() const {return FId;}

    // допольнительный ID строковой
    virtual void setidstr(QString p){SETPROP(Fidstr);}
    virtual QString  idstr() const {return Fidstr;}
    BaseObject * baseObjectById(const quint64 &id) const;

    virtual bool isCanAddObject(QObject *O) const {Q_UNUSED(O);return false;}
    virtual void addObject(QObject *O) {O->setParent(this);}

    // определяет сохранять или нет
    virtual bool isStoredXML() const{return true;}

    // восстановление параметров после загрузки(линки)
    virtual void updateAfterLoad();
    QString XMLfile() const {return xmlFile;}
    void setXMLfile(QString p){xmlFile=p;}

    // самопроверка
    virtual void validation(ListObjStr *l) const {Q_UNUSED(l);}

    void emitPropertyChanged(){emit propertyChanged(this);}
    void emitStateChanged(){emit stateChanged(this);}
    virtual void resetStates();

    void setTag(int key, QVariant val);
    QVariant getTag(int key) const;

    void addTagObject(QObject *ob,int key=0);
    void delTagObject(QObject *ob);
    QObjectList tagObjects() const;
    QObject* tagObject(int key) const;

    bool disableUpdateStates=false;
    virtual void updateStates(); // основной жизненный цикл
    void _prepare_updateStates();
    void _emit_after();
    quint64 stateChangedCount() const {return _stateChangedCount;}


public slots:



protected:
    quint64 FId;
    QString Fidstr;
    QString xmlFile;
    QVector<QPointer<QObject> >vTagObjects;
    QMap<int,QPointer<QObject>> mTagObjects;


    virtual void doPropertyChanged();
    virtual void doStateChanged();
    void validationEmptySignals(ListObjStr *l) const;
    void validationEmptyLinks(ListObjStr *l) const;

    QMap<int,QVariant> mTags;

private:
    bool onlyOneEmitEnabled=false;
    bool isStateChanged=false;
    quint64 _stateChangedCount=0;

signals:
    void propertyChanged(QObject *O);
    void stateChanged(QObject *O);


};





#endif // BASEOBJECT_H
