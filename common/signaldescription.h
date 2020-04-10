#ifndef SIGNALDESCRIPTION_H
#define SIGNALDESCRIPTION_H

/*
 * структура описания сигнала
 * */

#include "qstring.h"
#include <QStringList>
#include <QDateTime>
#include <qmetatype.h>

#include "gtbuffer.h"



class SignalDescription
{
public:


    SignalDescription();
    SignalDescription(const SignalDescription &other);
    SignalDescription(int type,const QString &name,int offset);

    IGtBufferInterface *GtBufferInterface() const;


    ~SignalDescription(){}
    QString chanelName() const {return QString(FChanelName);}
    void setChanelName(QString p);
    int    chanelType() const {return FChanelType;}
    void setChanelType(int p);
    int    chanelOffset() const {return FChanelOffset;}
    void setChanelOffset(int p){FChanelOffset=p;}
    bool   isInvers() const {return FisInvers;}
    void setIsInvers(bool p){FisInvers=p;}
    bool   isNotUse() const {return FisNotUse;}
    void setIsNoUse(bool p){FisNotUse=p;}
    bool   isInnerUse() const {return FisInnerUse;}
    void setIsInnerUse(bool p){FisInnerUse=p;}
    SignalDescription  innerUse() const ;
    QString storedSignalName()const;
    void setStoredSignalName(QString p);
    bool value_1bit() const;
    quint8 value_1byte() const;

    const char *value_data(int sz) const;
    const char *value_data(int offset,int sz) const;
    void acceptGtBuffer();
    GtBuffer *getBuffer() const;

    bool is33() const;

    void setSignalTS(QString chanelName,int chanelOffset);

    void setValue_1bit(bool v);
    void setValue_1byte(quint8 v);
    void setValue_2byte(int v);
    void setValue_4byte(quint32 v);
    void setData(const void * v,int sz);

    static GtBuffer *getBuffer(int type,QString name);
    static bool value_1bit(GtBuffer *B,int chanelOffset);
    static void setbit8(quint8* buf, size_t pos, int val = 1);


    bool isEmpty() const {
        if ((FChanelName.isEmpty())&&(FChanelOffset==0)) return true;
        return false;
    }

    bool isUsed() const { return (!isEmpty() && !FisNotUse);}

    void clear();
    QString toString() const;
    void fromString(QString s);

    bool	operator == (const SignalDescription &other) const {
        return (FChanelName==other.chanelName() &&
                    FChanelType==other.chanelType() && (FChanelOffset==other.chanelOffset()) && FisInvers==other.isInvers() && FisNotUse==other.isNotUse());
    }
    bool	operator != (const SignalDescription &other) const {
        return (FChanelName!=other.chanelName()) ||
                    ( FChanelType!=other.chanelType() || FChanelOffset!=other.chanelOffset() || FisInvers!=other.isInvers()  ||FisNotUse!=other.isNotUse() );
    }

private:
    int    FChanelType=0;
    QString   FChanelName="";
    int   FChanelOffset=0;
    bool   FisInvers=false;
    bool   FisNotUse=false;
    bool   FisInnerUse=false;
    QString   FStoredSignalName="";
    GtBuffer *gtBuffer;
    const char *chanelData(int &sz) const;
};

Q_DECLARE_METATYPE(SignalDescription)
IGtBufferInterface *setDefaultGetGtBufferInterface(IGtBufferInterface *i);


#endif // SIGNALDESCRIPTION_H
