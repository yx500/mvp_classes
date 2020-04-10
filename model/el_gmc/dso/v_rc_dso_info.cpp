#include "v_rc_dso_info.h"

#include "m_rc_dso.h"
#include "m_strel_dso_dbk.h"
#include "m_rc_zkr_dso.h"

#include "qpfunction.h"
#include "mcolorpalette.h"
#include "baseobjecttools.h"

#include "v_dso.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_RC_DSO_INFO,"ДСО РЦ инфо","Горка")

v_RC_DSO_INFO::v_RC_DSO_INFO(v_Base *parent) : v_Base(parent)
{
    FTEXTH=0;
    RTEXTH=10;
    DSO_RC=0;
    mDSO_RC=0;
    mDSO_Strel=0;
    mRC_ZKR_DSO=0;
    dtStartShowText=QDateTime::currentDateTime();
    lastOsyCount=0;
    d_calculateGeometry();
}

ObjectLink v_RC_DSO_INFO::LNKMODEL() const
{
    return FLNKMODEL;
}

void v_RC_DSO_INFO::setLNKMODEL(ObjectLink p)
{
    if (FLNKMODEL!=p){
        FLNKMODEL=p;
        BaseObject *B=dynamic_cast<BaseObject *>(p.obj());
        mDSO_RC=dynamic_cast<m_RC_DSO *>(B);
        mDSO_Strel=dynamic_cast<m_STREL_DSO_DBK *>(B);
        mRC_ZKR_DSO=dynamic_cast<m_RC_ZKR_DSO *>(B);

        if (mDSO_RC!=0) DSO_RC= mDSO_RC; else
            if (mDSO_Strel!=0) DSO_RC= mDSO_Strel; else
                if (mRC_ZKR_DSO!=0) DSO_RC= mRC_ZKR_DSO; else
                    DSO_RC=0;

         if (DSO_RC) {
             setObjectName("ДСО ИНФ "+DSO_RC->name());
             connect(DSO_RC,SIGNAL(stateChanged(QObject*)),this,SLOT(slotModelStateChanged(QObject*)));
         }
    }
}

void v_RC_DSO_INFO::updateAfterLoad()
{
    v_Base::updateAfterLoad();
    if (FLNKMODEL.id().num()!=0){
        BaseObject *B=superFindObjectById(this,FLNKMODEL.id());
        if (!B){
            qCritical() << "Ошибочная ссылка LNKMODEL ! " << objectName();
        }
        //setLNKMODEL(FLNKMODEL);
        //BaseObject *B=dynamic_cast<BaseObject *>(p.obj());
        FLNKMODEL.linkObj(B);
        if(B!=0){
            connect(B,SIGNAL(stateChanged(QObject*)),this,SLOT(slotModelStateChanged(QObject*)));
        }
    }
    setPos(grid2point(Fxy));
    d_calculateGeometry();


    FPropReactionEnable=true;
}

QRectF v_RC_DSO_INFO::d_boundingRect() const
{
    QRectF R=boundrect_text;
    R.adjust(-5,-5,+5,+5);
    return R;
}

void v_RC_DSO_INFO::d_paint(QPainter *painter, const QStyleOptionGraphicsItem *)
{
     
    boundrect_text=QRectF();
    qint64 msShowTime=1000;
    qint64 ms=0;
    QString text;
    if (ISEDITORMODE) {
        ms=0;
        text="дсо инф";
        if (DSO_RC!=0) text=text+DSO_RC->name();
    } else  {
        ms=dtStartShowText.msecsTo(QDateTime::currentDateTime());
        text=QString::number(getOsyCount());
    }
    if (ms<msShowTime){
        QColor clrText=Qt::black;
        if (DSO_RC!=0){
            if (get_STATE_DSO_ERROR()) clrText=getColor(v_DSO::Color_dso_error);
        }
        int alpha=255.*(1.*(msShowTime-ms)/msShowTime);
        clrText.setAlpha(alpha);
        QFont font("Times", RTEXTH-1);
        painter->setFont(font);
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QBrush(clrText),0,Qt::SolidLine));
        QRectF RT=painter->fontMetrics().boundingRect(text);
        RT.moveCenter(M[0]);
        int flags=Qt::AlignCenter | Qt::TextDontClip;
        painter->drawText(RT,flags,text,&boundrect_text);
    }
}

void v_RC_DSO_INFO::d_calculateGeometry()
{
    RTEXTH=viewPropertyInt("TEXTH_DSO_INFO",FTEXTH,10);

    QFont font("Times", RTEXTH-1);
    QFontMetrics fm(font);
    int flags=Qt::AlignCenter | Qt::TextDontClip;
    QString text=get_DSO_RC_Text();
    boundrect_text=fm.boundingRect(QRect(),flags,text);
    boundrect_text.moveCenter(M[0]);
}


void v_RC_DSO_INFO::slotModelStateChanged(QObject *O)
{
    Q_UNUSED(O);
    int newOsyCount=getOsyCount();
    if (lastOsyCount!=newOsyCount){
        lastOsyCount=newOsyCount;
        dtStartShowText=QDateTime::currentDateTime();
    }
}

int v_RC_DSO_INFO::getOsyCount() const
{
    if (DSO_RC!=0){
        if (mDSO_RC)     return mDSO_RC->STATE_DSO_OSY_COUNT();
        if (mDSO_Strel)  return mDSO_Strel->STATE_DSO_OSY_COUNT();
        if (mRC_ZKR_DSO)  return mRC_ZKR_DSO->STATE_DSO_OSY_COUNT();
    }
    return 0;
}

QString v_RC_DSO_INFO::get_DSO_RC_Text()
{

    QString text="<NN>";
    bool STATE_DSO_DIRECT=false;
    bool STATE_DSO_ERR_D1=false;
    bool STATE_DSO_ERR_D2=false;
    bool STATE_DSO_ERR_D3=false;
    bool STATE_DSO_ERR_D4=false;
    if (DSO_RC!=0){
        m_RC_DSO *mDSO_RC=dynamic_cast<m_RC_DSO *>(DSO_RC);
        m_STREL_DSO_DBK *mDSO_Strel=dynamic_cast<m_STREL_DSO_DBK *>(DSO_RC);

        if (mDSO_RC)     {
            text=QString::number(mDSO_RC->STATE_DSO_OSY_COUNT());STATE_DSO_DIRECT=mDSO_RC->STATE_DSO_DIRECT();
            STATE_DSO_ERR_D1=mDSO_RC->STATE_DSO_ERR_D1();STATE_DSO_ERR_D2=mDSO_RC->STATE_DSO_ERR_D2();
        } else if (mDSO_Strel) {
            text=QString::number(mDSO_Strel->STATE_DSO_OSY_COUNT());STATE_DSO_DIRECT=mDSO_Strel->STATE_DSO_DIRECT();
            STATE_DSO_ERR_D1=mDSO_Strel->STATE_DSO_ERR_D1();STATE_DSO_ERR_D2=mDSO_Strel->STATE_DSO_ERR_D2();STATE_DSO_ERR_D3=mDSO_Strel->STATE_DSO_ERR_D3();STATE_DSO_ERR_D4=mDSO_Strel->STATE_DSO_ERR_D4();
        } else if (mRC_ZKR_DSO) {
            text=QString::number(mRC_ZKR_DSO->STATE_DSO_OSY_COUNT());STATE_DSO_DIRECT=mRC_ZKR_DSO->STATE_DSO_DIRECT();
            STATE_DSO_ERR_D1=mRC_ZKR_DSO->STATE_DSO_ERR_D1();STATE_DSO_ERR_D2=mRC_ZKR_DSO->STATE_DSO_ERR_D2();
        }
        QString D0,D1;
        if (1){
            D0="";D1=">";
            if (STATE_DSO_DIRECT==1){D0="<";D1="";}
        } else{
            D0="<";D1="";
            if (STATE_DSO_DIRECT==1){D0="";D1=">";}
        }
        if (STATE_DSO_ERR_D1) text=text+" E1";
        if (STATE_DSO_ERR_D2) text=text+" E2";
        if (STATE_DSO_ERR_D3) text=text+" E3";
        if (STATE_DSO_ERR_D4) text=text+" E4";
        text=D0+text+D1;
    }
    return text;
}

bool v_RC_DSO_INFO::get_STATE_DSO_ERROR()
{
    if (DSO_RC!=0){
        m_RC_DSO *mDSO_RC=dynamic_cast<m_RC_DSO *>(DSO_RC);
        m_STREL_DSO_DBK *mDSO_Strel=dynamic_cast<m_STREL_DSO_DBK *>(DSO_RC);

        if (mDSO_RC)     return mDSO_RC->STATE_DSO_ERROR();
        if (mDSO_Strel) return mDSO_Strel->STATE_DSO_ERROR();
    }
    return false;
}





void v_RC_DSO_INFO::setTEXTH(int p)
{
    if (p>=0){
        SETPROPG(FTEXTH);
    }
}

