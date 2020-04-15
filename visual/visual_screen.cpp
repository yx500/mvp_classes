#include "visual_screen.h"
#include "v_base.h"

#include "baseobjecttools.h"
#include "v_linked.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(Visual_Screen,"Экран","VISUAL БАЗОВЫЕ")

Visual_Screen::Visual_Screen(QObject *parent) : v_Base(parent)
{
    setObjectName("Экран");
    FGRIDDX=10;
    FGRIDDY=10;
    FisEditorMode=false;
    FVisualGroupPropertyes=nullptr;
    FVIEWSIZE=QSizeF();
}

bool Visual_Screen::isCanAddObject(QObject *O) const
{
    if (qobject_cast<VisualGroupPropertyes*>(O)) return true;
    if (qobject_cast<v_Linked*>(O)) return true;
    return false;
}

void Visual_Screen::addObject(QObject *O)
{
    O->setParent(this);
}

void Visual_Screen::setGRIDDX(int p)
{
    if (FGRIDDX!=p){
        FGRIDDX=p;
        reCalculateGeometry();
    }
}


void Visual_Screen::setGRIDDY(int p)
{
    if (FGRIDDY!=p){
        FGRIDDY=p;
        reCalculateGeometry();
    }
}


void Visual_Screen::updateAfterLoad()
{
    v_Base::updateAfterLoad();
    FVisualGroupPropertyes=findChildren<VisualGroupPropertyes*>(QString(),Qt::FindDirectChildrenOnly).first();
    if (FVisualGroupPropertyes){
        connect(FVisualGroupPropertyes,SIGNAL(visualGroupPropertyesChanged(QObject*)),this,SLOT(slotVisualGroupPropertyesChanged(QObject*)));
        FVisualGroupPropertyes->setTarget(this);
    }

}


void Visual_Screen::activate()
{
    QList<v_Base *> lV=findChildren<v_Base *>();
    foreach (v_Base *v, lV) {
        v->setVisualParent(this);
        v->setScreen(this);
    }
    if (FVisualGroupPropertyes)
        FVisualGroupPropertyes->setTarget(this);


    // активируем подэкраны
    QList<Visual_Screen *> ls=findChildren<Visual_Screen *>(QString(),Qt::FindDirectChildrenOnly);
    foreach (Visual_Screen *s, ls) {
        s->activate();
    }
    reCalculateGeometry();
}

void Visual_Screen::reCalculateGeometry()
{
        QList<v_Base *> lV1=findChildrenVisual<v_Base *>();
        foreach (v_Base *V1, lV1) {
            V1->setxy(V1->xy());
            V1->calculateGeometry();
            V1->getProxyGraphicsItem()->update();
        }
}



void Visual_Screen::slotVisualGroupPropertyesChanged(QObject *)
{
    reCalculateGeometry();
}

void Visual_Screen::updateStates()
{
    if (proxyGraphicsItem->scene()==nullptr) return;
    v_Base::updateStates();

}
