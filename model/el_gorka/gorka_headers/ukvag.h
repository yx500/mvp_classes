#ifndef ukvagH
#define ukvagH

//---------------------------------------------------------------------------

#include "old_sys_types.h"

//---------------------------------------------------------------------------

#define IpxNameToUkVag "toukvag"
#define IpxNameFromUkVag "fromukvag"
#define IpxTypeUkVag 6

#pragma pack(push, 1)

struct ukv {
	uint8_t    D;   // 0..19 , 20 > , 21 --, 32 ������, 255- ����� �� D8
	uint8_t    M;
	uint8_t    D8[8]; // � ����� � �����
	uint8_t    status; // 0 - ok, �� ���� -  �� �����
};

struct UkVag {
       ukv  data[3];
};

//---------------------------------------------------------------------------
#pragma pack(pop)



#endif  //ukvagH
