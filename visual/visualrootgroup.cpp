#include "visualrootgroup.h"
#include <QFileInfo>
#include "mvp_objectfactory.h"
#include "mcolorpalette.h"
REGISTERELEMENT(VisualRootGroup,"VISUAL","VISUAL БАЗОВЫЕ")

VisualRootGroup::VisualRootGroup(QObject *parent) : BaseObject(parent)
{
}

void VisualRootGroup::updateAfterLoad()
{
    BaseObject::updateAfterLoad();

    // загрузим поллитру
    if ((!FPALETTE_FILENAME.isEmpty()&&(!XMLfile().isEmpty()))){
        QString fn=QFileInfo(XMLfile()).path()+"/"+FPALETTE_FILENAME;
        MColorPalette::instance()->load(fn);
    }
}

bool VisualRootGroup::isCanAddObject(QObject *O) const
{
    if (qobject_cast<Visual_Screen*>(O)) return true;
    if (qobject_cast<Visual_Elements*>(O)) return true;
    return false;

}


