#include "v_base.h"

#include <QMetaProperty>
#include <QDebug>
#include <QWidget>
#include <qmath.h>
#include "visual_screen.h"
#include "baseobjecttools.h"

#include "mvp_objectfactory.h"
//REGISTERELEMENT(v_Base,"v_Base","")

REGISTERPROPERTY(BaseObject,objectName,"ИМЯ","Имя объекта","")
REGISTERPROPERTY2(v_Base,OPACITY,"ПРОЗРАЧНОСТЬ","Прозрачность(0-1)","Основные параметры",0.1,2)
REGISTERPROPERTY(v_Base,XY,"XY","Координаты центра в координатах сетки","Основные параметры")
REGISTERPROPERTY(v_Base,Z,"Z","Высота","Основные параметры")
//REGISTERPROPERTY(v_Base,ENABLED,"ENABLED","Не используется пока","Основные параметры")
//REGISTERPROPERTY(v_Base,VISIBLE,"VISIBLE","Если откл, то стнет невидимым","Основные параметры")
REGISTERPROPERTY(v_Base,ROTATION,"ПОВОРОТ","Поворот относительно центра","Основные параметры")
REGISTERPROPERTY2(v_Base,SCALE,"МАСШТАБ","1=1:1","Основные параметры",0.1,2)

REGISTERPROPERTY(v_Base,INVERTDRAWX,"ИНВЕРС.ГОРИЗОНТ.","Направление отрисовки по горизонтали","Направление отрисовки")
REGISTERPROPERTY(v_Base,INVERTDRAWY,"ИНВЕРС.ВЕРТ.","Направление отрисовки по вертикали","Направление отрисовки")

REGISTERCOLOR(v_Base,FON,"Фон",Qt::lightGray);
REGISTERCOLOR2(v_Base,object_selected,"Элемент выделен",Qt::red,Qt::yellow);
REGISTERCOLOR(v_Base,state_33,"Отсутствие входных данных",Qt::magenta);
REGISTERCOLOR(v_Base,flag_mk_normal,"Флаг МК норма",Qt::green);
REGISTERCOLOR2(v_Base,flag_mk_predotkaz,"Флаг МК предотказ",Qt::yellow,Qt::lightGray);
REGISTERCOLOR2(v_Base,flag_mk_otkaz,"Флаг МК отказ",Qt::red,Qt::lightGray);

REGISTERCOLOR(v_Base,flag_op_focus,"Флаг ОП фокус",Qt::white);
REGISTERCOLOR(v_Base,flag_op_warning,"Флаг ОП внимание",Qt::yellow);
REGISTERCOLOR(v_Base,flag_op_critical,"Флаг ОП авария",Qt::darkRed);



v_Base::v_Base(QObject *parent) :
    BaseObject(parent),
    FINVERTDRAWX(false),
    FINVERTDRAWY(false),
    FSELECTRECT(),
    FFLAGRECT(),
    FLEVELDETAIL(2),
    mCOMMON_PROPERTYESG2S(),
    Fxy(0,0),
    _boundingRect(0,0,5,5),
    _shape()
{
    FEXTFLAGSRC.clear();
    proxyGraphicsItem=new ProxyGraphicsItem(this);
    colorAnimate=false;
    FSTATE_MK_FLAG=0;
    addCOMMON_PROPERTY("LEVELDETAIL","LEVELDETAIL");
}

v_Base::~v_Base()
{
    if (proxyGraphicsItem->scene()) proxyGraphicsItem->setParentItem(0);
    delete proxyGraphicsItem;
}

void v_Base::validation(ListObjStr *l) const
{
    validationEmptySignals(l);
    validationEmptyLinks(l);
}


void v_Base::updateAfterLoad()
{
    BaseObject::updateAfterLoad();
    foreach(QObject * b,children()){
        v_Base *childI=qobject_cast<v_Base *>(b);
        if (childI) childI->proxyGraphicsItem->setParentItem(this->proxyGraphicsItem);
    }
}


//void v_Base::setEXTSTATE(v_Base::TExtState flag, bool p)
//{
//    if (p)
//         FEXTSTATES=FEXTSTATES | flag  ; else FEXTSTATES=FEXTSTATES &~flag  ;
//}



QString v_Base::COMMON_PROPERTYES() const
{
    QStringList sl;
    foreach (const QString &groupName,mCOMMON_PROPERTYESG2S.keys()){
        sl.push_back(QString("%1=%2").arg(mCOMMON_PROPERTYESG2S[groupName]).arg(groupName));
    }
    return sl.join(";");
}

void v_Base::setCOMMON_PROPERTYES(QString &p)
{
    if (COMMON_PROPERTYES()!=p)    {
        mCOMMON_PROPERTYESG2S.clear();
        QStringList sl=p.split(";");
        foreach (QString kv, sl) {
            QString propNameSelf=kv.section('=',0,0);
            QString propNameGroup=kv.section('=',1);
            if (propNameSelf.isEmpty()) continue;
            if (propNameGroup.isEmpty()) continue;
            mCOMMON_PROPERTYESG2S[propNameGroup]=propNameSelf;
            if ((screen())&&(screen()->visualGroupPropertyes())) {
                screen()->visualGroupPropertyes()->add(propNameSelf,propNameGroup,this);
            }

        }
    }
}


void v_Base::setxy(QPointF p)
{
    if (Fxy!=p){
        Fxy=p;
        proxyGraphicsItem->setPos(grid2point(Fxy));
        doPropertyChanged();
    } else {
        if (grid2point(Fxy)!=proxyGraphicsItem->pos())
            proxyGraphicsItem->setPos(grid2point(Fxy));
    }
    // координаты центра не влияют на геометрию
    // но должны же сообщить что изменились
    // сразу пересчитаем и pos
    // нафиг нам поз - мы все время от 0 считаем


}

QPointF v_Base::xy() const
{
    return Fxy;
}

qreal v_Base::zValue() const
{
    return proxyGraphicsItem->zValue();
}

void v_Base::setZValue(qreal z)
{
    proxyGraphicsItem->setZValue(z);
}

qreal v_Base::opacity() const
{
    return proxyGraphicsItem->opacity();
}

void v_Base::setOpacity(qreal opacity)
{
    proxyGraphicsItem->setOpacity(opacity);
}

void v_Base::setRotation(qreal angle)
{
    proxyGraphicsItem->setRotation(angle);
}

qreal v_Base::rotation() const
{
    return proxyGraphicsItem->rotation();
}

void v_Base::setScale(qreal scale)
{
    proxyGraphicsItem->setScale(scale);
}

qreal v_Base::scale() const
{
    return proxyGraphicsItem->scale();
}

bool v_Base::isVisible() const
{
    return proxyGraphicsItem->isVisible();
}

void v_Base::setVisible(bool visible)
{
    if (proxyGraphicsItem->isVisible()!=visible){
        proxyGraphicsItem->setVisible(visible);
        doStateChanged();
    }
}

bool v_Base::isSelected() const
{
    return proxyGraphicsItem->isSelected();
}

void v_Base::setSelected(bool selected)
{
    if (proxyGraphicsItem->isSelected()!=selected){
        proxyGraphicsItem->setSelected(selected);
        doStateChanged();
    }
}

void v_Base::prepareGeometryChange()
{
    proxyGraphicsItem->prepareGeometryChange();
}

void v_Base::update()
{
    proxyGraphicsItem->update();
}


int v_Base::gridDX() const
{
    if (screen()) return screen()->GRIDDX();
    return 10;
}

int v_Base::gridDY() const
{
    if (screen()) return screen()->GRIDDY();
    return 10;
}

QPointF v_Base::grid2point(QPointF gridFM) const
{
    return QPointF(gridFM.x()*gridDX(),gridFM.y()*gridDY());
}

QPointF v_Base::point2grid(QPointF p) const
{
    // точка в координатах сетки
    int xg=qRound(p.x()/gridDX());
    int yg=qRound(p.y()/gridDY());
    return QPointF(xg,yg);
}

QPointF v_Base::point2Parent(QPointF p, v_Base *par)
{
    v_Base *par1=this;
    while (par1!=nullptr){
        p.setX(p.x()+par1->proxyGraphicsItem->pos().x());
        p.setY(p.y()+par1->proxyGraphicsItem->pos().y());
        // а еще и поворот и растяжение..
        if (par==par1) break;
        par1=qobject_cast<v_Base *>(par1->parent());
    }
    return p;
}

QPointF v_Base::parent2point(QPointF p, v_Base *par)
{
    v_Base *par1=this;
    while (par1!=0){
        p.setX(p.x()-par1->proxyGraphicsItem->pos().x());
        p.setY(p.y()-par1->proxyGraphicsItem->pos().y());
        if (par==par1) break;
        // а еще и поворот и растяжение..
        par1=qobject_cast<v_Base *>(par1->parent());
    }
    return p;
}

QRectF v_Base::boundingRect() const
{
    if (FSTATE_MK_FLAG>0){
        return _boundingRect.united(flagRect()).adjusted(-1,-1,1,1);
    }
    return _boundingRect;
}

QPainterPath v_Base::shape() const
{
    if (_shape.isEmpty()) {
        QPainterPath path;
        path.addRect(boundingRect());
        return path;
    }
    return _shape;

}




void v_Base::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    startChekingColorAnimate();
    d_paint(painter, option);
}

void v_Base::advance(int phase)
{
    if (phase)
        if (isAnimatedColorUsed())
            update();
}





void v_Base::drawBackground(QPainter *painter)
{
    if (FSTATE_MK_FLAG!=0)
        drawBackgroundFLAG(painter, FSTATE_MK_FLAG);
}

void v_Base::drawForeground(QPainter *painter)
{
    if (FSTATE_MK_FLAG!=0)
        drawForegroundFLAG(painter, FSTATE_MK_FLAG);
}

QRectF v_Base::flagRect() const
{
    if (FFLAGRECT.isEmpty())
        return _boundingRect.adjusted(-gridDX(),-gridDY(),gridDX(),gridDY());
    return QRectF(grid2point(FFLAGRECT.topLeft()),grid2point(FFLAGRECT.bottomRight()));
}


void v_Base::drawBackgroundFLAG(QPainter *painter, int flag)
{
    if (flag==1){ // МК ок
    }
    if (flag==2){ // МК предотказ
    }
    if (flag==3){ // МК отказ
    }

    if ((flag==102)||(flag==103))
    {
        QColor clr0=getColor(Color_FON);
        clr0.setAlphaF(0);
        QColor clr1;
        if (flag==102) clr1=getColor(Color_flag_op_warning);// ОП предупреждение
        if (flag==103) clr1=getColor(Color_flag_op_critical);// ОП критич предупреждение
        QRectF R=flagRect();

        qreal dd=MColorPalette::instance()->animateK(1000);
        QPointF center=R.center();
        QRadialGradient gradient(center,qMax(R.width()/2,R.height()/2));

//        R.setSize(QSize(R.width()*dd,R.height()*dd));
//        R.moveCenter(center);

        if (dd<0.5){
            gradient.setColorAt(0, clr1);
            gradient.setColorAt(dd*2., clr0);
        } else {
            gradient.setColorAt((dd-0.5)*2., clr0);
            gradient.setColorAt(1, clr1);
        }

        QBrush B=QBrush(gradient);
        painter->setBrush(B);
        painter->setPen(Qt::NoPen);
        //painter->drawEllipse(R);
        painter->drawRect(R);
    }
}

void v_Base::drawForegroundFLAG(QPainter *painter, int flag)
{
    if ((flag==1)||(flag==2)||(flag==3))
    {
        QColor clr;
        if (flag==1) clr=getColor(Color_flag_mk_normal);
        if (flag==2) clr=getColor(Color_flag_mk_predotkaz);
        if (flag==3) clr=getColor(Color_flag_mk_otkaz);
        QRectF R=flagRect();

        painter->setPen(clr);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(R);
    }

    if ((flag==101)||(flag==102)||(flag==103))
    {
        QColor clr;
        if (flag==101) clr=getColor(Color_flag_op_focus); // ОП фокус
        if (flag==102) clr=getColor(Color_flag_op_warning);// ОП предупреждение
        if (flag==103) clr=getColor(Color_flag_op_critical);// ОП критич предупреждение
        QRectF R=flagRect();
        painter->setPen(clr);
        painter->setBrush(Qt::NoBrush);
        QPointF center=R.center();
        //if (flag==101){
            qreal dd=MColorPalette::instance()->animateK(1000);
            R.setSize(QSize(R.width()*dd,R.height()*dd));
            R.moveCenter(center);
       // }
        painter->drawRect(R);
    }

}



QRectF v_Base::groupBoundingRect() const
{
    QRectF R=boundingRect();
    foreach(QObject * b,children()){
        v_Base *child=qobject_cast<v_Base *>(b);
        if (child!=nullptr) {
            QRectF R1=child->boundingRect();
            R1.moveCenter(child->proxyGraphicsItem->pos());
            R=R.united(R1);
        }
    }
    return R;
}

QPainterPath v_Base::groupShape() const
{
    QPainterPath path;
    path.addPath(shape());
    foreach(QObject * b,children()){
        v_Base *child=qobject_cast<v_Base *>(b);
        if (child!=nullptr)  path.addPath(child->shape());
    }
    return path;
}

void v_Base::groupPaint(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    painter->save();
    QMatrix m=painter->matrix();
    painter->setMatrix(proxyGraphicsItem->matrix(), true);
    m=painter->matrix();
    paint(painter,option);

    foreach(QObject * b,children()){
        v_Base *child=qobject_cast<v_Base *>(b);
        if (child!=nullptr)  {
            painter->save();
            {
                QTransform transform=painter->transform();
                transform.translate(child->proxyGraphicsItem->pos().x(),child->proxyGraphicsItem->pos().y());
                painter->setTransform(transform);
            }
            child->groupPaint(painter,option);
            painter->restore();

        }
    }
    painter->restore();
}

bool v_Base::isCanAddObject(QObject *O) const
{
    if (qobject_cast<v_Base*>(O)!=nullptr) return true;
    return false;
}

void v_Base::addObject(QObject *O)
{
    BaseObject::addObject(O);
    v_Base * v=qobject_cast<v_Base*>(O);
    if (v){
        v->setParent(this);
        v->setVisualParent(this);
    }
}

QList<ProxyGraphicsItem *> v_Base::findChildren_ProxyGraphicsItem (ProxyGraphicsItem *pi)
{
    QList<ProxyGraphicsItem *> l;
    if (pi==nullptr) return l;
    l.push_back(pi);
    foreach(QGraphicsItem * gi,pi->childItems()){
        ProxyGraphicsItem *pi1=qgraphicsitem_cast<ProxyGraphicsItem *>(gi);
        if (pi1!=nullptr) {
            QList<ProxyGraphicsItem *> l1=findChildren_ProxyGraphicsItem (pi1);
            l+=l1;
        }
    }
    return l;
}

v_Base *v_Base::visualParent() const
{
    if (proxyGraphicsItem!=nullptr) {
        ProxyGraphicsItem * pgi=qgraphicsitem_cast<ProxyGraphicsItem *>(proxyGraphicsItem->parentItem());
        if (pgi!=nullptr)
            return qobject_cast<v_Base *>(pgi->getvBase());
    }
    return nullptr;
}

void v_Base::setVisualParent(v_Base *v)
{
    if ((v!=nullptr)&&(v->getProxyGraphicsItem()!=nullptr)){
        proxyGraphicsItem->setParentItem(v->getProxyGraphicsItem());
    } else
        proxyGraphicsItem->setParentItem(nullptr);
}


void v_Base::calculateGeometry()
{
}

QMap<int,QPointF> v_Base::getMarkersInfo() const
{
    QMap<int,QPointF> m;
    return m;
}

void v_Base::moveMarker(int markerId, QPointF xyItem)
{
    Q_UNUSED(markerId);Q_UNUSED(xyItem);
}



QPolygonF v_Base::centreLine() const
{
    if (!boundingRect().isEmpty()){
        QPolygonF P;
        P << QPointF(boundingRect().left(),boundingRect().height()/2) << QPointF(boundingRect().right(),boundingRect().height()/2);
        return P;
    }
    return QPolygonF();
}


void v_Base::updateStates()
{
    BaseObject::updateStates();
    t_mk_flag mk_flag;
    ExternFlags::getFlag(FEXTFLAGSRC,0/*src*/,FId,mk_flag);
    setSTATE_MK_FLAG(mk_flag.type);
}


bool v_Base::isSceneEditorMode() const
{
    if (screen()) return screen()->isSceneEditorMode();
    return true;

}


QColor v_Base::getColor(int uid)
{
    if ((!colorAnimate)&&(MColorPalette::instance()->getMColor(uid)->isAnimated())) colorAnimate=true;
    return MColorPalette::instance()->getMColor(uid)->getCOLOR();
}

QColor v_Base::getColor(const QString &idstr)
{
    QColor clr;
    if (!idstr.isEmpty()){
        if (MColorPalette::instance()->existsColor(idstr)){
            MColor *mc=MColorPalette::instance()->getMColor_idstr(idstr);
            if (mc) clr=mc->getCOLOR();
        } else {
            clr.setNamedColor(idstr);
        }
    }
    return clr;
}

void v_Base::startChekingColorAnimate()
{
    colorAnimate=false;
}

bool v_Base::isAnimatedColorUsed()
{
    return colorAnimate;
}

Visual_Screen *v_Base::screen() const
{
    v_Base *vp=visualParent();
    Visual_Screen * vs=qobject_cast<Visual_Screen *>(vp);
    if (vs) return vs;
    if (vp) return vp->screen();
    return nullptr;
}

void v_Base::setScreen(Visual_Screen *p)
{
    // устанавливаем визуал иерархию
    QList<v_Base *> l=findChildren<v_Base *>(QString(),Qt::FindDirectChildrenOnly);
    foreach (v_Base *v,l) {
        v->setVisualParent(this);
        v->setScreen(p);
    }
}




