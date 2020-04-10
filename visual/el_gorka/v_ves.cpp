#include "v_ves.h"

#include "m_ves.h"
#include "qpfunction.h"


#include "mvp_objectfactory.h"
REGISTERELEMENT(v_Ves,"Весомер","VISUAL ГОРКА")

v_Ves::v_Ves(QObject *parent) : v_Model(parent)
{
    FFONT=QFont("Times", 12-1);
    FTEXT_HEIGHT=1;
    FSIZE=QSizeF(2,1);
    resetStates();
}

void v_Ves::resetStates()
{
    v_Model::resetStates() ;
    FSTATE_OSY_COUNT=0;
    FSTATE_OSY_SREDN_VES=0;
}

void v_Ves::updateAfterLoad()
{
    v_Model::updateAfterLoad();
    _m_Ves=qobject_cast<m_Ves*>(modelObject());
}

void v_Ves::calculateGeometry()
{
    R=QRectF(0,0,FSIZE.width()*gridDX(),FSIZE.height()*gridDY());
    R.moveCenter(QPointF(0,0));
//    FFONT.setPointSizeF(FTEXT_HEIGHT*gridDY());
//    QFontMetrics fm(FFONT);
//    int flags=Qt::AlignCenter;
//    boundrect_text=fm.boundingRect(R.toRect(),flags,objectName());
    {
        _boundingRect=R;
        if (!boundrect_text.isEmpty())
            _boundingRect=R.united(boundrect_text);
        _boundingRect=R.adjusted(-1,-1,+1,+1);
    }
}

void v_Ves::d_paint(QPainter *painter, const QStyleOptionGraphicsItem *)
{
    painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));
    painter->setBrush(Qt::NoBrush);
    if (_m_Ves)
        if (_m_Ves->STATE_33()) painter->setBrush(getColor(Color_state_33));
    painter->drawRect(R);
    if (FTEXT_HEIGHT*gridDY()<=0) FFONT.setPointSizeF(1); else FFONT.setPointSizeF(FTEXT_HEIGHT*gridDY());
    painter->setFont(FFONT);
    int flags=Qt::AlignCenter;
    QString text=QString::number(FSTATE_OSY_SREDN_VES,'g',1);
    if (ISEDITORMODE) text=objectName();
    painter->drawText(R,flags,text,&boundrect_text);
}
