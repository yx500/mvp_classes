#include "v_gmcdiag.h"
#include "mcolorpalette.h"

#include <qmath.h>

#include "qpfunction.h"
#include "mcolorpalette.h"
#include "mvp_system.h"
#include "gmc_dbk.h"
#include "mvp_objectfactory.h"
REGISTERELEMENT(v_GMCDiag,"Состояния ГМЦ","VISUAL Диагностика")

REGISTERCOLOR(v_GMCDiag,gmc_mainSost_wait_start,"ГМЦ осн.сост_ожид.старта",Qt::yellow);
REGISTERCOLOR(v_GMCDiag,gmc_mainSost_live,"ГМЦ осн.сост_ок",Qt::green);
REGISTERCOLOR(v_GMCDiag,gmc_mainSost_error,"ГМЦ осн.сост_ошибка",Qt::red);
REGISTERCOLOR(v_GMCDiag,gmc_wdSost_ok,"ГМЦ состWD_ок",Qt::green);
REGISTERCOLOR(v_GMCDiag,gmc_wdSost_error,"ГМЦ состWD_ошибка",Qt::red);
REGISTERCOLOR(v_GMCDiag,gmc_lineSost_ok,"ГМЦ состLINE_ок",Qt::green);
REGISTERCOLOR(v_GMCDiag,gmc_lineSost_error,"ГМЦ состLINE_ошибка",Qt::red);

void v_GMCDiag::setCOMPLECTNUMBER(int p)
{
    FCOMPLECTNUMBER=p;
    FSIGNAL_STATUS.setChanelType(TGMCSostData::packetType());
    FSIGNAL_STATUS.setChanelName(TGMCSostData::packetName(FCOMPLECTNUMBER));
    FSIGNAL_STATUS.setChanelOffset(0);
}

v_GMCDiag::v_GMCDiag(v_Base *parent) : v_StaticText(parent)
{
    FINDICATOR=mainSost;
    FCOMPLECTNUMBER=0;
    FTO_SEC=5;
    _DatagramPacket2_tick=0;
    packetRecivedTime=QTime();
    FSIGNAL_STATUS.clear();
    resetStates();
}

void v_GMCDiag::resetStates()
{
    FSTATE_REGIM=0;
    FSTATE_RELE_VKL=0;
    FSTATE_RELE_VIBOR=0;
}

void v_GMCDiag::d_paint(QPainter *painter, const QStyleOptionGraphicsItem * ){
    QBrush B;
    QString text;
    if (FSTATE_33){
        B=QBrush(Qt::gray);
        text="нет канала";
    } else {


        switch (FINDICATOR) {
        case mainSost: {
            switch (FSTATE_REGIM){
            case TGMCSostData::gmc_regim_nothing:
                B=QBrush(Qt::gray);
                text="---";
                break;
            case TGMCSostData::gmc_regim_wait_start:
                B=QBrush(getColor(Color_gmc_mainSost_wait_start));
                text="СТАРТ";
                break;
            case TGMCSostData::gmc_regim_wait_start_with_brother:
                B=QBrush(getColor(Color_gmc_mainSost_wait_start));
                text="СТАРТ!";
                break;
            case TGMCSostData::gmc_regim_live:
                B=QBrush(getColor(Color_gmc_mainSost_live));
                text="В РАБОТЕ";
                break;
            case TGMCSostData::gmc_regim_error:
                B=QBrush(getColor(Color_gmc_mainSost_error));
                text="ОШИБКА";
                break;


            }
        }; break;
        case wdSost: {
            if (FSTATE_RELE_VKL==1){
                B=QBrush(getColor(Color_gmc_wdSost_ok));
                text="ВКЛЮЧЕН";
            } else {
                B=QBrush(getColor(Color_gmc_wdSost_error));
                text="СТОП";
            }
        };break;
        case lineSost: {
            if (FCOMPLECTNUMBER==0) {
                B=QBrush(getColor(Qt::gray));
                text="---";
            }
            if (TGMC_ComplectNumber::isOnLine(FCOMPLECTNUMBER,FSTATE_RELE_VIBOR)){
                B=QBrush(getColor(Color_gmc_lineSost_ok));
                text="В ЛИНИИ";
            } else {
                B=QBrush(getColor(Color_gmc_lineSost_error));
                text="ОТКЛ";
            }
        };break;
        }
    }
    painter->setBrush(B);
    painter->setPen(Qt::black);
    painter->drawRect(allign_rect);
    if (FSH==0) FFONT.setPointSizeF(12); else FFONT.setPointSizeF(FSH);
    painter->setFont(FFONT);
    painter->setPen(QPen(QBrush(FCOLOR_TEXT),1,Qt::SolidLine));
    int flags=FFLAGS;
    painter->drawText(allign_rect,flags,text,&boundrect_text);

}

void v_GMCDiag::updateAfterLoad()
{
    v_StaticText::updateAfterLoad();

}

void v_GMCDiag::updateStates()
{
    GtBuffer *chanel=FSIGNAL_STATUS.getBuffer();
    if ((chanel==nullptr)||(chanel->sost!=GtBuffer::_alive))  {
        setSTATE_33(true);
        setSTATE_REGIM(0);
        setSTATE_RELE_VKL(0);
        setSTATE_RELE_VIBOR(0);
        return;
    }
    auto GMCSostData=reinterpret_cast<const TGMCSostData *>(FSIGNAL_STATUS.value_data(sizeof(TGMCSostData)));
    if (GMCSostData->complectNumber==FCOMPLECTNUMBER){
        if (_DatagramPacket2_tick!=chanel->getTick()){
            _DatagramPacket2_tick=chanel->getTick();
            packetRecivedTime=QTime::currentTime();
        }
    }
    bool bonline=true;

    if ((!packetRecivedTime.isValid())||(packetRecivedTime.elapsed()>FTO_SEC*1000)) bonline=false;
    if (_DatagramPacket2_tick==0) bonline=false;
    if (bonline) {
        setSTATE_33(false);
        setSTATE_REGIM(GMCSostData->GMCSost);
        setSTATE_RELE_VKL(GMCSostData->releVKL);
        setSTATE_RELE_VIBOR(GMCSostData->releVibor);
    } else {
        setSTATE_33(true);
    }
}


