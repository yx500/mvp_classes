#ifndef GROUPBASEOBJECTS_H
#define GROUPBASEOBJECTS_H
#include "baseobject.h"
class GtBuffer;

// основной контентейнер, которы может храниться внешне
class LIBSHARED_EXPORT GroupBaseObjects : public BaseObject
{
    Q_OBJECT
public:
    Q_INVOKABLE GroupBaseObjects(QObject *parent = nullptr);
    virtual ~GroupBaseObjects(){}

    virtual void updateAfterLoad();

    void connectPropertyChangedSlot();
    QList<GtBuffer *> registrateUsedGtBuffers() const ;

    QString XMLfile() const {return xmlFile;}
    void setXMLfile(QString p){xmlFile=p;}
    virtual void updateStates(); // основной жизненный цикл

signals:
    void ChildObjectPropertyChanged(QObject *O);


protected:
    QString xmlFile;


private slots:
    void slotChildObjectPropertyChanged(QObject *O);
};

#endif // GROUPBASEOBJECTS_H
