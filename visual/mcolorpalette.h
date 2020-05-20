#ifndef MCOLORPALETTE_H
#define MCOLORPALETTE_H
/*
 * палитра
 * */
#include "mcolor.h"
#include <QMap>
#include "baseobject.h"
#include <QElapsedTimer>

class MColorPalette : public BaseObject
{
    Q_OBJECT
public:
    Q_INVOKABLE MColorPalette(QObject *parent = nullptr);
    virtual ~MColorPalette();

    int registerColor(QString idstr,QString name,Qt::GlobalColor color);
    int registerColor(QString idstr,QString name, Qt::GlobalColor color1, QColor color2);
    MColor *getMColor(int uid) const ;
    MColor *getMColor_idstr(QString idstr) ;
    QColor color(int uid) const;
    QColor color(QString idstr) ;
    bool existsColor(QString idstr) const;
    bool existsUnLoadedColors() const {return FexistsUnLoadedColors;}

    static MColorPalette* instance(){
           static MColorPalette one;
           return &one;
    }



    virtual void updateAfterLoad();
    virtual bool isCanAddObject(QObject *) const;
    virtual void addObject(QObject *);
    void load(QString fn);
    static QString rgbString(QColor c);

    qreal animateK(int ms_peroid) const;


signals:

public slots:
    virtual void updateStates(){}
    void updateTimer();
protected:
    QMap<int , MColor*> m_UID2MColor;
    QMap<QString , MColor*> m_IDSTR2MColor;
    QTimer *timer;
    QElapsedTimer timerStart;
    qlonglong timer_ms_elapsed;
    bool FexistsUnLoadedColors;


};

QColor Color2Color(QColor c1,QColor c2,qreal procent);
QColor mvpColor(QString idstr);


#endif // MCOLORPALETTE_H
