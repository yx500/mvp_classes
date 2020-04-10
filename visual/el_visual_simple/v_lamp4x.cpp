#include "v_lamp4x.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_Lamp4x,"Лампочка4зн","VISUAL Простые элементы")


REGISTERPROPERTY(v_Lamp4x,SCOLOR_P2, "CLRS_P__2_0010","Цвет букв 2","Цвет текст")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_P3, "CLRS_P__3_0011","Цвет букв 3","Цвет текст")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_P4, "CLRS_P__4_0100","Цвет букв 4","Цвет текст")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_P5, "CLRS_P__5_0101","Цвет букв 5","Цвет текст")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_P6, "CLRS_P__6_0110","Цвет букв 6","Цвет текст")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_P7, "CLRS_P__7_0111","Цвет букв 7","Цвет текст")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_P8, "CLRS_P__8_1000","Цвет букв 8","Цвет текст")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_P9, "CLRS_P__9_1001","Цвет букв 9","Цвет текст")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_P10,"CLRS_P_10_1010","Цвет букв 10","Цвет текст")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_P11,"CLRS_P_11_1011","Цвет букв 11","Цвет текст")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_P12,"CLRS_P_12_1100","Цвет букв 12","Цвет текст")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_P13,"CLRS_P_13_1101","Цвет букв 13","Цвет текст")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_P14,"CLRS_P_14_1110","Цвет букв 14","Цвет текст")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_P15,"CLRS_P_15_1111","Цвет букв 15","Цвет текст")

REGISTERPROPERTY(v_Lamp4x,SCOLOR_B2, "CLRS_P__2_0010","Цвет заполн 2","Цвет заполн")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_B3, "CLRS_P__3_0011","Цвет заполн 3","Цвет заполн")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_B4, "CLRS_P__4_0100","Цвет заполн 4","Цвет заполн")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_B5, "CLRS_P__5_0101","Цвет заполн 5","Цвет заполн")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_B6, "CLRS_P__6_0110","Цвет заполн 6","Цвет заполн")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_B7, "CLRS_P__7_0111","Цвет заполн 7","Цвет заполн")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_B8, "CLRS_P__8_1000","Цвет заполн 8","Цвет заполн")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_B9, "CLRS_P__9_1001","Цвет заполн 9","Цвет заполн")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_B10,"CLRS_P_10_1010","Цвет заполн 10","Цвет заполн")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_B11,"CLRS_P_11_1011","Цвет заполн 11","Цвет заполн")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_B12,"CLRS_P_12_1100","Цвет заполн 12","Цвет заполн")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_B13,"CLRS_P_13_1101","Цвет заполн 13","Цвет заполн")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_B14,"CLRS_P_14_1110","Цвет заполн 14","Цвет заполн")
REGISTERPROPERTY(v_Lamp4x,SCOLOR_B15,"CLRS_P_15_1111","Цвет заполн 15","Цвет заполн")

v_Lamp4x::v_Lamp4x(v_Base *parent) : v_Lamp1x(parent)
{
    FSIGNAL_2.clear();
    FSIGNAL_3.clear();
    FSIGNAL_4.clear();
}

void v_Lamp4x::updateStates()
{
    v_Base::updateStates();
    int i=FSIGNAL_4.value_1bit()*8+
          FSIGNAL_3.value_1bit()*4+
          FSIGNAL_4.value_1bit()*2+
          FSIGNAL_1.value_1bit()*1;
    if (i>=16) i=-1;
    setSTATE_SIGNAL(i);
}
