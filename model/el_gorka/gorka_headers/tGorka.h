#ifndef TGORKA_H
#define TGORKA_H

#include "old_sys_types.h"
#include "tNsl4Term.h"

#pragma pack(push, 1)

struct t_VMetrCell {
    uint8  addr;
    uint16 V;       //Скорость
    uint16 Voleg;   //Скорость
    uint8 Step;    //Ступень торможения
    uint8 RC;
    uint8  xx;
};
struct  t_VMetrCellsPacket {
    uint32     biostime;
    t_VMetrCell Cells[1];
};

// тоже самое только по другому обзывается
typedef struct  {
    unsigned   char T1: 1;
    unsigned   char T2: 1;
    unsigned   char T3: 1;
    unsigned   char T4: 1;
    unsigned   char R: 1;
    unsigned   char xx: 3;
} tZamCmd;
typedef struct  {
    unsigned   char RC: 1;
    unsigned   char RRC: 1;
    unsigned   char ErrCS: 1;
    unsigned   char RC_pred: 1;
    unsigned   char xx: 3;
    unsigned   char UFO: 1;
} tZamRC;

struct  t_ARSDescrCell {
    uint8  addr;
    uint16 V;       //Скорость
    uint16 Voleg;   //Скорость
    tZamCmd Step;    //Ступень торможения
    tZamRC RC;
    uint8  xx;
};

struct  t_ARSDescrCellsPacket {
    uint32     biostime;
    t_ARSDescrCell Cells[1];
};



struct t_Descr {//51+2
     uint8  num    ; // Номер отцепа 1-255 Живет в течении роспуска одного
     uint8  mar    ; // Резерв
     uint8  mar_f  ; // Фактический маршрут(==Заданному либо 0 либо реализов)
     int16  start  ; // Местоположение Головы отцепа |    N Эл РЦ в ТС     |
     int16  end    ; // Местоположение Хвоста отцепа |номер сигнала занятия|
     uint8  ves    ; // Вес отцепа в тоннах
     uint8  osy    ; // Длинна ( в осях)
     uint8  len    ; // Длинна ( в вагонах)
     uint8  baza   ; // Признак длиннобазности
     uint8  nagon  ; // Признак нагона
     uint8  end_slg; // Признак конца слежения (по последней РЦ на путях)
     uint8  err    ; // Признак неперевода стрелки
     uint8  dir    ; // Направление
     uint16 V_rc   ; // Скорость по РЦ
     uint16 V_zad  ; // Скорость заданная
     uint8  Stupen ; // Ступень торможения
     uint8  osy1   ; // Длинна ( в осях)
     uint8  osy2   ; // Длинна ( в осях)
     uint16 V_zad2 ; // Скорость заданная 2TP
     uint16 V_zad3 ; // Скорость заданная  3TP
     uint16 pricel ;
     uint8  old_num;
     uint8  old_mar;
     uint16 U_len  ;
     uint8  vagon  ;
     uint16 V_out  ;
     uint16 V_in2  ;
     uint16 V_out2 ;
     uint16 V_in3  ;
     uint16 V_out3 ;
     uint32 Id     ;
     uint8  st     ;
     uint16 ves_sl ;
     uint8  r_mar  ;
     uint8  t_ot[3]; // 0- растарможка 1-4 ступени максимал ступень работы замедлителя
     uint8  r_a[3] ; // 0-автомат режим ручного вмешательсва
     uint16 V_in   ; // Cкорость входа 1 ТП
     uint16 Kzp    ; // КЗП по расчету Антона
     uint8  v_rosp ; // Скорость расформирования   - норма/быстро/медленно - 0/1/2
     uint8  flag_ves; // Работоспособность весомера - да/нет/ - 0/1
     uint8  flag_r  ; // Признак ручной установки скорости
     uint8  FirstVK ;
     uint8  LastVK  ;
     uint16 addr_tp[3]; // Занятый замедлитель
     uint8  v_rosp1 ; // Скорость расформирования   - норма/быстро/медленно - 0/1/2
     uint8  p_rzp   ; // Признак выше ПТП
     uint16        VrospZ;
     uint16        VrospF;
     uint16        V_zad2_S ; // Скорость заданная 2TP
};

struct t_Descr_ext{
    int  sz_struct;

    uint8 STATE_LOCATION;
    long long  STATE_ID_RCS;
    long long STATE_ID_RCF;
    uint8 STATE_ERROR_TRACK;
    uint8 STATE_IS_CURRENT;

    uint8 STATE_GAC_ACTIVE;
    uint8 STATE_GAC_W_STRA;
    uint8 STATE_GAC_W_STRB;
    uint8 STATE_ARS_ACTIVE;
    uint8 STATE_SL_BAZA;
    uint8 STATE_SL_UR;
    uint8 STATE_SL_OSO;
    uint8 STATE_ZKR_PROGRESS;
    uint8 STATE_ZKR_S_IN;
    uint8 STATE_PUT_NADVIG;
    uint8 STATE_KZP_OS;
    uint16 STATE_KZP_D;
    uint16 STATE_V;
    uint16 STATE_V_ARS;
    uint16 STATE_V_KZP;
    uint16 STATE_V_DISO;
    uint16 STATE_D_RCS_XOFFSET;
    uint16 STATE_D_RCF_XOFFSET;
    uint8 STATE_D_ORDER_RC;
    uint16 STATE_TICK;


};

struct t_NewDescr{
    t_Descr D;
    t_Descr_ext E;
};


#define MaxKZPCells 50

struct  t_KzpCell {
    int16 D;   // Значение
    uint16 E;   // Ошибка
};
struct  t_KzpCell_V {
    int16  V;   // Скорость *10
    uint16 P;   // Признак , 0- нормально
};

struct  t_KzpCellsPacket {
    uint32   tick;
    t_KzpCell   Cells[MaxKZPCells];
    t_KzpCell_V Cells_V[MaxKZPCells];
};


struct  t_OsyCell {
    uint32 V;   // Значение
};

struct  t_OsyCellsPacket {
    uint32   biostime;
    t_OsyCell Cells[1];
};


// 20 01 2021
struct  t_OsyCell_21 {
    int32 V;   // Значение
    int8 D;   // посл. напр.
    int8 E;   // сбой
    int32 EV;   // кол-во сбоев
};
// 17 - тип
// diso - имя
#define IpxNameZKR "ZKR" //"ZKR1" "ZKR2"
#define IpxTypeZKR 18
struct t_BufZkr {
long tim;
uint16 num;
uint16 osy;
uint16 ves;
uint16 baza;
uint16 vagoni;
uint16 flag_r;
uint16 osy1;
uint16 osy2;
uint16 osy_v;
uint16 num_rosp;
// astana
uint16 osy3;
uint16 telegi;
uint16 osy_wrk;
uint16 t_osy;
uint16 err;
//kln
uint16 osy1_rosp;
uint16 osy2_rosp;
uint16 osy3_rosp;
//lng
uint16 ves_osi;

};
#define IpxNameKZP "KZP_"   //KZP_mar
#define IpxTypeKZP 19
struct t_KvKzp {
    uint8   mar;           // маршрут   c 1                     //обязательное поле для заполнения
    uint16  val;           // наше значение после пересчета     //обязательное поле для заполнения
    uint32  tim;           // время в милисекундах              //обязательное поле для заполнения
    uint16  stat;          // пока не знаю, будут коды ошибок
    uint8   addr;          // адрес блока Kv
    uint16  val_ks;        // получили от блока
    uint16  val_ks_i;      // получили от блока мгновенное
    uint16  cnt;           // счетчик, полученный от блока
    uint16  dat[64];
};

#define IpxNameKZPINF "KZP_" //KZP_addr
#define IpxTypeKZPINF 20
struct t_KzpInf {
    uint32  x;             //
    uint32  mar;           // маршрут по оллыку с 1
    long  sum_vag_len;       // длина вагонов зашедших на путь
    long  v;             // скорость последнего движущегося отцепа по кзп
    long  pred_val_kzp;  // предыдущее значение кзп до сброса в нул
    long  pred_vag_len;       // длина отепа зашедшего на путь
    long points_count;
    struct Point {
        uint16 val;
        uint32 tim;
    } points[16];
};

#define IpxNameKZPMKINF "KZPMK_" //KZP_mar
#define IpxTypeKZPMKINF 20

struct t_KzpMKInfCell{
        int32 d;              // x хвоста, 0 - реальное, 1..7 расчитанное по прогнозу, с минусом - хвост до КЗП
        int32  v;             // скрость , 0 - реальное, 1..7 расчитанное по прогнозу
        uint32 Descr_Id;      // ...
        uint8  Descr_num;     // Descr.num
        uint16 Descr_len;     // Descr.len
        uint16 Descr_vagon;     // Descr.len
        uint16 Descr_pricel;     // Descr.pricel
        uint16 d_start_prognose;   // расстояние с кторого начался прогноз(пред
        uint16 d_finish_prognose;  // прогнозируемая остановка
        uint16 v_finish_prognose;  // прогнозируемая скорость в момент остановки
        uint16 sec_finish_prognose;// прогнозируемое время до остановки сек
};

#define MaxKzpMKInfCell 8

struct t_KzpMKInf {
    uint32  mar;           // маршрут по оллыку с 1
    uint32  d_current;
    uint32  e_current;
     int32  v_current;   // -1, если ксорость не определена, скорость только в сторону парка
    uint32  U_len_rospusk;   // сумма U_len с включения режима РОСПУСК
    uint8   vagon_rospusk;   // сумма vagon с включения режима РОСПУСК
    uint16  v_out_tp;    // Предподчтительня скорость выхода с ТП3(для отцепа на ТП, если есть или для условного легкого вагона если ТП не занята)

     t_KzpMKInfCell Cells[MaxKzpMKInfCell]; // отслеживаемые отцепы (0 - крайний,1..7 - перед ним)
};




#define MaxVagon 120

#define IpxNameADK3 "adk3" //KZP_mar
#define IpxTypeADK3 25
struct  tADK3Cell {
    uint8 a;    // адрес
    uint8 p[2]; // давление
    uint8 t[2]; // температура
    uint8 u[2]; // напряжение
    uint8 r;    // rezerv

    
};

struct  tADK3CellsPacket {
    uint32    biostime;
    tADK3Cell Cells[1];
};

struct TKZPFROM_test{
        unsigned   reserv3:1;      //        0	Зарезервировано
        unsigned   err_ots_tok:1;  //        1	Отсутствует ток в рельсовой цепи 	2, 4
        unsigned   reserv2:1;      //        2	Зарезервировано
        unsigned   err_temp:1;     //        3	Слишком высокая температура внутри БИЗП	2, 4
        unsigned   reserv1:1;       //        4	Зарезервировано
        unsigned   err_acp_napr:1; //        5	Ошибка теста АЦП напряжения	1, 4
        unsigned   err_acp_tok:1;  //        6	Ошибка теста АЦП тока	1, 4
        unsigned   err:1;          //        7	Признак наличия ошибка (1 - да, 0 - нет)


};
struct TKZPFROM_dopflag{
    unsigned   err_kalir:1;          //    0	Требуется проведение повторной калибровки пути	3, 5
    unsigned   err_ots_sv:1;          //    1	Отсутствие связи	2, 4
    unsigned   err_crc:1;          //    2	Неправильная контрольная сумма в принятом ответе от БИЗП	2, 4
    unsigned   err_kod:1;          //    3	Код БИЗПД больше допустимого значения	3, 5
    unsigned   put_svob:1;          //    4	Путь свободен	1
    unsigned   err_pogr:1;          //    5	Погрешность определения расстояния выше нормы	1, 5
    unsigned   err_sopr:1;          //    6	Сопротивление рельсовой линии выше нормы	2, 4
    unsigned   reserv:1;          //7	Зарезервировано
} ;
struct TKZPFROM{
        uint32          time;
        uint16          ms;
        unsigned        puch:4;
        unsigned        put:4;
        uint16          all;
        uint16          len;
        TKZPFROM_test   test;
        TKZPFROM_dopflag dopflag;
};

struct  tDescrCom {
     uint8  type   ; //  TYP Є®¬¬ ­¤л
     uint8  num    ; // Ќ®¬Ґа ®вжҐЇ  1-255 †ЁўҐв ў вҐзҐ­ЁЁ а®бЇгбЄ  ®¤­®Ј®
     uint8  mar    ; // ‡ ¤ ­­л© ¬ аиагв
     uint16 Vr     ; // ђ бзҐв­ п бЄ®а®бвм ўле®¤ 
     uint8  Stupen ; // ‘вгЇҐ­м в®а¬®¦Ґ­Ёп
     uint8  old_num;
     uint8  old_mar;
     uint16 U_len  ;
     uint8  vagon  ;
     long   Id     ;
     uint8  flag_r ;
     uint16 ves_sl ;

     uint16 Vr1     ; // Расчетная скорость выхода 1 ТП
     uint16 Kzp     ; // КЗП по расчету Антона
     uint8  v_rosp  ; // Скорость расформирования   - норма/быстро/медленно - 0/1/2
     uint8  flag_ves; // Работоспособность весомера - да/нет/ - 0/1
     uint8  FirstVK;
     uint8  LastVK;
     uint8  rez[1]  ; // Резерв
     uint8  v_rosp1 ; // Скорость расформирования   - норма/быстро/медленно - 0/1/2  ; // Резерв
     uint8  PointBeforTormoz ;// Признак для олега рассчета прицела выше 3ТП
     uint16 Vnadv;
     uint16 VnadvFact;


};

#define IpxNameV_zad "V_zad"
#define IpxTypeV_zad 23
typedef struct   {
     uint8  num    ; // Номер отцепа 1-255 Живет в течении роспуска одного
     uint32 Id     ;
     uint16 V_zad2_S; // Скорость заданная 2TP от Юры
  } tDescrComS;



#pragma pack(pop)


#endif //TGORKA_H




