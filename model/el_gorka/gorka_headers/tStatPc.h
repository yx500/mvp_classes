#ifndef TSTATPC_H
#define TSTATPC_H

#include "old_sys_types.h"

#pragma pack(push, 1)


struct  tStatPC {//14:10
      uint8 TestCP;        //Тест платы ТС/ТУ    !!! Текущий номер отцепа                             
      uint8 TestLP;        //Тест платы ФТУ                                                           
      uint8 PetlTU;        //Совпадение входного и выходного потока ТУ                                
      uint8 Test800;       //Наличие частоты покоя в линии                                            
      uint8 TestCS;        //Соответствие интервала между ЦС нормативному                             
      uint8 TestLanCPX;    //Наличие       'СОГЕНЕРАТОРА'                                             
      uint8 TestLanDNC1;   //Наличие канала связи с источниками ТУ                                    
      uint8 TestLanDNC2;   //Наличие канала связи с источниками ТУ                                    
      uint8 SostGenCPX;    //Статус        'СОГЕНЕРАТОРА'                                             
      uint8 SostGen;       //Статус Собственный   
      uint32  time;        //Время данного сообщения                                   
};

struct tStatGate {
      uint8  BadDg;         //Кол-во потерь пакетов
      uint8  BadCRC;        //Кол-во потерь CRC
      uint8  TransSost;     //Петля статуса
      uint8  SostGenWan;    //Статус соседа WAN
      uint8  SostGenWan1;   //Статус соседа WAN (из LAN)
      uint8  TestWan;       //Наличие WAN
      uint8  PWan;          //Меня слышат
      uint8  CountWan;      //Счетчик посылок WAN
      uint8  TestLanCPX;    //Наличие 'СОГЕНЕРАТОРА'
      uint8  TestLan;       //Наличие LAN
      uint8  SostGenCPX;    //Статус 'СОГЕНЕРАТОРА'
      uint8  SostGenCPX1;   //Статус 'СОГЕНЕРАТОРА'( из WAN)
      uint8  SostGen;       //Статус Собственный
      uint8  Rezerv[15];    //
      uint32 tim;           //Для сохранания общности
};


struct tStatPCDg
{
  char  Name[12];
  uint8 Type;
  tStatPC d;
};



#pragma pack(pop)


#endif //TSTATPC_H
