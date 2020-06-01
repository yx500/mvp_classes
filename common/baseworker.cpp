#include "baseworker.h"

BaseWorker::BaseWorker(QObject *parent) : BaseObject(parent)
{
    FSTATE_ENABLED=true;
}

QList<SignalDescription> BaseWorker::acceptOutputSignals()
{
    QList<SignalDescription> l;
    return l;
}
