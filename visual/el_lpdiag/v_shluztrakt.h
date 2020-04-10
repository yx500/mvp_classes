#ifndef V_SHLUZTRAKT_H
#define V_SHLUZTRAKT_H

#include "v_lpdiag.h"
class LIBSHARED_EXPORT v_ShluzTrakt : public v_LPDiag
{
    Q_OBJECT
public:
    Q_INVOKABLE v_ShluzTrakt(v_Base *parent = nullptr);
    virtual ~v_ShluzTrakt(){}
    virtual void setNORDER(int p);
    virtual void advance(int phase){}
    virtual void updateStates(){}
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

    virtual void calculateGeometry(); // перестраивает внтренние координаты

signals:

public slots:
};

#endif // V_SHLUZTRAKT_H
