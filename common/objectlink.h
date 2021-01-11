#ifndef OBJECTLINK_H
#define OBJECTLINK_H

#include "baseobject.h"
#include <QPointer>

class ObjectLink
{
    public:

    quint64 storedFid;


    ObjectLink(){storedFid=0;B=nullptr;FisNotUsed=false;}
    ObjectLink(const ObjectLink &other){
        storedFid=other.storedFid;B=other.baseObject();FisNotUsed=other.isNotUse();
    }
//    ObjectLink(QObject *o){
//        FisNotUsed=false;
//        linkObj(o);
//    }
    ObjectLink(BaseObject *o){
        FisNotUsed=false;
        linkObj(o);
    }
    ObjectLink(const QString &s){
        fromString(s);
    }

    QString toString() const{
        if (FisNotUsed) return "";
        return QString("%1").arg(storedFid);
    }
    void fromString(QString s){
        storedFid=0;
        if (s.isEmpty()){FisNotUsed=true;} else {FisNotUsed=false;storedFid=str2objectId(s);}
        B=nullptr;
    }

    static ObjectLink _fromString(const QString &s)    {
        return ObjectLink(s);
    }

    static QString _toString(const ObjectLink &l)    {
        return l.toString();
    }


    BaseObject * baseObject()const {
        return B;
    }
    const quint64 &id() {return storedFid;}
    bool isNotUse() const {return FisNotUsed;}
    void setInNotUse(bool p){FisNotUsed=p;}
    bool isNull() const {return B.isNull();}
    bool isEmpty() const {return storedFid==0;}

    void linkObj(const BaseObject* B){
        this->B=const_cast<BaseObject*>(B);
        if (B!=nullptr){
            storedFid=B->id();
        } else {
            //storedFid=0;
        }
    }
    void clear(){B=nullptr;;storedFid=0;}
    bool like(const ObjectLink &other){
        return storedFid==other.storedFid;
    }
    void setId(quint64 newId){storedFid=newId;}

    bool	operator == (const ObjectLink &other) const {
            return storedFid==other.storedFid && (FisNotUsed==other.isNotUse());
    }
    bool	operator != (const ObjectLink &other) const {
            return storedFid!=other.storedFid || (FisNotUsed!=other.isNotUse());
    }
protected:
    QPointer<BaseObject> B;
    bool FisNotUsed;

};


Q_DECLARE_METATYPE(ObjectLink)

#endif // OBJECTLINK_H
