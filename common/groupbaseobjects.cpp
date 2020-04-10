#include "groupbaseobjects.h"
#include <QMetaProperty>

#include "baseobjecttools.h"
#include "gtbuffer.h"
#include "signaldescription.h"
#include "mvp_system.h"

REGISTERELEMENT(GroupBaseObjects,"Группа","БАЗОВЫЕ")


GroupBaseObjects::GroupBaseObjects(QObject *parent) :
    BaseObject(parent)
{
    xmlFile.clear();
}

void GroupBaseObjects::updateAfterLoad()
{
    BaseObject::updateAfterLoad();
    registrateUsedGtBuffers();
}

void GroupBaseObjects::connectPropertyChangedSlot()
{
    QList<BaseObject*> l=findChildren<BaseObject*>();
    foreach (BaseObject *B, l) {
        connect (B,SIGNAL(propertyChanged(QObject*)),this,SLOT(slotChildObjectPropertyChanged(QObject*)));
        connect (B,SIGNAL(stateChanged(QObject*)),this,SLOT(slotChildObjectPropertyChanged(QObject*)));
    }
}


void GroupBaseObjects::slotChildObjectPropertyChanged(QObject *O)
{
    emit ChildObjectPropertyChanged(O);
}

QList<GtBuffer *> GroupBaseObjects::registrateUsedGtBuffers() const
{
    QList<GtBuffer *> lChanels;

    QList<QObject *> l=findChildren<QObject *>();
    foreach (QObject *O, l) {
        const QMetaObject *metaObject=O->metaObject();
        for (int idx = 0; idx < metaObject->propertyCount(); idx++) {
            QMetaProperty metaProperty = metaObject->property(idx);
            int type = metaProperty.userType();
            if (type == qMetaTypeId<SignalDescription>()){
                QVariant val=metaProperty.read(O);
                SignalDescription p = val.value<SignalDescription>();
                if (!p.chanelName().isEmpty()){
                    GtBuffer * chanel=p.getBuffer();
                    if (lChanels.indexOf(chanel)<0) lChanels.push_back(chanel);
                }
            }
        }
    }
    return lChanels;
}

void GroupBaseObjects::updateStates()
{
    // убираем множественное срабатывание если изменилось более одного состояния
    foreach(QObject * b,children()){
        auto *B=qobject_cast<BaseObject *>(b);
        if (!B->disableUpdateStates){
            B->setStateChanged(false);
            B->setStateChangedEmit(false);
            B->updateStates();
            B->setStateChangedEmit(true);
            if (B->isStateChanged()) B->emitStateChanged();
        }
    }
}

