#ifndef VISUAL_SCREEN_H
#define VISUAL_SCREEN_H

#include "objectlink.h"
#include "visualgrouppropertyes.h"
#include "visual_elements.h"
#include <QSizeF>


/* содержит
 * ссылку на список элементов
 * под экраны
 * парметры вывода
*/

class Visual_Screen : public v_Base
{
    Q_OBJECT
    Q_PROPERTY(int    GRIDDX READ GRIDDX WRITE setGRIDDX DESIGNABLE true)
    Q_PROPERTY(int    GRIDDY READ GRIDDY WRITE setGRIDDY DESIGNABLE true)
public:
    MYPROP(QSizeF,  VIEWSIZE)
    int GRIDDX() const {return FGRIDDX;}
    int GRIDDY() const {return FGRIDDY;}
    void setGRIDDX(int p);
    void setGRIDDY(int p);
    VisualGroupPropertyes*visualGroupPropertyes() const {return FVisualGroupPropertyes;}
public:
    Q_INVOKABLE Visual_Screen(QObject *parent = 0);
    virtual ~Visual_Screen(){}

    virtual bool isCanAddObject(QObject *O) const;
    virtual void addObject(QObject *O);

    virtual void updateAfterLoad();
    void activate();
    void reCalculateGeometry();


    bool isSceneEditorMode() const{return FisEditorMode;}
    void setSceneEditorMode(bool p){FisEditorMode=p;}

signals:

public slots:
    void updateStates(); // основной жизненный цикл
protected:
    int FGRIDDX;
    int FGRIDDY;
    bool FisEditorMode;
    VisualGroupPropertyes *FVisualGroupPropertyes;
};

#endif // VISUAL_SCREEN_H
