#ifndef SWITCHLABEL_H
#define SWITCHLABEL_H

#include <QObject>
#include <QGraphicsLayoutItem>

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsSimpleTextItem>

class SwitchLabel : public QGraphicsScene
{

public:
    explicit SwitchLabel(QGraphicsScene *parent = 0);

    void setColor(QString str);
    void setText(QString str);


private:

    QGraphicsEllipseItem* ellipse;
    QGraphicsLineItem* line;
    QGraphicsSimpleTextItem* textItem;

};

#endif // SWITCHLABEL_H
