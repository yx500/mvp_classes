#ifndef BASEVISUALTOOLS_H
#define BASEVISUALTOOLS_H

#include "m_base.h"
#include "v_model.h"
#include "baseobject.h"
#include "qobject2xml.h"

class Station;

// копирует визуалы с моделями и помещает в общую группу
QString copyQObjects2XMLBOX(QList<QObject *>lcpy, bool bWithModels=true);
QList<QObject *> pasteXMLBOX(QString &xmlCopyGroup, QObject *Parent,bool bWithModels=true);
// ищет визуалы с моделью у суперпарента V
QList<v_Model *> superFindElementsByModel(v_Base *V,m_Base * M);
//QList<v_Model *> stationFindElementsByModel(m_Base * M);


QList<v_Model *> listVisualElementWithsModel(v_Base * V, BaseObject *B, const QObject *_superParent=0);
void deleteVisualElement(v_Base * V);

#endif // BASEVISUALTOOLS_H
