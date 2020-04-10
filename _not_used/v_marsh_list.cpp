#include "v_marsh_list.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_Marsh_List,"VISUAL: Список маршутов","Стандартные")

v_Marsh_List::v_Marsh_List(v_Base *parent) : v_Model(parent)
{
    setZValue(10);
    setObjectName("Маршруты");
}

void v_Marsh_List::updateAfterLoad()
{
    v_Model::updateAfterLoad();
    updateFromModel();
}
void v_Marsh_List::calculateGeometry()
{
    R=QRectF(0,0,gridDX()*4,gridDY()*4);
}

void v_Marsh_List::updateFromModel()
{
    foreach(m_Marsh *mMarsh,marsh_List()->marshruts()){
        v_Marsh *vMarsh=new v_Marsh(this);
        vMarsh->setModelObject(mMarsh);
        vMarsh->update_rcList();
        vMarsh->setObjectName(mMarsh->objectName());

    }
}


v_Marsh *v_Marsh_List::addMarsh()
{
    m_Marsh *mMarsh=new m_Marsh(this->marsh_List());
    v_Marsh *vMarsh=new v_Marsh(this);
    mMarsh->setid(ObjectId::generated());
    vMarsh->setModelObject(mMarsh);
    return vMarsh;
}

void v_Marsh_List::delMarsh(v_Marsh *M)
{
    if (children().indexOf(M)>=0){
        M->marsh()->setParent(0);
        M->setParent(0);
        //delete M->marsh();
        //delete M;
    }
}

void v_Marsh_List::clear()
{
    QList<v_Marsh*> lMarsh=findChildren<v_Marsh *>();
    foreach (v_Marsh*M, lMarsh) {
        M->marsh()->setParent(0);
        M->setParent(0);
    }
    updateModel();

}

void v_Marsh_List::updateModel()
{
    marsh_List()->reBuildlMarsh_Str();

}

QList<v_Marsh *> v_Marsh_List::marshruts() const
{
    QList<v_Marsh*> lMarsh=findChildren<v_Marsh *>();
    return lMarsh;
}

void v_Marsh_List::clearSELECTED()
{
    QList<v_Marsh*> lMarsh=findChildren<v_Marsh *>();
    foreach (v_Marsh*M, lMarsh) {
        M->setSELECTED(false);
    }
}

QRectF v_Marsh_List::d_boundingRect() const
{
    return R;
}

void v_Marsh_List::d_paint(QPainter *painter, const QStyleOptionGraphicsItem *)
{
    QBrush B=QBrush(Qt::NoBrush);
    QPen   P=QPen(Qt::black);
    painter->setPen(P);painter->setBrush(B);
    painter->drawRect(R);
    painter->drawText(R, Qt::AlignCenter,objectName());
}


