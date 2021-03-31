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
	uint8_t    D;   // 0..19 , 20 > , 21 --, 32 пробел, 255- брать из D8
	uint8_t    M;
	uint8_t    D8[8]; // с платы в плату
	uint8_t    status; // 0 - ok, не ноль -  ош связи
};

struct UkVag {
       ukv  data[3];
};

//---------------------------------------------------------------------------
#pragma pack(pop)



#endif  //ukvagH
