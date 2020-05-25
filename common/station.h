#ifndef STATION_H
#define STATION_H

#include "baseobject.h"

class QTimer;

// универсальный контейнер для всего

class Station : public BaseObject
{
    Q_OBJECT
    Q_PROPERTY(int UPDATE_STATES_INTERVAL READ UPDATE_STATES_INTERVAL WRITE setUPDATE_STATES_INTERVAL DESIGNABLE true)

public:
    int UPDATE_STATES_INTERVAL() const;
    void setUPDATE_STATES_INTERVAL(int p);

public:
    Q_INVOKABLE Station(QObject *parent = nullptr);
    virtual ~Station();

    virtual void updateAfterLoad();

    virtual bool isCanAddObject(QObject *O) const;


public slots:
    void slot_updateStates(); // основной жизненный цикл

signals:

protected:
    QTimer *timerUPDATE_STATES;


};



#endif // STATION_H
