/*
  расширение работы с логическими выражениями
  добавление обработки ТС и свойств QObject
*/


#include "formulaobjts.h"

#include <QVariant>

#include "signalsmanager.h"
#include "signalchanel.h"

#include "signaldescriptiontextinterface.h"

//---------------------------------------------------------------------------


FormulaObjTs::FormulaObjTs() :Formula()
{
}

bool FormulaObjTs::elementsAccept(SignalsManager *SM, SignalDescriptionTextInterface *SD, QObject *mainObject)
{
    error=false;
    foreach (TFormulaElement *el, vFormulaElement) {
        if (!FormulaElementAccept(el,SM,SD,mainObject)) error=true;
    }
    return !error;
}

static char _id_str_property_name[]="IDSTR";
QList<QObject *> _findObjectByIdstr(QObject *O, const QString &idstr)
{
    QList<QObject *> l;
    if (O->property(_id_str_property_name).toString()==idstr) l.push_back(O);
    foreach(QObject * o,O->children()){
        QList<QObject *> l1=_findObjectByIdstr(o,idstr);
        if (!l1.empty()) l+=l1;
    }
    return l;
}


bool FormulaObjTs::FormulaElementAccept(TFormulaElement *el,SignalsManager *SM, SignalDescriptionTextInterface *SD, QObject *mainObject)
{
    // число
    bool ok;
    int n=el->ElStr.toInt(&ok);
    if (ok){
        el->tag_int[0]=n;
        el->Tip=_pt_number;
        return true;
    }
    // если ффф.ммм
    if (mainObject!=nullptr){
        int p=el->ElStr.indexOf('.');
        if ((p>0)&&(p<el->ElStr.size()-2)){
            el->tag_str[0]=el->ElStr.left(p);
            el->tag_str[1]=el->ElStr.mid(p+1);
            el->Tip=_pt_objprop;
            // расставляем объекты

            QList<QObject *> l=_findObjectByIdstr(mainObject, el->tag_str[0]);
            if (l.empty()){
                err(QString("объект не найден %1").arg(el->tag_str[0]));
                return false;
            } 
                if (l.size()>1){
                    err(QString("%2 объектов %1").arg(el->tag_str[0]).arg(l.size()));
                    return false;
                } 
                    el->tag_object[0]=l.first();
                    QVariant value = el->tag_object[0]->property(qPrintable(el->tag_str[1]));
                    if (value.isValid()) {
                        return true;
                    }  
                        err(QString("%1 нет св-ва %2").arg(el->tag_str[0]).arg(el->tag_str[1]));
                        el->tag_object[0]=nullptr;
                        return false;
                    
                
            
        }
    }
    if (SD!=nullptr){
        // если не число то ТС
        if (!el->ElStr.isEmpty()){
            el->tag_str[0]=el->ElStr;
            el->Tip=_pt_ts_val;

            static QString _strT0="#T0_";
            static QString _strT1="#T1_";
            int i=el->ElStr.indexOf(_strT0);
            if (i==0){
                el->Tip=_pt_ts_to;
                el->tag_int[1]=0;
                el->tag_str[0]=el->ElStr.right(i);
            } else {
                i=el->ElStr.indexOf(_strT1);
                if (i==0){
                    el->Tip=_pt_ts_to;
                    el->tag_int[1]=1;
                    el->tag_str[0]=el->ElStr.right(i);
                }
            }
            el->tag_object[0]=nullptr;
            el->tag_int[0]=0;
            QVector<SignalDescription> fs=SD->FindSignals(el->tag_str[0]);
            if (fs.empty()){
                err(QString("ТС %1 не наден").arg(el->tag_str[0]));
                return false;
            }
            if (fs.size()>1){
                err(QString("ТС %1 продублирован %2 раза").arg(el->tag_str[0]).arg(fs.size()));
                return false;
            }
            if (fs.size()==1){
                SignalChanel *chanel=SM->getSignalChanel(1,fs.first().chanelName());
                if (chanel==nullptr){
                    err(QString("Канал отсутствует %1 ").arg(fs.first().chanelName()));
                }
                el->tag_object[0]=chanel;
                el->tag_int[0]=fs.first().chanelOffset();
            }
            return true;
        }
    }
    return false;
}


//bool value_1bit(GtBuffer *B,int FChanelOffset)
//{
//    int sz=0;
//    if (B==nullptr) return 0;

//    const char *A=B->A.data();

//    quint16 n_im=FChanelOffset;
//    quint16 nbyte = n_im/8;
//    quint8 nbit = n_im%8;
//    // тут эмуляция старого пакета ТС 8 бит, со смещением начала на 1 байт
//    if (B->type==1) nbyte++;
//    if (nbyte>=sz) return 0;
//    const quint8 *TSBUF=(const quint8 *)A;
//    quint8 r=( (TSBUF[nbyte]>>nbit) & 0x01 ) ? 1 : 0;
//    return r;
//}

bool FormulaObjTs::FormulaElementSetR(TFormulaElement *el)
{
    el->R=0;
    if (el->Tip == _pt_number) {
        el->R=el->tag_int[0];
        return true;
    }
    if (el->Tip == _pt_ts_val) {
        auto chanelOffset=static_cast<quint16>(el->tag_int[0]);
        auto *chanel=dynamic_cast<SignalChanel *>(el->tag_object[0].data());
        if (chanel){
            el->R= chanel->getSignal1bit(chanelOffset);
            return true;
        }
        return false;
    }
    if (el->Tip == _pt_ts_to) {
        auto chanelOffset=static_cast<quint16>(el->tag_int[0]);
        auto *chanel=dynamic_cast<SignalChanel *>(el->tag_object[0].data());
        if (chanel){
            int r=chanel->getSignal1bit(chanelOffset);
            if (el->tag_int[1]==0){
                if (r==0) el->R=static_cast<int>(chanel->getSignal1bitTms(chanelOffset));
            }
            if (el->tag_int[1]==1){
                if (r==1) el->R=static_cast<int>(chanel->getSignal1bitTms(chanelOffset));
            }
            return true;
        }
        return false;
    }
    if (el->Tip == _pt_objprop){
        if (el->tag_object[0]){
            QVariant value = el->tag_object[0]->property(qPrintable(el->tag_str[1]));
            if (value.isValid()) {
                el->R=value.toInt();
                return true;
            }
        }
    }
    return false;
}




QList<QObject *> FormulaObjTs::usedObjects() const
{
    QList<QObject *>l;
    for (auto el : vFormulaElement) {
        if (el->Tip == _pt_objprop){
            if (el->tag_object[0]) l.push_back(el->tag_object[0]);
        }
    }
    return l;

}






//QString FormulaObjTs::fevFormula2St()
//{
//    QString szout = "";
//    for (TvFormulaElement::iterator i = vFormulaElement.begin(); i != vFormulaElement.end(); ++i)
//        szout += i-> szVal;
//    return szout.c_str();
//}



//const char * FormulaObjTs::feGetSimpleFormula(const char * szFormula)
//{
//    TvFormulaElement vFormulaElement;
//    feSt2vFormula(szFormula, vFormulaElement);
//    return fevFormula2StN(vFormulaElement);
//}



//---------------------------------------------------------------------------







