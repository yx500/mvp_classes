#include "v_picture.h"

#include "qpfunction.h"


#include "mvp_objectfactory.h"
REGISTERELEMENT(v_Picture,"Картинка","VISUAL Простые элементы")



v_Picture::v_Picture(v_Base *parent):
    v_Base(parent)
{
    M[0]=QPointF(0,0);
    FSIZE=QSizeF(1,1);
    FFRAME=true;
    levelOfDetail=1;
}

v_Picture::~v_Picture()
{
}

void v_Picture::setFN(QString p)
{
    if (FFN!=p){
        FFN=p;
        // относительно чего?
        // в ресурсы нах! или абсолютный путь!
//        image.load(FFN);
//        svgRenderer.load(FFN);
//        if (svgRenderer.isValid()){
//            image=QImage(100,100,QImage::Format_ARGB32);
//            image.fill(Qt::gray);
//        }
        doPropertyChanged();
    }
}

void v_Picture::setSVG_XML(QString p)
{
    if (FSVG_XML!=p){
        FSVG_XML=p;
        svgRenderer.load(FSVG_XML.toUtf8());
    }
}

void v_Picture::d_paint(QPainter *painter, const QStyleOptionGraphicsItem * option)
{
    //svgRenderer.render(painter,pictureRect);
    if ((option) &&(option->levelOfDetailFromTransform(painter->worldTransform())!=levelOfDetail)){
        levelOfDetail=option->levelOfDetailFromTransform(painter->worldTransform());
        mappedR=painter->worldTransform().mapRect(pictureRect);
        calculateGeometry();
    }
    painter->setBrush(Qt::NoBrush);
    painter->drawImage(pictureRect,image);

    if (FFRAME){
        painter->setPen(QPen(QBrush(Qt::black),0,Qt::SolidLine));
        painter->drawRect(pictureRect);

    }
}




void v_Picture::calculateGeometry()
{
    pictureRect=QRectF(0,0,FSIZE.width()*gridDX(),FSIZE.height()*gridDY());
    if (svgRenderer.isValid()){
        if (levelOfDetail!=1){
            image=QImage(mappedR.size().width(),mappedR.size().height(),QImage::Format_ARGB32);
        } else {
            image=QImage(pictureRect.size().width(),pictureRect.size().height(),QImage::Format_ARGB32);
        }

        QPainter painter(&image);
        image.fill(QColor(255,255,255,0));
        svgRenderer.render(&painter);//,pictureRect);
    }
    pictureRect.moveCenter(M[0]);
    {
        _boundingRect=pictureRect;
        _boundingRect.adjust(-1,-1,+1,+1);
    }

}


