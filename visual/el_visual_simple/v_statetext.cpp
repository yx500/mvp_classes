#include "v_statetext.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_StateText,"СвТекст","VISUAL Простые элементы")

void v_StateText::setTARGET(ObjectLink p)
{
    if (FTARGET!=p){
        FTARGET=p;
        bo=nullptr;
        if (FTARGET.id()!=0){
            bo=updateLink(FTARGET);
        }
        calculateGeometry();
    }
}

void v_StateText::setTEXT(QString p)
{
    if (FTEXT!=p){
        FTEXT=p;

        l_prop_name.clear();
        auto ls=FTEXT.split("%");
        for (int i=1;i<ls.size();i+=2){
            l_prop_name.push_back(ls[i]);
        }



        calculateGeometry();
    }
}

v_StateText::v_StateText(v_Base *parent) : v_StaticText(parent)
{
    setObjectName("ТЕКСТ");
}

void v_StateText::updateAfterLoad()
{
    v_StaticText::updateAfterLoad();
    bo=nullptr;
    if (FTARGET.id()!=0){
        bo=updateLink(FTARGET);
    }
    if(bo!=nullptr){
        connect(bo,SIGNAL(stateChanged(QObject*)),this,SLOT(slotModelStateChanged(QObject*)));
    }
}

QString v_StateText::getText()
{
    if (ISEDITORMODE){
        return objectName();
    }
    return outtext;
}

void v_StateText::updateStates()
{
    outtext=FTEXT;
    if (bo!=nullptr){
        foreach (auto prop_name, l_prop_name) {
            QVariant V=bo->property(qPrintable(prop_name));
            if (V.isValid()){
                outtext=outtext.replace("%"+prop_name+"%",V.toString());
            }
        }

    }
}

void v_StateText::slotModelStateChanged(QObject *)
{
    updateStates();
    calculateGeometry();
    update();
}
