#include "v_regim_indikator.h"

REGISTERCOLOR(v_Regim_Indikator,p_regimRospusk,"Режим РОСПУСК текст",Qt::black);
REGISTERCOLOR(v_Regim_Indikator,b_regimRospusk,"Режим РОСПУСК фон",Qt::green);
REGISTERCOLOR(v_Regim_Indikator,p_regimPausa,"Режим ПАУЗА текст",Qt::black);
REGISTERCOLOR(v_Regim_Indikator,b_regimPausa,"Режим ПАУЗА фон",Qt::yellow);
REGISTERCOLOR(v_Regim_Indikator,p_regimStop,"Режим СТОП текст",Qt::white);
REGISTERCOLOR(v_Regim_Indikator,b_regimStop,"Режим СТОП фон",Qt::red);

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_Regim_Indikator,"Режим индикатор","VISUAL ГОРКА")
v_Regim_Indikator::v_Regim_Indikator(QObject *parent) : v_Model(parent)
{
    regim=-1;
    FSIZE=QSizeF(6,2);
}

void v_Regim_Indikator::d_paint(QPainter *painter, const QStyleOptionGraphicsItem *)
{
    QColor penColor;
    QColor brushColor;
    QString text;

        switch (regim) {

        case 0: penColor=getColor(Color_b_regimStop);
                brushColor=getColor(Color_p_regimStop);
                text="СТОП";
        break;
        case 1: penColor=getColor(Color_b_regimRospusk);
                brushColor=getColor(Color_p_regimRospusk);
                text="РОСПУСК";
        break;
        case 2: penColor=getColor(Color_b_regimPausa);
                brushColor=getColor(Color_p_regimPausa);
                text="ПАУЗА";
        break;
        case 33: penColor=Qt::black;
                brushColor=getColor(Color_state_33);
                text="ПАУЗА";
        break;
        default:penColor=Qt::black;
                brushColor=Qt::gray;
                text="-----";
        break;
        }
        painter->setBrush(brushColor);
        painter->setPen(penColor);
        painter->drawRect(allign_rect);
        QFont F=QFont("Times", 1);
        if (allign_rect.height()>2) F.setPointSizeF(allign_rect.height()-2);
        painter->setFont(F);
        int flags=Qt::AlignCenter;
        painter->drawText(allign_rect,flags,text);
}

void v_Regim_Indikator::resetStates()
{
    regim=-1;
}

void v_Regim_Indikator::calculateGeometry()
{
    allign_rect=QRectF(0,0,FSIZE.width()*gridDX(),FSIZE.height()*gridDY());
    allign_rect.moveCenter(QPointF(0,0));
    _boundingRect=allign_rect;
}

void v_Regim_Indikator::updateStates()
{
    regim=modelObject()->property("STATE_REGIM").toInt();
    if (modelObject()->property("STATE_33")==1) regim=33;
}
