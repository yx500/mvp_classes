#ifndef V_SVET_GMC4_H
#define V_SVET_GMC4_H

#include "v_svet_gor_4n.h"

class LIBSHARED_EXPORT v_Svet_GMC4 : public v_Svet_Gor_4N
{
    Q_OBJECT
    MYSTATE(bool, STATE_DBK_ERROR)
public:
    Q_INVOKABLE v_Svet_GMC4(v_Base *parent = 0);
    virtual ~v_Svet_GMC4(){}

signals:

public slots:

protected:
    virtual void updateStatesFromModel();

};

#endif // V_SVET_GMC4_H
