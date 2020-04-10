#ifndef OBJECTLINK_H
#define OBJECTLINK_H

#include "baseobject.h"
#include <QPointer>

class ObjectLink
{
    public:

    quint64 storedFid;


    ObjectLink(){storedFid=0;O=nullptr;FisNotUsed=false;}
    ObjectLink(const ObjectLink &other){
        storedFid=other.storedFid;O=other.obj();FisNotUsed=other.isNotUse();
    }
    ObjectLink(QObject *o){
        FisNotUsed=false;
        linkObj(o);
    }
    ~ObjectLink(){}

    QString toString(){
        if (FisNotUsed) return "";
        return QString("%1").arg(storedFid);
    }
    void fromValue(QString s){
        storedFid=0;
        if (s.isEmpty()){FisNotUsed=true;} else {FisNotUsed=false;storedFid=str2objectId(s);}
        O=nullptr;
    }
    QObject * obj()const {return O;}

    BaseObject * baseObject()const {
        QObject *o=O;
        return qobject_cast<BaseObject *>(o);
    }
    const quint64 &id() {return storedFid;}
    bool isNotUse() const {return FisNotUsed;}
    void setInNotUse(bool p){FisNotUsed=p;}
    bool isNull() const {return O.isNull();}
    bool isEmpty() const {return storedFid==0;}

    void unLinkObj(){
            O=nullptr;storedFid=0;
    }

    void linkObj(QObject* o){
        BaseObject *B=qobject_cast<BaseObject *>(o);
        if (B!=nullptr){
            O=o;
            storedFid=B->id();
        } else {
            O=nullptr;
            storedFid=0;
        }
    }
    void linkObj(const BaseObject* B){
        if (B!=nullptr){
            O=const_cast<BaseObject*>(B);
            storedFid=B->id();
        } else {
            O=nullptr;
            storedFid=0;
        }
    }
    void clear(){unLinkObj();}
    bool like(const ObjectLink &other){
        return storedFid==other.storedFid;
    }
    void setId(quint64 newId){storedFid=newId;}

    bool	operator == (const ObjectLink &other) const {
        if (O!=nullptr) {
            return O==other.obj();
        } else {
            return storedFid==other.storedFid && (FisNotUsed==other.isNotUse());
        }
    }
    bool	operator != (const ObjectLink &other) const {
        if (O!=nullptr) {
            return O!=other.obj();
        } else {
            return storedFid!=other.storedFid || (FisNotUsed!=other.isNotUse());
        }
    }
protected:
    QPointer<QObject> O;
    bool FisNotUsed;

};
Q_DECLARE_METATYPE(ObjectLink)

#endif // OBJECTLINK_H
