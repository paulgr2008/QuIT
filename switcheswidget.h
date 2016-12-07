#ifndef SWITCHESWIDGET_H
#define SWITCHESWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsLayoutItem>

#include <QGraphicsScene>
#include <QGraphicsItem>

namespace Ui {
class SwitchesWidget;
}

class QLabel;

class SwitchesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SwitchesWidget(QWidget *parent = 0);
    void resetSwitches();
    void switchesHandler( int row, int code );
    void switchOn( int row, int column, bool bit );
    void highlightChangedLabel( int row,  bool isSwitchOn );
    void setPrevState(int row, int code){prevState[row]=code;}
    int* getPrevState() {return prevState;}
    void highlightChangedSwitchNumber( int row, int code );

    ~SwitchesWidget();
public slots:
    void switchNumbersToInitialState( );

private:
    Ui::SwitchesWidget *ui;
    QList<QGraphicsView*> gViews;
    int prevState[4];
    void setLabelPalette(QLabel* label,QColor color);

    void createGraphicsElement(QGraphicsView* gView, QString str);
};

#endif // SWITCHESWIDGET_H
