#include "basevisualtools.h"
#include <QMetaProperty>
#include <QMetaType>
#include <QDebug>
#include "station.h"
#include "visual_elements.h"
#include "baseobjecttools.h"
#include "objectidgenerator.h"
#include "v_model.h"

/*
BaseObject *cloneVisual(v_Base *V, QObject2XML *XML, bool withModels)
{
    BaseObject *clonedGroup=new BaseObject();
    // запихиваем в новую группу вместе с подъобедками
    V->prepareToSave();
    QString xmlObject=XML->write(V);
    QObject *clonedObject=XML->read(xmlObject);
    clonedObject->setParent(clonedGroup);
    v_Model *vm=qobject_cast<v_Model *>(V);
    if ((withModels)&&(vm)){
        // запихиваем и модель сплошным списком - если и было дерево то уберется
        // сначала себя
        if (vm->modelObject()!=0){
            vm->modelObject()->prepareToSave();
            QString xmlObject=XML->write(vm->modelObject());
            QObject *clonedObject=XML->read(xmlObject);
            clonedObject->setParent(clonedGroup);
        }
        // и подъобектные модели
        QList<v_Model *> lv1=V->findChildren<v_Model *>();
        foreach (v_Model *subV, lv1) {
            if (subV->modelObject()!=0){
                subV->modelObject()->prepareToSave();
                QString xmlObject=XML->write(subV->modelObject());
                QObject *clonedObject=XML->read(xmlObject);
                clonedObject->setParent(clonedGroup);
            }
        }
    } else {
        QList <v_Model *> lv=clonedObject->findChildren<v_Model *>();
        foreach (v_Model *v, lv) {
            v->setModelObject(0);
        }
    }
    // восстанавливаем линки в новой группе по старым id
    clonedGroup->updateAfterLoad();
    // проставляем новые id
    QList<BaseObject*>l=clonedGroup->findChildren<BaseObject*>();
    for (int i=0;i<l.size();i++){
        l[i]->setid(l[i]->id().generated());
    }
    // убираем модель из группы - на них ссылаются из V
//    for (int i=0;i<lm.size();i++){
//        lm[i]->setParent(0);
//    }



    return clonedGroup;
}
*/
// clonedGroup copyGroup v_Base m_Base
QString copyQObjects2XMLBOX(QList<QObject *>lcpy,bool bWithModels)
{
    QList<QObject *>l;
    // детей не брать
    foreach (QObject *O1,lcpy){
        // не берем если парент уже в группе
        if (lcpy.indexOf(O1->parent())<0) l.push_back(O1);

//        if (O1->parent()!=nullptr){
//            foreach (QObject *O2, l) {
//                if (O1->parent()==O2){
//                    qDebug() << "copyVisuals2XML детей не брать";
//                    return QString();
//                }
//            }
//        }
    }

    QObject2XML XML;
    QString xmlCopyGroup="";
    BaseObject *BOX=new BaseObject();   BOX->setObjectName("BOX");
    BaseObject *OBJ=new BaseObject(BOX);OBJ->setObjectName("OBJ");
    BaseObject *LNK=nullptr;
    if (bWithModels){
        LNK=new BaseObject(BOX);LNK->setObjectName("LNK");
    }

    // клонируем

    foreach (QObject *O,l){
        QString xmlObject=XML.write(O);
        QObject *clonedObject=XML.read(xmlObject);

        clonedObject->setParent(OBJ);
        if (bWithModels){
            v_Model *vm=qobject_cast<v_Model *>(O);
            if ((vm)&&(vm->modelObject())){
                QString xmlObject=XML.write(vm->modelObject());
                QObject *clonedObject=XML.read(xmlObject);
                clonedObject->setParent(LNK);
            }
        }
    }

    xmlCopyGroup=XML.write(BOX);

    // подчистим за собой
    delete BOX;
    return xmlCopyGroup;
}

QList<QObject *> pasteXMLBOX(QString &xmlCopyGroup, QObject *Parent, bool bWithModels)
{
    QObject2XML XML;
    QList<QObject *> l;
    QObject *qBOX=XML.read(xmlCopyGroup);
    BaseObject *BOX=qobject_cast<BaseObject *>(qBOX);
    if (!BOX) return l;
    BaseObject *OBJ=BOX->findChild<BaseObject *>("OBJ",Qt::FindDirectChildrenOnly);
    if (!OBJ) return l;
    BaseObject *LNK=nullptr;
    if (bWithModels){
        LNK=BOX->findChild<BaseObject *>("LNK",Qt::FindDirectChildrenOnly);
        if (!LNK) return l;
    }

    // восстанавливаем внутренние ссылкки
    //BOX->updateAfterLoad();
    // переобуваем ИД

    QList<BaseObject *> lb=OBJ->findChildren<BaseObject *>();
    if (bWithModels) lb=BOX->findChildren<BaseObject *>();
    const QObject *SParent=superParent(Parent);
    foreach (BaseObject *B,lb){
        if (findObjectById(SParent,B->id())){
            // Строим новые id, перестраиваем линки внутри
            quint64 oldId=B->id();
            B->setid(quint64Generator::instance()->generate());
            quint64 newId=B->id();
            foreach (BaseObject *B1,lb){
                replaceLinks(B1,oldId,newId);
            }

        }
    }
    updateLinksObjects(BOX,BOX);

    l=OBJ->children();
    // чистим
    QList<QObject *> lOBJ=OBJ->children();
    // исключаем из группы и удалаяем коробку
    foreach (QObject*O, lOBJ) {
        O->setParent(0);
    }
    if (bWithModels){
        QList<QObject *> lLNK=LNK->children();
        foreach (QObject*O, lLNK) {
            O->setParent(0);
        }
    }

    delete BOX;
    return l;
}

QList<v_Model *> superFindElementsByModel(v_Base *V, m_Base *M)
{
    QList<v_Model *> l;
    // найдем родителя
    v_Base *V0=V;
    while (qobject_cast<v_Base *>(V0->parent())!=nullptr) V0=qobject_cast<v_Base *>(V0->parent());

    // ищем по детям
    QList<v_Model *> lvm=V0->findChildren<v_Model *>();
    foreach(v_Model *vm,lvm){
        if (vm->modelObject()==M) l.push_back(vm);
    }
    return l;
}

//QList<v_Model *> stationFindElementsByModel(m_Base *M)
//{
//    QList<v_Model *> l;
//    if (M){
//        Station *S=stationParent(M);
//        if (S){
//            // ищем по детям
//            QList<v_Model *> lvm=S->findChildren<v_Model *>();
//            foreach(v_Model *vm,lvm){
//                if (vm->LNKMODEL().id()==M->id()) l.push_back(vm);
//            }
//        }
//    }
//    return l;

//}




void deleteVisualElement(v_Base *V)
{
    // модель есть?
    v_Model * vm=qobject_cast<v_Model *>(V);
    if ((vm)&&(vm->modelObject()!=0)){
        BaseObject *M=vm->modelObject();
        // последний ссылается на модель?
        QList<v_Model *> l=listVisualElementWithsModel(V,M);
        if ((l.size()==1)&&(l.first()==V)){
            unlinkAllObjects2B2(M);
            M->setParent(0);
            delete M;
        }
    }
    unlinkAllObjects2B2(V);
    //V->setView(0);
    V->getProxyGraphicsItem()->setParentItem(0);
    V->setParent(0);
     delete V;

}




QList<v_Model *> listVisualElementWithsModel(v_Base *V, BaseObject *B,const QObject *_superParent)
{
    QList<v_Model *> lres;
    if (_superParent==0) _superParent=superParent(V);
    if (_superParent==0) return lres ;
    QList<v_Model *> l=_superParent->findChildren<v_Model *>();
    foreach (v_Model *vm, l) {
        if (vm->modelObject()==B) lres.push_back(vm);
    }
    return lres;
}


