#ifndef QNXInc_H
#define QNXInc_H

#include <stdint.h>
#define __time32_t quint32

#pragma pack(push, 1)

struct tNewMask
{
  uint8_t len;
  uint8_t typ;

  uint8_t  Isprav;
  uint8_t  YesObm;
  uint8_t  IspCANA;
  uint8_t  IspCANB;
  uint8_t  USOIn[4];
  uint8_t  KolUSOOS;
  uint8_t  USOOutO[8];
  uint8_t  KolUSORE;
  uint8_t  USOOutR[8];
};


struct tDLP{  // тип 14
 uint8_t       Len;
 uint8_t       Typ;
 uint8_t       Moduls;            //  Исправность модулей
 uint8_t       MaskRazrObm;       //  Маска разрешения обмена
 uint8_t       ObmModCANA;        //  Исправность обмена с модулями по CANA
 uint8_t       ObmModCANB;        //  Исправность обмена с модулями по CANB
 uint8_t       USO_VivMcO;        //  Исправность УСО Вывода младшего крейта основной комплект
 uint8_t       USO_VivMcR;//4.8. Исправность УСО Вывода младшего крейта резервный комплект
 uint8_t       USO_VivScO;//4.9. Исправность УСО Вывода старшего крейта основной комплек
 uint8_t       USO_VivScR;//4.10. Исправность УСО Вывода старшего крейта резервный комплект
 uint8_t       USO_VivOtvO;//4.11. Исправность УСО Ответственного Вывода основной комплект
 uint8_t       USO_VivOtvR;//4.12. Исправность УСО Ответственного Вывода резервный комплект

 uint8_t       USO_Vv[16];
 //uint8_t       USO_VvRk[5];



};



struct tTIMask
{
  uint16_t XERR;

  uint8_t  PModErr[2];
  uint8_t  PMActive[2];

  uint8_t  USOIn[8];
  uint8_t  USOOut[4];
  uint8_t  USOCom;

  uint8_t  XXX;
};

struct tKPMask  // тип 15
{
  uint8_t  AddrUz[2];
  uint8_t  SLK1;
  uint8_t  SLK2;
  uint8_t  KodSost[2];
  uint16_t SLK3;
  uint32_t SLK4;
  uint8_t  rez[8];
};



struct tObmen
{
  uint16_t  SKP[10];
};






//  Эта для ГАВ ХОДА
#ifdef MOSC
struct tQMsg
{
 __int16      Ln;
 __int16      Tp;
 uint8        Id;
 uint8        Id2;
 time_t       Time;
 uint16       Dst;
 uint16       Src;
 tTIMask      LPY;
// tDLP        LPX;
 uint8       CRC;
};

#else

//  Эта версия работала для ГОРЬКОГО !

struct tQMsg
{
 int16_t      Ln;
 int16_t      Tp;
 uint16_t      Dst;
 uint16_t      Src;
 __time32_t      Time;
 uint8_t       Id;
 uint8_t       Id2;
 tTIMask      LPY;
// tDLP        LPX;
 uint8_t       CRC;
};
#endif


#define QMHEADERSZ 16

#pragma pack(pop)



const uint16_t TYPE_EX  = 0x0475;
const uint16_t TYPE_TS1 = 0x0501;
const uint16_t TYPE_TS2 = 0x0511;
const uint16_t TYPE_TS3 = 0x0512;
const uint16_t TYPE_TU  = 0x0505;
const uint16_t TYPE_RTU = 0x0502;
const uint16_t TYPE_TI1 = 0x0503;
const uint16_t TYPE_TI2 = 0x0504;

//#include <windows.h>
//#include "crc_16.h"
//inline uint16 CalcCRC(void* p, int sz) { return crc_16((uint8*)p, sz); }


#endif
