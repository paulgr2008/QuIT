#include "switchlabel.h"
#include <QGraphicsGridLayout>
#include <QGraphicsWidget>

SwitchLabel::SwitchLabel(QGraphicsScene *parent) : QGraphicsScene(parent)
{
    //QGraphicsGridLayout *layout = new QGraphicsGridLayout;
   /* QGraphicsWidget *wForm = new QGraphicsWidget;
    wForm->setLayout(layout);
    this->addItem(wForm);*/
    QColor black(40,57,66);

    QBrush blackBrush(black);
    QPen outlinePen(Qt::black);
    outlinePen.setWidth(0);
    this->setSceneRect(0,0,50,45);
    int w = 45;
    int h = 45;
    int x=0;
    int y =0;

    ellipse = this->addEllipse(x, y, w, h, outlinePen, blackBrush);
    QPen linePen (black);

    linePen.setWidth(ellipse->boundingRect().width()/8.);
    line = this->addLine(x-ellipse->boundingRect().height()/3.,
                                     y+ellipse->boundingRect().width()/2.,
                                     x,y+ellipse->boundingRect().height()/2.,linePen);
    textItem= new QGraphicsSimpleTextItem();
    QFont font=QFont("Tahoma");

    font.setPixelSize(ellipse->boundingRect().width()/1.5);
    textItem->setBrush(QColor(Qt::white));
    textItem->setFont(font);
    textItem->setPos(ellipse->boundingRect().width()/2.-textItem->boundingRect().width()/2.+x-10,y+3);
    textItem->setParentItem(ellipse);
    line->setParentItem(ellipse);
}

void SwitchLabel::setText(QString str)
{
    textItem->setText(str);
}

void SwitchLabel::setColor(QString str)
{
   QColor _color(str) ;
   QBrush brush(_color);
   QPen outlinePen(_color);
   outlinePen.setWidth(0);
   ellipse->setBrush(brush);
   ellipse->setPen(outlinePen);

   QPen linePen (_color);
   linePen.setWidth(ellipse->boundingRect().width()/8.);
   line->setPen(linePen);
}

