#ifndef LINESTAB_H
#define LINESTAB_H

#include <QtWidgets>
#include "switcheswidget.h"
#define N_LINES    8


//const int nHVACLines[]={8,5,5,4};


class LinesTab : public QWidget
{
    Q_OBJECT
public:
    explicit LinesTab(QMap<QString,QString> *_commandBuffer,int _devType, int _numbHVAC, QWidget *parent = 0);
    void fillLinesForm( QList<QStringList> rows );
signals:
    void bufferFilled(bool);

public slots:
    void handleLines( QString str );
    void switchNumbersToInitialState();

private:
    /* private functions*/
    void createLinesView();
    void comboBoxFilter();
    void setLinesInitialState();
    void sortItems();
    void createDipSwitches();

    /* fields*/
    QList<QComboBox*> lineCombosList;
    QList<QLabel*> lineLabelList;
    QList<QGroupBox*> lineFrameBoxesList;
    QList<QLabel*> switchesList;
    QMap<QString,QString> *commandBuffer;
    int devType;
    int numbHVAC;
    int nHVACLines;
    bool isDisconnect;
    bool isHandlingLines;
    QLabel* HVACPorts;
    QLabel* numberOfHVACPort;
    QStringList selectedDevices;
    SwitchesWidget* switchWidget;

    /* lines layouts */
    QLayout* layoutMainHVAC;
    QHBoxLayout* layoutMainPage;
};

#endif // LINESTAB_H
