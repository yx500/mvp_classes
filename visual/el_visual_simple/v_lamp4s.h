#ifndef V_LAMP4S_H
#define V_LAMP4S_H

#include "v_stat_text.h"

class v_Lamp4s : public v_StaticText
{
    Q_OBJECT
    MYPROP(ObjectLink, LNKMODEL2)
    MYPROP(ObjectLink, LNKMODEL3)
    MYPROP(ObjectLink, LNKMODEL4)
    MYPROP(QString, STATE1)
    MYPROP(QString, STATE2)
    MYPROP(QString, STATE3)
    MYPROP(QString, STATE4)
    Q_PROPERTY(QVariantHash COLOR_TEXT_16 READ getCOLOR_TEXT_16 WRITE setCOLOR_TEXT_16 DESIGNABLE true )
    Q_PROPERTY(QVariantHash COLOR_RECT_16 READ getCOLOR_RECT_16 WRITE setCOLOR_RECT_16 DESIGNABLE true )
    Q_PROPERTY(QVariantHash COLOR_BRUSH_16 READ getCOLOR_BRUSH_16 WRITE setCOLOR_BRUSH_16 DESIGNABLE true )


    QVariantHash getCOLOR_TEXT_16() const{return FCOLOR_TEXT_H;}
    void setCOLOR_TEXT_16(QVariantHash &p);
    QVariantHash getCOLOR_RECT_16() const{return FCOLOR_RECT_H;}
    void setCOLOR_RECT_16(QVariantHash &p);
    QVariantHash getCOLOR_BRUSH_16() const{return FCOLOR_BRUSH_H;}
    void setCOLOR_BRUSH_16(QVariantHash &p);

public:
    Q_INVOKABLE v_Lamp4s(v_Base *parent = nullptr);
    virtual ~v_Lamp4s(){}
    virtual void updateAfterLoad();// восстановление параметров после загрузки(линки)

    virtual void updateStates();
    virtual void updateStatesFromModel();

    virtual QString getText(){return objectName();}
    virtual QColor getTextColor();
    virtual QColor getRectColor();
    virtual QColor getBrusColor();

signals:
protected:
  std::array<QString, 16> FCOLOR_RECT_16;
  std::array<QString, 16> FCOLOR_BRUSH_16;
  std::array<QString, 16> FCOLOR_TEXT_16;
    QVariantHash FCOLOR_TEXT_H;
    QVariantHash FCOLOR_RECT_H;
    QVariantHash FCOLOR_BRUSH_H;
    int FSTATE_SIGNAL;
    BaseObject *B[4];
    QString S[4];

};

#endif // V_LAMP4S_H
