#ifndef FORMULAOBJTS_H
#define FORMULAOBJTS_H

/* расширение работы с логическими выражениями
   добавление обработки ТС и свойств QObject
*/

#include "formula.h"

class SignalsManager;
class SignalDescriptionTextInterface;


enum {_pt_ts_val=_pt_number+1,_pt_ts_to,_pt_objprop};

class FormulaObjTs :public Formula
{
public:



    explicit FormulaObjTs();
    ~FormulaObjTs(){}


    bool elementsAccept(SignalsManager *SM,SignalDescriptionTextInterface *SD,QObject *mainObject);// подключаем выставляем параметр
    QList<QObject*> usedObjects() const;



protected:
    bool FormulaElementAccept(TFormulaElement* el,SignalsManager *SM,SignalDescriptionTextInterface *SD,QObject *mainObject);// подключаем выставляем параметр
    bool FormulaElementSetR(TFormulaElement* el);

};

#endif // FORMULAOBJTS_H
