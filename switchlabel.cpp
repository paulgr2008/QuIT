#include "switchlabel.h"
#include <QGraphicsGridLayout>
#include <QGraphicsWidget>

SwitchLabel::SwitchLabel(QGraphicsScene *parent) : QGraphicsScene(parent)
{
    QGraphicsGridLayout *layout = new QGraphicsGridLayout;
    QGraphicsWidget *wForm = new QGraphicsWidget;
    wForm->setLayout(layout);
    this->addItem(wForm);
    QColor black(40,57,66);

    QBrush blackBrush(black);
    //QBrush blueBrush(Qt::blue);
    QPen outlinePen(Qt::black);
    outlinePen.setWidth(0);

    // addEllipse(x,y,w,h,pen,brush)
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
    textItem= new QGraphicsTextItem();
    QFont font=QFont("Arial", ellipse->boundingRect().width()/2.5, QFont::Bold);
    textItem->setDefaultTextColor(Qt::white);
    textItem->setFont(font);

    textItem->setPos(ellipse->boundingRect().width()/2.-textItem->boundingRect().width()/2.+x-10,
                       ellipse->boundingRect().height()/2.-textItem->boundingRect().height()/2.+y);
    textItem->setParentItem(ellipse);
    line->setParentItem(ellipse);
}

void SwitchLabel::setText(QString str)
{
    textItem->setHtml(str);
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

