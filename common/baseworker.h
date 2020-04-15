#ifndef BASEWORKER_H
#define BASEWORKER_H

#include "baseobject.h"

class BaseWorker : public BaseObject
{
    Q_OBJECT
    MYSTATE(bool,STATE_ENABLED)
public:
    explicit BaseWorker(QObject *parent = nullptr);
    virtual ~BaseWorker(){}

    void setWorkLevel(int p){workLevel =p;}
    int getWorkLevel() const {return workLevel;}

    virtual void work(const QDateTime &T){Q_UNUSED(T)}
    virtual void state2buffer(){}
signals:

public slots:
protected:
    int workLevel=0;
};

#endif // BASEWORKER_H
