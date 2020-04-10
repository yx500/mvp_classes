#ifndef UART_PACKET
#define UART_PACKET

#include <stdint.h>
#include <cstring>

#pragma pack(push, 1)
struct  tUartIn {
    uint8_t  b0_addr;
//    uint16_t  b1_Vzad;//> только в 4Горка и Могилев //todo
    uint8_t  b1_Vzad;
    uint8_t  b2_reg;
    uint8_t  b3_stupen;
    uint8_t  b4_addr;
    uint8_t  b5_dVot;
    uint8_t  b6_len;
    uint8_t  b7_xxx;//need crc
};

struct  tUartOut {
    uint8_t  b0_addr;
    uint16_t w1_V;
    uint16_t w3_V2;
    uint8_t  b5_st;    //Ступень торможения
    uint8_t  b6_rj;
    uint8_t  b7_xxx;//need crc
};



struct  tSoftIn {
    tUartIn block8;

    tSoftIn(){
        std::memset( this, 0, sizeof(*this) );
    }
};

struct  tSoftOut {
    tUartOut block8;
    uint32_t count;
    int32_t  F;
    int32_t  A;
    uint8_t  sigs;
    uint8_t  tp_out;//tp out
    uint16_t conf;//addr+jmprs
    uint32_t ms;//millisec

    tSoftOut(){
        std::memset( this, 0, sizeof(*this) );
    }
};

#pragma pack(pop)


#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <iterator>

inline std::string print_8byte(const char *p)
{
    std::ostringstream ss;
    ss << "{";
    ss << std::nouppercase << std::hex;
    std::copy( p, p+8, std::ostream_iterator<int>(ss, " ") );
    ss<< "}"<<std::ends;
    return ss.str();
}

inline std::string print_tUartOut(const tUartOut& p)
{
    std::ostringstream ss;
    ss << "{";
    ss << std::nouppercase << std::setfill('0')
       << "a=" << std::setw(2) << std::hex << int(p.b0_addr)
       << ", v=" << std::dec << int(p.w1_V)
       << ", v2=" << std::dec << int(p.w3_V2)
       << ", st=" << std::setw(2) << std::hex << int(p.b5_st)
       << ", rj=" << std::setw(2) << std::hex << int(p.b6_rj)
       << ", x="  << std::setw(2) << std::hex  << int(p.b7_xxx);
    ss<< "}"<<std::ends;
    return ss.str();
}

inline std::string print_tUartIn(const tUartIn& p)
{
    std::ostringstream ss;
    ss<<print_8byte( reinterpret_cast<const char*>(&p) );
    return ss.str();
}




#endif // UART_PACKET

