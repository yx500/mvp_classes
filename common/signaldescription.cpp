#include "signaldescription.h"
#include "mvp_system.h"



static IGtBufferInterface *DefaultGtBufferInterface=nullptr;
IGtBufferInterface *setDefaultGetGtBufferInterface(IGtBufferInterface *i)
{
    IGtBufferInterface * _i=DefaultGtBufferInterface;
    DefaultGtBufferInterface=i;
    return _i;
}

SignalDescription::SignalDescription()
    :FChanelType(0),FChanelName(),FChanelOffset(0),
    FisInvers(false),FisNotUse(false),FisInnerUse(false),FStoredSignalName()
{
       gtBuffer=nullptr;
}

SignalDescription::SignalDescription(const SignalDescription &other)
{
    FChanelName=other.chanelName();
    FStoredSignalName=other.storedSignalName();
    FChanelType=other.chanelType();
    FChanelOffset=other.chanelOffset();
    FisInvers=other.isInvers();
    FisNotUse=other.isNotUse();
    FisInnerUse=other.isInnerUse();
    //gtBuffer=other.gtBuffer;
    acceptGtBuffer();
}

SignalDescription::SignalDescription(const QString &s)
{
    fromString(s);
}

SignalDescription::SignalDescription(int type,const QString &name,int offset)
    :FChanelType(0),FChanelName(),FChanelOffset(0),
    FisInvers(false),FisNotUse(false),FisInnerUse(false),FStoredSignalName()
{
    FChanelName=name;
    FChanelType=type;
    FChanelOffset=offset;
    gtBuffer=nullptr;
}

void SignalDescription::setChanelName(QString p)
{
    FChanelName=p;
    acceptGtBuffer();
}

void SignalDescription::setChanelType(int p)
{
    FChanelType=p;
    acceptGtBuffer();
}

SignalDescription SignalDescription::innerUse() const
{
    SignalDescription s=SignalDescription(*this);
    s.setIsInnerUse(true);
    return s;
}

QString SignalDescription::storedSignalName() const
{
    return FStoredSignalName;
}

void SignalDescription::setStoredSignalName(QString p)
{
    FStoredSignalName=p;
}

const char *SignalDescription::chanelData(int &sz) const
{
    if (gtBuffer==nullptr) return nullptr;
    sz=gtBuffer->A.size();
    return gtBuffer->A.data();
}

const char *SignalDescription::value_data(int sz) const
{
    int sz1=0;
    const char *BUF=chanelData(sz1);
    if (BUF!=nullptr){
        int chanelOffsetSz=FChanelOffset*sz;
        if (chanelOffsetSz+sz<=sz1)
            return &BUF[chanelOffsetSz];
    }
    return nullptr;
}
const char *SignalDescription::value_data(int offset,int sz) const
{
    int sz1=0;
    const char *BUF=chanelData(sz1);
    if (BUF!=nullptr){
        int chanelOffsetSz=offset*sz;
        if (chanelOffsetSz+sz<=sz1)
            return &BUF[offset];
    }
    return nullptr;
}

void SignalDescription::acceptGtBuffer()
{
    if (DefaultGtBufferInterface==nullptr) gtBuffer=nullptr; else
    gtBuffer= DefaultGtBufferInterface->getGtBuffer(FChanelType,FChanelName);
}

GtBuffer *SignalDescription::getBuffer(int type,QString name)
{
    if (DefaultGtBufferInterface==nullptr) return nullptr;
    return DefaultGtBufferInterface->getGtBuffer(type,name);

}

GtBuffer *SignalDescription::getBuffer() const
{
    return gtBuffer;
}

bool SignalDescription::value_1bit()const
{
    int sz=0;
    const char *A=chanelData(sz);
    if (A==nullptr) return 0;
    bool r=value_1bit(gtBuffer,FChanelOffset);
    if (FisInvers) r=!r;
    return r;
}

bool SignalDescription::value_1bit(GtBuffer *B,int chanelOffset)
{
    int nbyte = chanelOffset/8;
    int nbit = chanelOffset%8;
    // тут эмуляция старого пакета ТС 8 бит, со смещением начала на 1 байт
    if (B->getType()==1) nbyte++;
    if (nbyte>=B->sizeData()) return 0;
    const char *TSBUF=B->A.data();
    quint8 r=( (TSBUF[nbyte]>>nbit) & 0x01 ) ? 1 : 0;
    return r;
}


quint8 SignalDescription::value_1byte() const
{
    int sz=0;
    const char *A=chanelData(sz);
    if (A==nullptr) return 0;
    if (FChanelOffset<sz) {
        const quint8 *TSBUF=(const quint8 *)A;
        return TSBUF[FChanelOffset];
    }
    return 0;
}

bool SignalDescription::is33() const
{
    if (FisInnerUse) return false;
    if (FisNotUse) return false;
    if (FChanelName.isEmpty()) return false;
    if (gtBuffer==nullptr) return true;
    if (gtBuffer->sost!=GtBuffer::_alive) return true;
    return false;
}

void SignalDescription::setbit8(quint8* buf, size_t pos, int val)
{
    const int BitsPerChunk = 8;
    auto* bits = (quint8*)buf;
    if (val)
        *bits |=  ( 1 << (pos % BitsPerChunk) );
    else
        *bits &= ~( 1 << (pos % BitsPerChunk) );
}


void SignalDescription::setValue_1bit(bool v) const
{
    if (FChanelName.isEmpty()) return ;
    int sz=0;
    const char *A=chanelData(sz);
    if (A==nullptr) return;

    int n_im=FChanelOffset;
    int nbyte = n_im/8;
    quint8 nbit = n_im%8;
    // тут эмуляция старого пакета ТС 8 бит, со смещением начала на 1 байт
    if (FChanelType==1) nbyte++;
    if (nbyte>=sz) return ;
     quint8 *TSBUF=(quint8 *)A;
     setbit8(&TSBUF[nbyte],nbit,v);
}

void SignalDescription::setValue_1byte(quint8 v) const
{
    if (FChanelName.isEmpty()) return ;
    int sz=0;
    const char *A=chanelData(sz);
    if (A==nullptr) return;
    if (FChanelOffset>=sz) return ;
    quint8 *TSBUF=(quint8 *)A;
    TSBUF[FChanelOffset]=v;

}

void SignalDescription::setValue_data(const void *v, int sz) const
{
     if (gtBuffer==nullptr) return;
     int chanelOffset=FChanelOffset*sz;
     if (chanelOffset+sz>gtBuffer->A.size()) gtBuffer->A.resize(sz);
     memcpy(&gtBuffer->A.data()[chanelOffset],v,sz);
}


void SignalDescription::setBufferData(const void *v, int sz) const
{
    if (gtBuffer==nullptr) return;
    if (sz!=gtBuffer->A.size()) gtBuffer->A.resize(sz);
    memcpy(gtBuffer->A.data(),v,sz);
}

QString SignalDescription::toString() const
{
    if (FisNotUse) return "";
    QString s=QString("%1,").arg(FChanelName)+
            QString("%1,").arg(FChanelType)+
            QString("%1,").arg(FChanelOffset)+
            QString("%1,").arg(FisInvers)+
            QString("%1").arg(storedSignalName());
    return s;
}

void SignalDescription::clear()
{
    FChanelName.clear();
    FStoredSignalName.clear();
    FChanelOffset=0;
    FChanelType=0;
    FisInvers=false;
    FisNotUse=false;
    FisInnerUse=false;
    gtBuffer=nullptr;
}

bool SignalDescription::fromString(const QString &s)
{
    clear();
    if (s.isEmpty()) {
        FisNotUse=true;
    } else {
        QStringList l=s.split(",");
        if (l.size()>0) FChanelName=l[0];
        if (l.size()>1) FChanelType=l[1].toInt();
        if (l.size()>2) FChanelOffset=l[2].toInt();
        if (l.size()>3) FisInvers=(bool)l[3].toInt();
        if (l.size()>4) setStoredSignalName(l[4]);
        FisNotUse=false;
        acceptGtBuffer();
    }
    return true;
}

SignalDescription SignalDescription::_fromString(const QString &s)
{
    return SignalDescription(s);
}

QString SignalDescription::_toString(const SignalDescription &s)
{
    return s.toString();
}


//QString valueType2String(SignalDescription::MVP_Enums::TSignalValue valueType)
//{
//    if (valueType==SignalDescription::val1bit) return "val1bit";
//    if (valueType==SignalDescription::val1byte) return "val1byte";
//    if (valueType==SignalDescription::val2byte) return "val2byte";
//    if (valueType==SignalDescription::val4byte) return "val4byte";
//    if (valueType==SignalDescription::valunknowbyte) return "valunknowbyte";
//    return "??";
//}


void SignalDescription::setSignalTS(QString chanelName, int chanelOffset)
{
    FChanelName=chanelName;
    FChanelOffset=chanelOffset;
    FChanelType=1;
    acceptGtBuffer();
}

