#ifndef TSTATPC_H
#define TSTATPC_H

#include "old_sys_types.h"

#pragma pack(push, 1)


struct  tStatPC {//14:10
      uint8 TestCP;        //���� ����� ��/��    !!! ������� ����� ������                             
      uint8 TestLP;        //���� ����� ���                                                           
      uint8 PetlTU;        //���������� �������� � ��������� ������ ��                                
      uint8 Test800;       //������� ������� ����� � �����                                            
      uint8 TestCS;        //������������ ��������� ����� �� ������������                             
      uint8 TestLanCPX;    //�������       '������������'                                             
      uint8 TestLanDNC1;   //������� ������ ����� � ����������� ��                                    
      uint8 TestLanDNC2;   //������� ������ ����� � ����������� ��                                    
      uint8 SostGenCPX;    //������        '������������'                                             
      uint8 SostGen;       //������ �����������   
      uint32  time;        //����� ������� ���������                                   
};

struct tStatGate {
      uint8  BadDg;         //���-�� ������ �������
      uint8  BadCRC;        //���-�� ������ CRC
      uint8  TransSost;     //����� �������
      uint8  SostGenWan;    //������ ������ WAN
      uint8  SostGenWan1;   //������ ������ WAN (�� LAN)
      uint8  TestWan;       //������� WAN
      uint8  PWan;          //���� ������
      uint8  CountWan;      //������� ������� WAN
      uint8  TestLanCPX;    //������� '������������'
      uint8  TestLan;       //������� LAN
      uint8  SostGenCPX;    //������ '������������'
      uint8  SostGenCPX1;   //������ '������������'( �� WAN)
      uint8  SostGen;       //������ �����������
      uint8  Rezerv[15];    //
      uint32 tim;           //��� ���������� ��������
};


struct tStatPCDg
{
  char  Name[12];
  uint8 Type;
  tStatPC d;
};



#pragma pack(pop)


#endif //TSTATPC_H
