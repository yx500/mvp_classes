#include "v_model.h"

#include <QMetaProperty>
#include <QDebug>

#include "baseobjecttools.h"

#include "mvp_objectfactory.h"
//REGISTERELEMENT(v_Model,"","Модель")
REGISTERPROPERTY(v_Base,LNKMODEL,"МОДЕЛЬ","Используемый объект модели","")
REGISTERCOLOR(v_Base,modelNotAccepted,"Модель не подвязана",Qt::gray);

v_Model::v_Model(QObject *parent) : v_Base(parent)
{
    FLNKMODEL.clear();
    FINVERTDRAWX=false;
    FINVERTDRAWY=false;
    resetStates();
}

void v_Model::resetStates()
{
    FSTATE_33=false;
}

quint64 v_Model::id() const
{
    //if (modelObject()!=0) return modelObject()->id().modelId();
    return v_Base::id();
}

QString v_Model::idstr() const
{
    if (modelObject()!=0) return QString("#V%1").arg(modelObject()->idstr());
    return "";
}


void v_Model::updateAfterLoad()
{
    v_Base::updateAfterLoad();
    if (FLNKMODEL.id()!=0){
        BaseObject *B=superFindObjectById(this,FLNKMODEL.id());
        if (!B){
            qCritical() << "Ошибочная ссылка LNKMODEL ! " << objectName();
        }
        setModelObject(B);
    }
}

ObjectLink v_Model::LNKMODEL() const
{
    return FLNKMODEL;
}

void v_Model::setLNKMODEL(ObjectLink p)
{
    if (FLNKMODEL!=p){
        FLNKMODEL=p;
        updateLink(FLNKMODEL);
        if (!FLNKMODEL.isNull()) {
            BaseObject *B=FLNKMODEL.baseObject();
            if (B){
                setObjectName(B->objectName());
                setModelObject(B);
                setMK_ID(B->id());;
            }
        }
    }
}
void v_Model::setModelObject(BaseObject *B)
{
    vCommonPropertyes.clear();
    FLNKMODEL.linkObj(B);
    if(B!=nullptr){
        for (int i=0;i<metaObject()->propertyCount();i++){
            _tCommonProperty cp;
            cp.VP=metaObject()->property(i);
            if (cp.VP.isStored(this)) continue;
            for (int j=0;j<B->metaObject()->propertyCount();j++){
                if (QString(metaObject()->property(i).name())==(QString(B->metaObject()->property(j).name()))){
                    cp.MP=B->metaObject()->property(j);
                    vCommonPropertyes.push_back(cp);
                    break;
                }
            }
        }

        connect(B,SIGNAL(propertyChanged(QObject*)),this,SLOT(slotModelPropertyChanged(QObject*)));
        connect(B,SIGNAL(stateChanged(QObject*)),this,SLOT(slotModelStateChanged(QObject*)));
    }
}

m_Base *v_Model::modelObject() const
{
    return qobject_cast<m_Base *>(FLNKMODEL.baseObject());
}


QVariant v_Model::modelProperty(const QString &propertyName) const
{
    if (modelObject()!=nullptr) {
        if (modelObject()->metaObject()->indexOfProperty(qPrintable(propertyName))>=0){
            return modelObject()->property(qPrintable(propertyName));
        }
    }
    return QVariant();
}

void v_Model::updateStates()
{
    v_Base::updateStates();

}

void v_Model::updateStatesFromModel()
{
    // обновляем свойства от моделей
    // обновление на случай не отрабатывания увязки модель-> отображение
    // 2 пути - работать с пропертями и забыть конкретный класс модель
    // или напрямую с вызовом функций
    //    if (modelObject()!=nullptr)
    //        setSTATE_33(modelObject()->STATE_33());
    m_Base * M=modelObject();
    if (M!=nullptr){
        foreach (const _tCommonProperty &cp, vCommonPropertyes) {
            cp.VP.write(this,cp.MP.read(M));
        }
        if (FSTATE_MK_FLAG==0) {
            t_mk_flag mk_flag;
            ExternFlags::getFlag(FEXTFLAGSRC,0/*src*/,M->id(),mk_flag);
            setSTATE_MK_FLAG(mk_flag.type);
        }
    }


    //SET_STATE_MODEL_BOOL(STATE_33);
}

void v_Model::d_paint(QPainter *painter, const QStyleOptionGraphicsItem *)
{
    if (ISEDITORMODE){
        QFont FFONT;
        FFONT.setPointSizeF(10);
        painter->setFont(FFONT);
        painter->setPen(Qt::black);
        painter->setBrush(Qt::NoBrush);


        painter->drawText(boundingRect(),0,modelObject()->objectName());
    }
}

void v_Model::slotModelPropertyChanged(QObject *O)
{
    if (O==modelObject()){
        update();
    }
}

void v_Model::slotModelStateChanged(QObject *O)
{
    if (O==modelObject()){
        updateStatesFromModel();
        updateStates();
    }
}

bool v_Model::isNotAccepted33(QColor &clrB)
{
    m_Base *M=qobject_cast<m_Base *>(FLNKMODEL.baseObject());
    if (!M) {
        clrB=getColor(Color_modelNotAccepted);
        return true;
    }
    if (M->STATE_33()){
        clrB=getColor(Color_state_33);
        return true;
    }
    return false;
}

bool v_Model::modelStateChanged()
{
    m_Base *M=modelObject();
    if (!M) return false;
    if (M->stateChangedCount()!=_modelStateChangedCount){
        _modelStateChangedCount=M->stateChangedCount();
        return true;
    }
    return false;
}


bool v_Model::modelPropertyBool(QString propertyName, bool defaultValue) const
{
    if (!modelObject()) return defaultValue;
    QVariant V=modelObject()->property(qPrintable(propertyName));
    if (V.isValid()) return V.toBool();
    return defaultValue;

}
int v_Model::modelPropertyInt(QString propertyName, int defaultValue) const
{
    if (!modelObject()) return defaultValue;
    QVariant V=modelObject()->property(qPrintable(propertyName));
    if (V.isValid()) return V.toInt();
    return defaultValue;
}

qreal v_Model::modelPropertyQreal(QString propertyName, qreal defaultValue) const
{
    if (!modelObject()) return defaultValue;
    QVariant V=modelObject()->property(qPrintable(propertyName));
    if (V.isValid()) return V.toReal();
    return defaultValue;
}

QString v_Model::modelPropertyesStr(QString propertyName, QString defaultValue) const
{
    if (!modelObject()) return defaultValue;
    QVariant V=modelObject()->property(qPrintable(propertyName));
    if (V.isValid()) return V.toString();
    return defaultValue;

    //    BaseObject *B=modelObject();
    //    if (B==0) return S;
    //    int i1;
    //    int i2;
    //    QString S1=S;
    //    S1.replace("$$","");
    //    i1=S1.indexOf("$",0);
    //    i2=S1.indexOf("$",i1+1);
    //    while ((i1>=0)&&(i2>=0)){
    //        i2=S1.indexOf("$",i1+1);
    //        if (i2<=i1) break;
    //        QString pn0=S1.mid(i1,i2);
    //        QString pn=S1.mid(i1+1,i2-1);
    //        if (B->metaObject()->indexOfProperty(qPrintable(pn))>=0){
    //            QVariant v=B->property(qPrintable(pn));
    //            QString ps=v.toString();
    //            S.replace(pn0,ps);
    //            S1.replace(pn0,ps);
    //        }
    //        i1=S1.indexOf("$",0);
    //        i2=S1.indexOf("$",i1+1);
    //    }
    //    return S;
}
