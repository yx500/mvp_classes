#include "v_train_nums.h"
#include "v_rc.h"
#include "mvp_system.h"
#include "signalchanel.h"
#include "m_otceps.h"

#include "mvp_objectfactory.h"
//REGISTERELEMENT(v_Train_Nums,"Номера поездов","Стандартные")


v_Train_Nums::v_Train_Nums(v_Base *parent) : v_Base(parent)
{
    Otceps=m_Otceps::instance();
    setZValue(10);
    setObjectName("Номера поездов");

}

void v_Train_Nums::updateStates()
{
    prepare();
//    QList<v_RC*> lrc=view()->findChildren<v_RC*>();
//    foreach (v_RC*vrc, lrc) {
//       m_RC *mrc=vrc->mRC();
//       int number=mrc->SIGNAL_BUSY().chanelOffset();
//       v_Train_Num *tn=add_Train_Num(number);
//       QPolygonF cl=vrc->centreLine();
//       QPointF rcP=cl.last();
//       QPointF tnP=view()->mapFromItem(vrc,rcP);
//       //tn->setParentItem(vrc);
//       tn->setPos(tnP);
//       tn->update();
//    }

//    QList<v_RC*> lrc=view()->findChildren<v_RC*>();
//    QList<m_Otcep *> vOtceps=Otceps->otceps();

//    foreach (m_Otcep *otc, vOtceps) {
//        foreach (v_RC*vrc, lrc) {
//            if (otc->RCS==vrc->modelObject()){
//                QPointF rcP=QPointF(0,0);
//                QPolygonF cl=vrc->centreLine();
//                if (cl.size()>0) {
//                    rcP=cl.last();
//                }
//                QPointF tnP=view()->mapFromItem(vrc,rcP);
//                v_Train_Num *tn=add_Train_Num(otc->Descr.num);
//                if (tn->pos()!=tnP){
//                    tn->setPos(tnP);
//                    tn->calculateGeometry();
//                    tn->update();
//                }
//                if (otc->Descr.err) tn->color=Qt::red; else  tn->color=Qt::white;

//                break;
//            }
//        }
//    }

//    foreach(v_Train_Num *tn,trainsVisible){
//        if (!tn->exists){
//            tn->setParentItem(0);
//            tn->setParent(0);
//            trainsVisible.removeOne(tn);
//            delete tn;
//        }
//    }
}

void v_Train_Nums::calculateGeometry()
{
    R=QRectF(0,0,gridDX()*4,gridDY()*4);
}

void v_Train_Nums::d_paint(QPainter *painter, const QStyleOptionGraphicsItem *)
{
    QBrush B=QBrush(Qt::NoBrush);
    QPen   P=QPen(Qt::black);
    painter->setPen(P);painter->setBrush(B);
    painter->drawRect(R);
    painter->drawText(R, Qt::AlignCenter,objectName());
}

void v_Train_Nums::prepare()
{
    foreach (v_Train_Num *tn, trainsVisible) {
        tn->exists=false;
    }
}

v_Train_Num *v_Train_Nums::add_Train_Num(int number)
{
//    foreach (v_Train_Num *tn, trainsVisible) {
//        if (tn->number==number){
//            tn->exists=true;
//            return tn;
//        }
//    }
//    v_Train_Num *tn=new v_Train_Num(this);
//    tn->setParentItem(view());
//    tn->number=number;
//    tn->exists=true;
//    tn->width=4*gridDX();
//    tn->height=2*gridDY();
//    tn->color=Qt::white;
//    tn->direct=1;

//    trainsVisible.push_back(tn);
//    return tn;
}
