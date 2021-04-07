#include "v_ukvag.h"

#include "m_ukvag.h"
#include "qpfunction.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_UkVag,"Указатель вагонов","VISUAL ГОРКА")

v_UkVag::v_UkVag(QObject *parent) : v_Model(parent)
{
    FFONT=QFont("Times", 12-1);
    FTEXT_HEIGHT=1;
    FSIZE=QSizeF(2,1);
}

void v_UkVag::updateAfterLoad()
{
    v_Model::updateAfterLoad();
    _m_UkVag=qobject_cast<m_UkVag*>(modelObject());
}

void v_UkVag::calculateGeometry()
{
    R=QRectF(0,0,FSIZE.width()*gridDX(),FSIZE.height()*gridDY());
    R.moveCenter(QPointF(0,0));

    {
        _boundingRect=R;
        if (!boundrect_text.isEmpty())
            _boundingRect=R.united(boundrect_text);
        _boundingRect=R.adjusted(-1,-1,+1,+1);
    }
}

void v_UkVag::d_paint(QPainter *painter, const QStyleOptionGraphicsItem *)
{
    painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));
    painter->setBrush(Qt::NoBrush);
    if (_m_UkVag)
        if (_m_UkVag->STATE_33()) painter->setBrush(getColor(Color_state_33));
    painter->drawRect(R);
    if (FTEXT_HEIGHT*gridDY()<=0) FFONT.setPointSizeF(1); else FFONT.setPointSizeF(FTEXT_HEIGHT*gridDY());
    painter->setFont(FFONT);
    int flags=Qt::AlignCenter;
    QString text="--";
    if (_m_UkVag)
        text=QString("%1 %2 %3").arg(_m_UkVag->STATE_D1()).arg(_m_UkVag->STATE_D2()).arg(_m_UkVag->STATE_D3());
    if (ISEDITORMODE) text=objectName();
    painter->drawText(R,flags,text,&boundrect_text);

}
