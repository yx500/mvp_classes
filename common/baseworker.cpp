#include "baseworker.h"

BaseWorker::BaseWorker(QObject *parent) : BaseObject(parent)
{
    FSTATE_ENABLED=true;
}
