#ifndef GMC_DBK_H
#define GMC_DBK_H
#include <QtGlobal>
#include <QDateTime>

/*
 * Структуры данных  ГМЦ  используемые во внешних модулях
 */



class TGMC_ComplectNumber{
public:
    enum {
        _none=0,
        gmc_1a=1,
        gmc_1b=2,
        gmc_2a=3,
        gmc_2b=4
    };
static quint8 calcComplectNumber(int Complect,int NomInComplect) {
    if ((Complect==1)&&(NomInComplect==1)) return gmc_1a;
    if ((Complect==1)&&(NomInComplect==2)) return gmc_1b;
    if ((Complect==2)&&(NomInComplect==1)) return gmc_2a;
    if ((Complect==2)&&(NomInComplect==2)) return gmc_2b;
    return _none;
}
static quint8 complectNumber_brother(quint8 complectNumber) {
    if (complectNumber==gmc_1a) return gmc_1b;
    if (complectNumber==gmc_1b) return gmc_1a;
    if (complectNumber==gmc_2a) return gmc_2b;
    if (complectNumber==gmc_2b) return gmc_2a;
    return _none;
}
static quint8 complectNumber_anotherMaster(quint8 complectNumber) {
    if (complectNumber==gmc_1a) return gmc_2a;
    if (complectNumber==gmc_1b) return gmc_2a;
    if (complectNumber==gmc_2a) return gmc_1a;
    if (complectNumber==gmc_2b) return gmc_1a;
    return _none;
}
static quint8 MyComplect(quint8 complectNumber) {
    if (complectNumber==gmc_1a) return 1;
    if (complectNumber==gmc_1b) return 1;
    if (complectNumber==gmc_2a) return 2;
    if (complectNumber==gmc_2b) return 2;
    return 0;
}
static quint8 NomInComplect(quint8 complectNumber) {
    if (complectNumber==gmc_1a) return 1;
    if (complectNumber==gmc_1b) return 2;
    if (complectNumber==gmc_2a) return 1;
    if (complectNumber==gmc_2b) return 2;
    return 0;
}

static bool isOnLine(int complectNumber,int releVibor) {
    if ((releVibor==1) && ((complectNumber==gmc_1a)||(complectNumber==gmc_1b))) return true;
    if ((releVibor==0) && ((complectNumber==gmc_2a)||(complectNumber==gmc_2b))) return true;
    return false;
}
};

#pragma pack(push, 1)
struct TGMCSostData{
    enum {
        gmc_regim_nothing=0,
        gmc_regim_wait_start=1,
        gmc_regim_wait_start_with_brother=2,
        gmc_regim_live=100,
        gmc_regim_error=-1
    };
    quint8 complectNumber;
    qint8 GMCSost;
    quint8 releVKL; // реле WD
    quint8  releVibor; // реле выбора комплекта
    quint32     tick; // тик
    quint32     recived_tu_code;
    quint32     recived_tu_counter;

    quint16 crc_magicBufer; // crc буфера состояний

    static const char * packetName(quint8 complectNumber)
    {
        switch (complectNumber){
            case TGMC_ComplectNumber::gmc_1a: return "gmc-1a_st";
            case TGMC_ComplectNumber::gmc_1b: return "gmc-1b_st";
            case TGMC_ComplectNumber::gmc_2a: return "gmc-2a_st";
            case TGMC_ComplectNumber::gmc_2b: return "gmc-2b_st";
            default: return "";
        }
    }
    static quint16 packetType(){return 115;}



};

struct TDBK485Packet{
    quint8      startByte;
    quint16     addrDBK;
    quint8      tip;
    quint8      version;
    quint8      diagnostic;
    quint8      data[6];
    quint32     tick;
    quint16     crc;
    QString toString(){
        return QString("start:%1 DBK:%2 tip:%3 ver:%4 diag:%5 d:%6 %7 %8 %9 %10 %11 tick:%12").
                       arg(startByte).arg(addrDBK).arg(tip).arg(version).arg(diagnostic).
                       arg(data[0]).arg(data[1]).arg(data[2]).arg(data[3]).arg(data[4]).arg(data[5]).arg(tick);
    }
};

// посылается при каждом запросе ДБК
struct TDBK485PacketExchange{
    quint8 complectNumber;  // 1..4
    quint8 dbkLine;         // номер в конфиге 1..4
    quint8 online;          //  в линии
    TDBK485Packet packet_to;
    TDBK485Packet packet_from;
    qint16 moxa485_err;    // ошибка при посылке в com порт COM_ERR в moxa485
    static const char *packetName(quint8 complectNumber)
    {
        /*switch (complectNumber){
            case TGMC_ComplectNumber::gmc_1a: return "klc-1a_st";
            case TGMC_ComplectNumber::gmc_1b: return "klc-1b_st";
            case TGMC_ComplectNumber::gmc_2a: return "klc-2a_st";
            case TGMC_ComplectNumber::gmc_2b: return "klc-2b_st";
            default: return "";
        }*/
        switch (complectNumber){
            case TGMC_ComplectNumber::gmc_1a: return "klc_st";
            case TGMC_ComplectNumber::gmc_1b: return "klc_st";
            case TGMC_ComplectNumber::gmc_2a: return "klc_st";
            case TGMC_ComplectNumber::gmc_2b: return "klc_st";
            default: return "";
        }

    }
    static quint16 packetType(){return 114;}
};



struct TGMCTU{
    quint32     code;
    quint32     id;     // идент , возвращаемый в ОТУ всегда уникальный

    quint64     objectId;       // код объекта [не исп]
    quint32     objectCmd;     // код команды для объекта [не исп]

    static const char * packetName(){
        return "gmc_tu";
    }
    static quint16 packetType(){return 100;}
    QString toString(){ return QString("%1 [%2-%3])").arg(code).arg(objectId).arg(objectCmd);}
};

struct TGMCRTU{
    enum TGMCRTUCODE{ rtu_unknow=0,
                      rtu_ok=1,
                      rtu_process=2,
                      rtu_error=-1
                    };
    TGMCTU gmc_tu;
    qint8 rtu_code;
    quint8 complectNumber;
    char status_message[64];
    static const char * packetName(){return "gmc_rtu";}
    static quint16 packetType(){return 102;}

};


#pragma pack(pop)









#endif // GMC_DBK_H
