#include "switcheswidget.h"
#include "ui_switcheswidget.h"
#include "switchlabel.h"
#include "bitset"
#include <QGraphicsGridLayout>
#include <QGraphicsWidget>
#include <QDebug>
#include "globalvars.h"

SwitchesWidget::SwitchesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SwitchesWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QStringList list ;
    int i =0;
    QString str;
    list<<"P"<<"Q"<<"R"<<"S";


    gViews = this->findChildren<QGraphicsView*>();
        foreach(QGraphicsView* view, gViews)
        {
            if(i==list.size())
                break;

            str = list.at(i++);
            createGraphicsElement(view, str);
        }


}

void SwitchesWidget::createGraphicsElement(QGraphicsView* gView, QString str)
{
        SwitchLabel* scene = new SwitchLabel;
        scene->setText(str);
        gView->setScene(scene);
}

/*
 * Clearing dip switches view
*/
void SwitchesWidget::resetSwitches()
{
    QRegExp ex( "^sw_*" );
    QList<QLabel*> switchesList = this->findChildren<QLabel*>( ex );
    foreach( QLabel* label,switchesList )
        label->setPixmap( QPixmap(":/images/switch_off_light.png") );
    foreach( QGraphicsView* view,gViews )
    {
        SwitchLabel* scena = static_cast<SwitchLabel*>(view->scene());
        scena->setColor(C_DISABLED);
    }
}

void SwitchesWidget::highlightChangedLabel(int row, bool isSwitchedOn )
{
    SwitchLabel* scena = static_cast<SwitchLabel*>(gViews.at(row)->scene());

    if( isSwitchedOn )
        scena->setColor(C_ERROR);
    else
        scena->setColor(C_TEXT);
}

/*
 * Handling with dip switches line
*/
void SwitchesWidget::switchesHandler( int row, int code )
{
    /*
    if ( code==0 )
    {
       SwitchLabel* scena = static_cast<SwitchLabel*>(gViews.at(row)->scene());
       scena->setColor("#D1D6D9");
       QGroupBox* group = this->findChild<QGroupBox*>(QString("switchBox_%1").arg(row));
       group->setStyleSheet("QGroupBox {border: 1px solid #D1D6D9;"
                                 "background: white;}");
       QRegExp ex( "^sw_*" );
       QList<QLabel*> switchesList = group->findChildren<QLabel*>( ex );
       foreach( QLabel* label,switchesList )
           label->setPixmap( QPixmap(":/images/switch_off_light.png") );
       ex.setPattern("^label*");
       QList<QLabel*> switchesLabelList = group->findChildren<QLabel*>(ex);
       foreach (QLabel* label,switchesLabelList )
           label->setStyleSheet("color:#D1D6D9");
       group->findChild<QLabel*>(QString("arr_on_%1").arg(row))->setPixmap(QPixmap(":/images/on_arrow_light.png"));
       return;
    }*/
    std::bitset <4> bits ( code );
    int j=4;
    for( int i=0; i<4; i++ )
    {
        switchOn( row,--j,bits[i] );
    }
}

/*
 * Handling with the particular dip switch
*/
void SwitchesWidget::switchOn( int row, int column, bool bit )
{
    QLabel* label;
    QString str = QString( "sw_%1_%2" ).arg( row ).arg( column );
    label = this->findChild<QLabel*>( str );

    if ( bit )
        label->setPixmap(QPixmap(":/images/switch_on.png") );/* insert image ON*/
    else
        label->setPixmap( QPixmap(":/images/switch_off.png") );/* insert image OFF*/
}

void SwitchesWidget::highlightChangedSwitchNumber( int row, int code )
{
    QLabel* label;

    std::bitset <4> bits ( code );
    std::bitset<4> prevBitMap (prevState[row]);
    int j=4;
    for( int i=0; i<4; i++ )
    {
        QString str = QString( "label_%1_%2" ).arg( row ).arg( --j );
        label = this->findChild<QLabel*>( str );
    if ( bits[i] != prevBitMap[i] )
        setLabelPalette( label,QColor("#9a27f3") );/* insert image ON*/
    else
        setLabelPalette( label,QColor("#283942"));
    }
}

void SwitchesWidget::switchNumbersToInitialState( )
{
    QLabel* label;

    for (int i=0; i<4; i++)
    {
        SwitchLabel* scena = static_cast<SwitchLabel*>(gViews.at(i)->scene());
        scena->setColor(C_TEXT);
        for (int j=0; j<4; j++)
        {
            QString str = QString( "label_%1_%2" ).arg( i ).arg( j );
            label = this->findChild<QLabel*>( str );
            setLabelPalette(label,QColor(C_TEXT));
        }
    }
}

void SwitchesWidget::setLabelPalette(QLabel* label,QColor color)
{
    QPalette palette = label->palette();
    palette.setColor(label->foregroundRole(),color );
    label->setPalette(palette);
}

SwitchesWidget::~SwitchesWidget()
{
    delete ui;
}
