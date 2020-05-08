#include "visualgroupproperty.h"

#include <QDebug>
#include <QEvent>

#include "mvp_objectfactory.h"
REGISTERELEMENT(VisualGroupProperty,"Общее свойство","VISUAL БАЗОВЫЕ")

VisualGroupProperty::VisualGroupProperty(QObject *parent) : BaseObject(parent)
{
//    V=QString();
    FV=QVariant();
}


//void VisualGroupProperty::setSTORED_VALUE(QString p)
//{
//    if (V!=p){
//        V=p;
//        emit propertyChanged(this);
//    }
//}

//bool VisualGroupProperty::event(QEvent *ev)
//{
//    if (ev->type() == QEvent:: DynamicPropertyChange) {
//        QString S;
//        MVP_ObjectFactory::instance()->QVariantToQString(property(qPrintable("VAL")),S);
//        setSTORED_VALUE(S);
//        return true;
//    }

//    return QObject::event(ev);
//}

