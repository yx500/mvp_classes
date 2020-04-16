#ifndef GTBUFFER_H
#define GTBUFFER_H

#include <QObject>
#include <QDateTime>
#include "libshared_global.h"



class LIBSHARED_EXPORT GtBuffer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int TYPE READ getType WRITE setType DESIGNABLE true )

    Q_PROPERTY(int SIZEDATA READ sizeData WRITE setSizeData DESIGNABLE true )
    Q_PROPERTY(int LIVEDT READ getMsecPeriodLive WRITE setMsecPeriodLive DESIGNABLE true )
    Q_PROPERTY(quint64 TICK READ getTick DESIGNABLE true )

public:
    void setType(int p){type=p;}
    int getType() const {return type;}
    void setSizeData(int p){A.resize(p);}
    int sizeData() const {return A.size();}
    void setName(QString p){name=p;setObjectName(p);}
    QString getName() const {return name;}
    void setMsecPeriodLive(int p){msecPeriodLive=p;}
    int getMsecPeriodLive() const  {return msecPeriodLive;}
    quint64 getTick() const {return tick;}
    QDateTime dataRecivedTime() const {return timeDataRecived;}
    QDateTime dataChangedTime()const  {return timeDataChanged;}
    void setData(const char *d, int size) {A=QByteArray::fromRawData(d,size);}
public:
    enum GtBuffersStates{
        _unknow=0,
        _alive=1,
        _error=-1
    };
    explicit GtBuffer(QObject *parent = nullptr):QObject(parent){
        type=0;
        name=QString();
        sost=_unknow;
        timeDataRecived=QDateTime();
        timeDataChanged=QDateTime();
        tick=0;
        A.clear();
        A.resize(1000);
        A.fill(0);
        msecPeriodLive=0;
    }

    virtual ~GtBuffer(){}
    int type;
    QString name;
    int sost;
    quint64 tick;
    QDateTime timeDataRecived;
    QDateTime timeDataChanged;
    QByteArray A;
    int msecPeriodLive;
    bool static_mode=false; // признак что буффер не для обмена
    void assign(const GtBuffer *B){
        type=B->type;
        name=B->name;
        msecPeriodLive=B->msecPeriodLive;
        assign_data(B);
    }
    void assign_data(const GtBuffer *B){
        sost=B->sost;
        tick=B->tick;
        timeDataRecived=B->timeDataRecived;
        timeDataChanged=B->timeDataChanged;
        A=B->A;
    }
    void clear(){
        type=0;
        name=QString();
        sost=_unknow;
        tick=0;
        timeDataRecived=QDateTime();
        timeDataChanged=QDateTime();
        A.clear();
    }
    void clearData(){
        sost=_unknow;
        tick=0;
        timeDataRecived=QDateTime();
        timeDataChanged=QDateTime();
        A.clear();
    }
    void emitBufferChanged(){
        emit bufferChanged(this);
    }
signals:
    void bufferChanged(GtBuffer *B);
protected:

};

class IGtBufferInterface :public QObject{
    public:
    IGtBufferInterface(QObject *parent = nullptr):QObject(parent){}
    virtual ~IGtBufferInterface();
    virtual GtBuffer *getGtBuffer(int type,const QString &name)=0;
    virtual int sendGtBuffer(const GtBuffer *B)=0;
    virtual int maxSizeData()=0;
};



#endif // GTBUFFER_H
