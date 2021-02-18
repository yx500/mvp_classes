#ifndef TGORKA_H
#define TGORKA_H

#include "old_sys_types.h"
#include "tNsl4Term.h"

#pragma pack(push, 1)

struct t_VMetrCell {
    uint8  addr;
    uint16 V;       //��������
    uint16 Voleg;   //��������
    uint8 Step;    //������� ����������
    uint8 RC;
    uint8  xx;
};
struct  t_VMetrCellsPacket {
    uint32     biostime;
    t_VMetrCell Cells[1];
};

// ���� ����� ������ �� ������� ����������
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
    uint16 V;       //��������
    uint16 Voleg;   //��������
    tZamCmd Step;    //������� ����������
    tZamRC RC;
    uint8  xx;
};

struct  t_ARSDescrCellsPacket {
    uint32     biostime;
    t_ARSDescrCell Cells[1];
};



struct t_Descr {//51+2
     uint8  num    ; // ����� ������ 1-255 ����� � ������� �������� ������
     uint8  mar    ; // ������
     uint8  mar_f  ; // ����������� �������(==��������� ���� 0 ���� ��������)
     int16  start  ; // �������������� ������ ������ |    N �� �� � ��     |
     int16  end    ; // �������������� ������ ������ |����� ������� �������|
     uint8  ves    ; // ��� ������ � ������
     uint8  osy    ; // ������ ( � ����)
     uint8  len    ; // ������ ( � �������)
     uint8  baza   ; // ������� ��������������
     uint8  nagon  ; // ������� ������
     uint8  end_slg; // ������� ����� �������� (�� ��������� �� �� �����)
     uint8  err    ; // ������� ���������� �������
     uint8  dir    ; // �����������
     uint16 V_rc   ; // �������� �� ��
     uint16 V_zad  ; // �������� ��������
     uint8  Stupen ; // ������� ����������
     uint8  osy1   ; // ������ ( � ����)
     uint8  osy2   ; // ������ ( � ����)
     uint16 V_zad2 ; // �������� �������� 2TP
     uint16 V_zad3 ; // �������� ��������  3TP
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
     uint8  t_ot[3]; // 0- ����������� 1-4 ������� �������� ������� ������ �����������
     uint8  r_a[3] ; // 0-������� ����� ������� ������������
     uint16 V_in   ; // C������� ����� 1 ��
     uint16 Kzp    ; // ��� �� ������� ������
     uint8  v_rosp ; // �������� ���������������   - �����/������/�������� - 0/1/2
     uint8  flag_ves; // ����������������� �������� - ��/���/ - 0/1
     uint8  flag_r  ; // ������� ������ ��������� ��������
     uint8  FirstVK ;
     uint8  LastVK  ;
     uint16 addr_tp[3]; // ������� �����������
     uint8  v_rosp1 ; // �������� ���������������   - �����/������/�������� - 0/1/2
     uint8  p_rzp   ; // ������� ���� ���
     uint16        VrospZ;
     uint16        VrospF;
     uint16        V_zad2_S ; // �������� �������� 2TP
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
    int16 D;   // ��������
    uint16 E;   // ������
};
struct  t_KzpCell_V {
    int16  V;   // �������� *10
    uint16 P;   // ������� , 0- ���������
};

struct  t_KzpCellsPacket {
    uint32   tick;
    t_KzpCell   Cells[MaxKZPCells];
    t_KzpCell_V Cells_V[MaxKZPCells];
};


struct  t_OsyCell {
    uint32 V;   // ��������
};

struct  t_OsyCellsPacket {
    uint32   biostime;
    t_OsyCell Cells[1];
};


// 20 01 2021
struct  t_OsyCell_21 {
    int32 V;   // ��������
    int8 D;   // ����. ����.
    int8 E;   // ����
    int32 EV;   // ���-�� �����
};
// 17 - ���
// diso - ���
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
    uint8   mar;           // �������   c 1                     //������������ ���� ��� ����������
    uint16  val;           // ���� �������� ����� ���������     //������������ ���� ��� ����������
    uint32  tim;           // ����� � ������������              //������������ ���� ��� ����������
    uint16  stat;          // ���� �� ����, ����� ���� ������
    uint8   addr;          // ����� ����� Kv
    uint16  val_ks;        // �������� �� �����
    uint16  val_ks_i;      // �������� �� ����� ����������
    uint16  cnt;           // �������, ���������� �� �����
    uint16  dat[64];
};

#define IpxNameKZPINF "KZP_" //KZP_addr
#define IpxTypeKZPINF 20
struct t_KzpInf {
    uint32  x;             //
    uint32  mar;           // ������� �� ������ � 1
    long  sum_vag_len;       // ����� ������� �������� �� ����
    long  v;             // �������� ���������� ����������� ������ �� ���
    long  pred_val_kzp;  // ���������� �������� ��� �� ������ � ���
    long  pred_vag_len;       // ����� ����� ��������� �� ����
    long points_count;
    struct Point {
        uint16 val;
        uint32 tim;
    } points[16];
};

#define IpxNameKZPMKINF "KZPMK_" //KZP_mar
#define IpxTypeKZPMKINF 20

struct t_KzpMKInfCell{
        int32 d;              // x ������, 0 - ��������, 1..7 ����������� �� ��������, � ������� - ����� �� ���
        int32  v;             // ������� , 0 - ��������, 1..7 ����������� �� ��������
        uint32 Descr_Id;      // ...
        uint8  Descr_num;     // Descr.num
        uint16 Descr_len;     // Descr.len
        uint16 Descr_vagon;     // Descr.len
        uint16 Descr_pricel;     // Descr.pricel
        uint16 d_start_prognose;   // ���������� � ������� ������� �������(����
        uint16 d_finish_prognose;  // �������������� ���������
        uint16 v_finish_prognose;  // �������������� �������� � ������ ���������
        uint16 sec_finish_prognose;// �������������� ����� �� ��������� ���
};

#define MaxKzpMKInfCell 8

struct t_KzpMKInf {
    uint32  mar;           // ������� �� ������ � 1
    uint32  d_current;
    uint32  e_current;
     int32  v_current;   // -1, ���� �������� �� ����������, �������� ������ � ������� �����
    uint32  U_len_rospusk;   // ����� U_len � ��������� ������ �������
    uint8   vagon_rospusk;   // ����� vagon � ��������� ������ �������
    uint16  v_out_tp;    // ���������������� �������� ������ � ��3(��� ������ �� ��, ���� ���� ��� ��� ��������� ������� ������ ���� �� �� ������)

     t_KzpMKInfCell Cells[MaxKzpMKInfCell]; // ������������� ������ (0 - �������,1..7 - ����� ���)
};




#define MaxVagon 120

#define IpxNameADK3 "adk3" //KZP_mar
#define IpxTypeADK3 25
struct  tADK3Cell {
    uint8 a;    // �����
    uint8 p[2]; // ��������
    uint8 t[2]; // �����������
    uint8 u[2]; // ����������
    uint8 r;    // rezerv

    
};

struct  tADK3CellsPacket {
    uint32    biostime;
    tADK3Cell Cells[1];
};

struct TKZPFROM_test{
        unsigned   reserv3:1;      //        0	���������������
        unsigned   err_ots_tok:1;  //        1	����������� ��� � ��������� ���� 	2, 4
        unsigned   reserv2:1;      //        2	���������������
        unsigned   err_temp:1;     //        3	������� ������� ����������� ������ ����	2, 4
        unsigned   reserv1:1;       //        4	���������������
        unsigned   err_acp_napr:1; //        5	������ ����� ��� ����������	1, 4
        unsigned   err_acp_tok:1;  //        6	������ ����� ��� ����	1, 4
        unsigned   err:1;          //        7	������� ������� ������ (1 - ��, 0 - ���)


};
struct TKZPFROM_dopflag{
    unsigned   err_kalir:1;          //    0	��������� ���������� ��������� ���������� ����	3, 5
    unsigned   err_ots_sv:1;          //    1	���������� �����	2, 4
    unsigned   err_crc:1;          //    2	������������ ����������� ����� � �������� ������ �� ����	2, 4
    unsigned   err_kod:1;          //    3	��� ����� ������ ����������� ��������	3, 5
    unsigned   put_svob:1;          //    4	���� ��������	1
    unsigned   err_pogr:1;          //    5	����������� ����������� ���������� ���� �����	1, 5
    unsigned   err_sopr:1;          //    6	������������� ��������� ����� ���� �����	2, 4
    unsigned   reserv:1;          //7	���������������
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
     uint8  type   ; //  TYP ���� ���
     uint8  num    ; // ����� ��楯  1-255 ����� � �祭�� ����  ������
     uint8  mar    ; // � � ��� � �����
     uint16 Vr     ; // � ��� � ᪮���� ��室 
     uint8  Stupen ; // ��㯥�� �ମ�����
     uint8  old_num;
     uint8  old_mar;
     uint16 U_len  ;
     uint8  vagon  ;
     long   Id     ;
     uint8  flag_r ;
     uint16 ves_sl ;

     uint16 Vr1     ; // ��������� �������� ������ 1 ��
     uint16 Kzp     ; // ��� �� ������� ������
     uint8  v_rosp  ; // �������� ���������������   - �����/������/�������� - 0/1/2
     uint8  flag_ves; // ����������������� �������� - ��/���/ - 0/1
     uint8  FirstVK;
     uint8  LastVK;
     uint8  rez[1]  ; // ������
     uint8  v_rosp1 ; // �������� ���������������   - �����/������/�������� - 0/1/2  ; // ������
     uint8  PointBeforTormoz ;// ������� ��� ����� �������� ������� ���� 3��
     uint16 Vnadv;
     uint16 VnadvFact;


};

#define IpxNameV_zad "V_zad"
#define IpxTypeV_zad 23
typedef struct   {
     uint8  num    ; // ����� ������ 1-255 ����� � ������� �������� ������
     uint32 Id     ;
     uint16 V_zad2_S; // �������� �������� 2TP �� ���
  } tDescrComS;



#pragma pack(pop)


#endif //TGORKA_H




