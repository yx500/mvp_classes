#ifndef M_IPD_H
#define M_IPD_H

#include "m_rtds.h"

class m_IPD : public m_RTDS
{
    Q_OBJECT
public:
    Q_INVOKABLE m_IPD(QObject *parent = 0);
    virtual ~m_IPD(){}

signals:

public slots:
};

#endif // M_IPD_H
