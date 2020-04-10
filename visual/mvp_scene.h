#ifndef MVP_SCENE_H
#define MVP_SCENE_H
/* базовый класс сцены для системы MVP
 * */

#include <QGraphicsScene>
#include <QMenu>
#include <QTimer>
#include "v_base.h"
#include "visual_screen.h"


class TSceneDrawMode{
public:
    bool isEditor;
};

class MVP_Scene: public QGraphicsScene
{
    Q_OBJECT
public:

    explicit MVP_Scene(QObject *parent = nullptr);
    virtual ~MVP_Scene();


    virtual void setVisual_Screen(Visual_Screen *p);
    Visual_Screen *visual_Screen() const {return FVisual_Screen;}

    int animatePeriod() const {return timerAnimate->interval();}
    void setAnimateInterval(int p){timerAnimate->setInterval(p);}
    int ivalidateInterval() const {return timerInvalidate->interval();}
    void setIvalidateInterval(int p){timerInvalidate->setInterval(p);}
    int updateStateInterval() const ;
    void setUpdateStateInterval(int p);


    QList<v_Base *> selectedV();
    QList<v_Base *> itemsV(const QPointF &pos, Qt::ItemSelectionMode mode = Qt::IntersectsItemShape, Qt::SortOrder order = Qt::DescendingOrder, const QTransform &deviceTransform = QTransform()) const;
    QList<v_Base *> itemsV(Qt::ItemSelectionMode mode = Qt::IntersectsItemShape, Qt::SortOrder order = Qt::DescendingOrder) const;

    bool isMyItem(QObject *O);
    v_Base *ItemByBaseModelElement(QObject *B);
    void removeAllItems();
    void setDrawModeisEditor(bool isEditor);
    TSceneDrawMode drawMode() const {return FDrawMode;}

public slots:
    void slotInvalidate();
    void slotAnimate();
    void slotUpdateState();
signals:

protected:
    Visual_Screen *FVisual_Screen;

    TSceneDrawMode FDrawMode;
    QTimer *timerAnimate;
    QTimer *timerInvalidate;
    QTimer *timerUpdateState;
    int FFPS;

    virtual void drawBackground(QPainter * painter, const QRectF & rect);
    virtual void drawForeground(QPainter * painter, const QRectF & rect);
    virtual void drawFon(QPainter * painter, const QRectF & rect);
    virtual void customDrawForeground(QPainter * painter, QGraphicsItem *qi);
    virtual void customDrawBackground(QPainter * painter, QGraphicsItem *qi);

    virtual QWidget* getWidget(QString type,QString idstr,QString propertyes);
    void addWigets();

};

#endif // MVP_SCENE_H
