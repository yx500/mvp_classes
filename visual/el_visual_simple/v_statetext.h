#ifndef V_STATETEXT_H
#define V_STATETEXT_H

#include "v_stat_text.h"

class v_StateText : public v_StaticText
{
    Q_OBJECT
    Q_PROPERTY(ObjectLink TARGET READ TARGET WRITE setTARGET DESIGNABLE true )
    Q_PROPERTY(QString STEXT READ TEXT WRITE setTEXT DESIGNABLE true )
public:
    ObjectLink TARGET() const {return FTARGET;}
    void setTARGET(ObjectLink p);
    QString TEXT() const {return FTEXT;}
    void setTEXT(QString p);


    Q_INVOKABLE v_StateText(v_Base *parent = nullptr);
    virtual ~v_StateText(){}

    virtual void updateAfterLoad();

    virtual QString getText();

    virtual void updateStates();
public slots:

    virtual void slotModelStateChanged(QObject *);  // следим за моделью


signals:
protected:
    ObjectLink FTARGET;
    QString FTEXT;
    QList<QString> l_prop_name;
    QString outtext;
    BaseObject*bo;

};

#endif // V_STATETEXT_H
