#ifndef OBJECTIDGENERATOR_H
#define OBJECTIDGENERATOR_H

/*
 *  функция генерации уникального ID
 * */



#include <QDate>

static QDateTime time0;
static quint64 FlastId;

class quint64Generator
{
public:

    quint64Generator(){
        time0=QDateTime(QDate(2015,01,01),QTime(0,0));
        FlastId=0;
    }

    static quint64 lastId(){return FlastId;}
    static quint64Generator* instance(){
        static quint64Generator one;
        return &one;
    }
    quint64 generate(){
        quint64 id=QDateTime::currentDateTime().msecsTo(time0);
        if (id<=FlastId) id=FlastId+1;
        FlastId=id;
        return id;
    }
private:




};


#endif // OBJECTIDGENERATOR_H
