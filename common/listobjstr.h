#ifndef LISTOBJSTR_H
#define LISTOBJSTR_H

/*
 * Список строк увязанных с объектом
 * */

#include <QObject>
#include <QPointer>

class ListObjStr
{

public:
    struct ObjStr{
        QPointer<const QObject> O;
        QString type;
        QString text;
        QString comment;
        QString prop_name;
    };

    ListObjStr(){}
    ListObjStr(const ListObjStr &other){
        l=other.list();
    }
    ~ListObjStr(){}

    QList<ObjStr> list() const {
        return l;
    }
    QList<ObjStr> *plist()  {
        return &l;
    }
    void message(const QObject *O,QString type,QString text,QString comment=QString(),QString prop_name=QString()){
        ObjStr os;
        os.O=O;
        os.type=type;
        os.text=text;
        os.comment=comment;
        os.prop_name=prop_name;
        l.push_back(os);
    }

    void warning(const QObject *O,QString text,QString comment=QString(),QString prop_name=QString()){
        message(O,"Предупреждение",text,comment,prop_name);
    }
    void error(const QObject *O,QString text,QString comment=QString(),QString prop_name=QString()){
        message(O,"Ошибка",text,comment,prop_name);
    }
    void clear(){l.clear();}
    void append(ListObjStr &l1){
        for (int i=0;i<l1.plist()->size();i++)
            l.push_back(l1.plist()->at(i));
    }
    QStringList toStringList(){
        QStringList sl;
        for (int i=0;i<l.size();i++){
            if (l.at(i).O)
                sl.push_back(QString("%1 %2 %3").arg(l.at(i).O->objectName()).arg(l.at(i).text).arg(l.at(i).comment)); else
                sl.push_back(QString("%1 %2 %3").arg(" --- ").arg(l.at(i).text).arg(l.at(i).comment));
        }
        return sl;
    }

protected:
    QList<ObjStr> l;
};

#endif // LISTOBJSTR_H
