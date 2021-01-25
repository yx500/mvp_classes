#include "v_button1x.h"
#include "qdrawutil.h"
#include <QPalette>
#include <QStylePainter>
#include <QApplication>

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_Button1x,"Кнопка1зн","VISUAL Простые элементы")


v_Button1x::v_Button1x(v_Base *parent) : v_Lamp1x(parent)
{
    FSDOWN=false;
}

void v_Button1x::d_paint(QPainter *painter, const QStyleOptionGraphicsItem *)
{
    QPalette pal;
    QString text=objectName();

    if ((FSTATE_SIGNAL<0)||(FSTATE_SIGNAL>=16)){
        pal=QPalette(getColor(Color_state_33));
    } else{
        pal=QPalette(getColor(FSCOLOR_B[FSTATE_SIGNAL]),getColor(Color_FON));
        pal.setColor(QPalette::ButtonText,getColor(FSCOLOR_P[FSTATE_SIGNAL]));
        if (!FSTEXT[FSTATE_SIGNAL].isEmpty()) text=FSTEXT[FSTATE_SIGNAL];

    }

    QStyleOptionButton ss;
    ss.state=QStyle::State_Enabled;

    if (FSDOWN) ss.state=QStyle::State_Sunken; else
                ss.state=QStyle::State_Raised;
    ss.rect=allign_rect.toRect();
    QString sss=text.replace("\\n","\n");
    ss.text=sss;//text+'\n'+text;

    if (RTEXTH>0){
        FFONT.setPointSizeF(RTEXTH);
        painter->setFont(FFONT);
    }
    ss.fontMetrics=painter->fontMetrics();
    ss.palette=pal;
    QApplication::style()->drawControl(QStyle::CE_PushButton,&ss,painter);


//    if ((FSDOWN)&&(FSTATE_SIGNAL))
//        qDrawWinButton(painter,allign_rect.toRect(),pal,true,&B); else
//        qDrawWinButton(painter,allign_rect.toRect(),pal,false,&B);
//    FFONT.setPointSizeF(RTEXTH);
//    painter->setFont(FFONT);
//    int flags=FFLAGS;
    //painter->drawText(allign_rect,flags,objectName(),&boundrect_text);
}
