#ifndef V_BASE_H
#define V_BASE_H

#include <QRectF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "proxygraphicsitem.h"

#include "externflags.h"
#include "signaldescription.h"
#include "mcolorpalette.h"

#include "baseobject.h"

#include "visualgrouppropertyes.h"


class Visual_Screen;




#define MYPROPS(type,n) Q_PROPERTY(type n READ n WRITE set##n DESIGNABLE true ) \
    protected: \
    type F##n; \
    public: \
    type n(){return F##n;}


#define DECLARECOLOR(color) static int Color_##color;
#define REGISTERCOLOR(class_name,idstr,rusname,color) int class_name::Color_##idstr=MColorPalette::instance()->registerColor(#idstr,rusname,color)
#define REGISTERCOLOR2(class_name,idstr,rusname,color1,color2) int class_name::Color_##idstr=MColorPalette::instance()->registerColor(#idstr,rusname,color1,color2)





#define ISEDITORMODE isSceneEditorMode()






class LIBSHARED_EXPORT v_Base : public BaseObject
{
    Q_OBJECT
    Q_FLAGS(TExtStates)

    Q_PROPERTY(QPointF XY READ xy WRITE setxy DESIGNABLE true)
    // QGraphicsItem
    Q_PROPERTY(qreal Z READ zValue WRITE setZValue NOTIFY geometryChange DESIGNABLE true)
    Q_PROPERTY(qreal OPACITY READ opacity WRITE setOpacity NOTIFY geometryChange DESIGNABLE true)
    Q_PROPERTY(qreal ROTATION READ rotation WRITE setRotation NOTIFY geometryChange DESIGNABLE true)
    Q_PROPERTY(qreal SCALE READ scale WRITE setScale NOTIFY geometryChange DESIGNABLE true)
    Q_PROPERTY(bool STATE_VISIBLE READ isVisible WRITE setVisible DESIGNABLE true STORED false)
    Q_PROPERTY(bool STATE_SELECTED READ isSelected WRITE setSelected DESIGNABLE true STORED false)

    MYPROP(bool,  INVERTDRAWX)
    MYPROP(bool,  INVERTDRAWY)
    MYPROP(QRectF,SELECTRECT)
    MYPROP(QRectF,FLAGRECT)
    MYPROP(int,  LEVELDETAIL)
    MYPROP(QVariantHash,COMMON_PROPERTYESH)

    MYPROP(quint64,MK_ID)
    MYPROP(QString,EXTFLAGSRC)
    MYSTATE(int,STATE_MK_FLAG)

    public:
        DECLARECOLOR(FON)

    void addCOMMON_PROPERTY(QString selfProperty,QString groupProperty){FCOMMON_PROPERTYESH[selfProperty]=groupProperty;}


    void setxy(QPointF p); // в kоординатах сетки
    QPointF xy() const;
    // v_Base -> QGraphicsItem
    qreal zValue() const;
    void setZValue(qreal z);
    qreal opacity() const;
    void setOpacity(qreal opacity);
    void setRotation(qreal angle);
    qreal rotation() const;
    void setScale(qreal scale);
    qreal scale() const;
    bool isVisible() const;
    void setVisible(bool visible);
    bool isSelected() const;
    void setSelected(bool selected);
    void prepareGeometryChange();
    void update();
    // v_Base -> QGraphicsItem

public:
    DECLARECOLOR(object_selected)
    DECLARECOLOR(state_33)
    DECLARECOLOR(modelNotAccepted)
    DECLARECOLOR(flag_mk_normal)
    DECLARECOLOR(flag_mk_predotkaz)
    DECLARECOLOR(flag_mk_otkaz)
    DECLARECOLOR(flag_op_focus)
    DECLARECOLOR(flag_op_warning)
    DECLARECOLOR(flag_op_critical)


    Q_INVOKABLE v_Base(QObject *parent = nullptr);
    virtual ~v_Base();

    ProxyGraphicsItem *getProxyGraphicsItem(){return proxyGraphicsItem;}

    virtual void validation(ListObjStr *l) const;

    virtual void updateAfterLoad();

    virtual int gridDX() const;  // размер сетки от view
    virtual int gridDY() const;
    QPointF grid2point(QPointF gridFM) const;
    QPointF point2grid(QPointF p) const;      // точка в координатах сетки
    QPointF point2Parent(QPointF p,v_Base *par);      // точка в координатах родителя
    QPointF parent2point(QPointF p,v_Base *par);      // точка родителя в координатах дитя




    // Drawing
    virtual void calculateGeometry(); // перестраивает внтренние координаты
    virtual QRectF boundingRect() const ;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
    virtual void advance(int phase);
    virtual void drawBackground(QPainter * painter)  ;
    virtual void drawForeground(QPainter * painter)  ;

    QColor getColor(int uid);
    QColor getColor(const QString &idstr);
    void startChekingColorAnimate();
    bool isAnimatedColorUsed();

    Visual_Screen *screen() const ;
    virtual void setScreen(Visual_Screen *p);


    QRectF groupBoundingRect() const;
    QPainterPath groupShape() const;
    void groupPaint(QPainter *painter,const QStyleOptionGraphicsItem *option);

    virtual bool isCanAddObject(QObject *O) const;
    virtual void addObject(QObject *O);

    static QList<ProxyGraphicsItem *> findChildren_ProxyGraphicsItem (ProxyGraphicsItem *pi);

    template<typename T>
    inline QList<T> findChildrenVisual(Qt::FindChildOptions options = Qt::FindChildrenRecursively) const
    {
        QList<T> list;
        foreach(QGraphicsItem * gi,proxyGraphicsItem->childItems()){
            ProxyGraphicsItem *pi1=qgraphicsitem_cast<ProxyGraphicsItem *>(gi);
            if (pi1!=nullptr) {
                v_Base *v=pi1->getvBase();
                T vt=qobject_cast<T>(pi1->getvBase());
                if (vt) {
                    list.push_back(vt);
                }
                if (options==Qt::FindChildrenRecursively){
                    if (v){
                        QList<T> slist=v->findChildrenVisual<T>(Qt::FindChildrenRecursively);
                        list+=slist;
                    }
                }
            }
        }
        return list;
    }


    v_Base *visualParent() const;
    void setVisualParent(v_Base *v);



    virtual QMap<int,QPointF> getMarkersInfo() const;  // исполльзуемы маркеры
    virtual void moveMarker(int markerId,QPointF xyItem);


    virtual QPolygonF centreLine() const; // возвращает осевую лдя елемента


signals:
    void geometryChange(QObject *O); // сообщаю что изменилась размеры
public slots:
    virtual void updateStates(); // основной жизненный цикл

protected:
    ProxyGraphicsItem * proxyGraphicsItem;
    bool isSceneEditorMode() const;

    QPointF Fxy;  // в коордианатах сетки
    virtual void d_paint(QPainter *painter, const QStyleOptionGraphicsItem *option){Q_UNUSED(painter);Q_UNUSED(option)}
    void drawBackgroundFLAG(QPainter *painter,int flag);
    void drawForegroundFLAG(QPainter *painter, int flag);
    bool colorAnimate;

    QRectF _boundingRect;
    QPainterPath _shape;

    virtual void doPropertyChanged(){
        prepareGeometryChange();
        calculateGeometry();
        emit geometryChange(this);
        BaseObject::doPropertyChanged();
    }
    virtual void doStateChanged(){
        update();
        BaseObject::doStateChanged();
    }

    QRectF flagRect() const;
};


#endif // V_BASE_H
