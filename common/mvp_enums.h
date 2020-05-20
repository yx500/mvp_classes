#ifndef MVP_ENUMS_H
#define MVP_ENUMS_H

#include <QMetaObject>
class MVP_Enums { /// < Обратите внимание, класс Enums теперь сам по себе!
public:
    enum  TDirect:int {left2right=0,right2left=1};
    Q_ENUM(TDirect)
    enum  TGroupBool:int {g_false=0,g_true=1,g_default=2};
    Q_ENUM(TGroupBool)
    enum  TStrelPol:int {pol_plus=0,pol_minus=1,pol_w=2,pol_unknow=-1};
    Q_ENUM(TStrelPol)
    enum  TRCBusy :int {free=0,busy=1,busy_unknow=2,busy_not_accepted};
    Q_ENUM(TRCBusy)
    enum TStikType{stik_0=0,stik_no_line=1,stik_line=2,stik_negab=3};
    Q_ENUM(TStikType)
    enum TRCSrezType{srez_n=0,srez_v,srez_h};
    Q_ENUM(TRCSrezType)
    enum TTextPos{tp_Custom=0,
                  tp_HLeftVTop=1,   tp_HCenterVTop=2,   tp_HRightVTop=3,
                  tp_HLeftVCenter=8,tp_HCenterVCenter=9,tp_HRightVCenter=4,
                  tp_HLeftVBottom=7,tp_HCenterVBottom=6,tp_HRightVBottom=5};

    Q_ENUM(TTextPos)
    Q_FLAGS(TTextFlags)
    enum TTextFlag{
             AlignLeft=Qt::AlignLeft,
             AlignRight=Qt::AlignRight,
             AlignHCenter=Qt::AlignHCenter,
             AlignJustify=Qt::AlignJustify,
             AlignTop=Qt::AlignTop,
             AlignBottom=Qt::AlignBottom,
             AlignVCenter=Qt::AlignVCenter,
             AlignCenter=Qt::AlignCenter,
             TextDontClip=Qt::TextDontClip,
             TextSingleLine=Qt::TextSingleLine,
             TextExpandTabs=Qt::TextExpandTabs,
             TextShowMnemonic=Qt::TextShowMnemonic,
             TextWordWrap=Qt::TextWordWrap,
             TextIncludeTrailingSpaces=Qt::TextIncludeTrailingSpaces
        };
    Q_ENUM(TTextFlag)
    Q_DECLARE_FLAGS(TTextFlags, TTextFlag)

    enum TSignalValue{val1bit=0,val1byte,val2byte,val4byte,valText,valunknowbyte};
    Q_ENUM(TSignalValue)





    Q_GADGET ///< Он легче Q_OBJECT и вообще скромняшка!
    MVP_Enums() = delete; ///< std=c++11, обеспечивает запрет на создание любого экземпляра Enums
};

const qreal _undefV_=65535;


#endif // MVP_ENUMS_H
