#include "mainwindow.h"
#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <qDebug>
#include <Dbt.h>
#include <initguid.h>
#include "ui_mainwindow.h"
#include "bitset"
#include "iostream"
#include "consoleTab.h"
#include "consolecontroller.h"
#define CA_TOOLS
 #include "line.h"

#define NAMEAPP "Quick Installation Tool"
#define VERSION "v0.0.1b"

DEFINE_GUID( GUID_DEVINTERFACE_USBDEVICE,0x4D36E978L, 0xE325, 0x11CE,0xBF,0xC1,0x08,0x00,0x2B,0xE1,0x03,0x18 );
#define VENDOR_ID 10668


bool comparator( const QSerialPortInfo &info1, const QSerialPortInfo &info2 );
Q_DECLARE_METATYPE(line_type_descr_t)


//typedef line_type_descr_t *line_type_descr_ptr;
//Q_DECLARE_METATYPE(line_type_descr_ptr)


MainWindow::MainWindow( QWidget *parent ):QMainWindow( parent ),
            ui( new Ui::MainWindow )
{
    this->setWindowFlags( Qt::Dialog | Qt::WindowSystemMenuHint |
                          Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint |
                          Qt::MSWindowsFixedSizeDialogHint |
                          Qt::MaximizeUsingFullscreenGeometryHint );
    ui->setupUi(this);

    /* List of device names in used */
    devTypeStrings<<"CoolMasterNet"<<"CoolLinkNet"<<"CoolLinkHub"<<"CoolPlug";



    initializeDeviceNotification();

    settings = new QSettings( "settings.ini",QSettings::IniFormat );
    portName = ( settings->value("settings/port") ).toString();
    baudRate = ( settings->value("settings/baudrate") ).toInt();
    serial = new QSerialPort(this);

    defineStyles();
    setupModel();
    setupConnectMenu();
    setupLinesView();
    //setTitle();
    /* GUI on mainwindow form */
    size = new QSize( 403,309 );/* Size of window - the first appearence */

    setFixedSize(*size);
    consoleTab = new ConsoleTab( serial,echo );
    ui->tabWidget->addTab( consoleTab,tr( "Console" ) );

    ui->tabWidget->hide();
    ui->deviceName->hide();
    ui->deviceVersion->hide();
    ui->labelConnected->hide();
    ui->labelStatus->hide();
    ui->dipswitch->hide();
    ui->numberOfLines->hide();
    ui->applyButton->hide();
    ui->rebootBtn->hide();
    ui->rebootBtn->setEnabled(false);
    ui->autocompleter->hide();
    isSessionBegin = false;
    sysMsgTimeGate = false;

    initGUIConnections();
}


void MainWindow::setTitle()
{
    QCoreApplication::setApplicationName(NAMEAPP);
    QCoreApplication::setApplicationVersion(VERSION);
    QString str = QCoreApplication::applicationName()+QCoreApplication::applicationVersion();
    setWindowTitle(str);
}

/*
 * Styles list of GUI elements
*/
void MainWindow::defineStyles()
{
    styleLineFrameUsed    = tr( "#group .QGroupBox {background: #256F5B;"
                                "padding-top: 2px;"
                                "padding-bottom: 2px;"
                                "border-radius: 8px;"
                                "padding-right:4px;}" );

    styleLineFrameDisabled = tr( "#group .QGroupBox {background:#f0f0f0;"
                                "border-top: 2px solid #f0f0f0;"
                                "padding-top: 2px;"
                                "padding-bottom: 2px;"
                                "padding-right:4px;}" );
/*
    switchOnStyle          = tr( "border: 0px solid white;"
                                "padding: 0px;"
                                "background: rgb( 89, 157, 141 )" );
*/switchOnStyle          = tr( "border: 0px solid white;"
                               "padding: 0px;"
                               "background: red" );

    switchOffStyle          = tr( "border: 0px solid white;"
                                   "padding: 0px;"
                                   "background: rgb( 89, 157, 141 )" );


    switchBeginStyle         = tr( "border: 0px solid white;"
                                "padding: 0px;"
                                "background: white" );

    dotLabelStySheet = tr( "border-top: 1px solid #999999;"
                              "border-bottom:1px solid #999999;"
                              "border-right:0px solid #ffffff;"
                              "border-left:0px solid #ffffff;"
                              "padding: 0px;"
                              "background: #ffffff" );

    /* set dot labels style of ip elements*/
    QRegExp rx( "*point*" );
    rx.setPatternSyntax( QRegExp::Wildcard );
    QList<QLabel*> dotLabelList = ui->netSet->findChildren<QLabel*>( rx );
    foreach( QLabel* label, dotLabelList )
        label->setStyleSheet( dotLabelStySheet );

    rx.setPatternSyntax( QRegExp::RegExp );
    rx.setPattern("^.*_0$");
    QList<QLineEdit*> ipList = ui->netSet->findChildren<QLineEdit*>( rx );
    foreach(QLineEdit* le, ipList)
        le->setStyleSheet( tr( "border-top: 1px solid #999999;"
                                  "border-bottom: 1px solid #999999;"
                                  "border-left: 1px solid #999999;"
                                  "border-right: 0px solid #ffffff;"
                                  "padding: 0px;"
                                  "background: #ffffff" ) );
    ipList.clear();
    rx.setPattern("^.*(_1|_2)$");
    ipList = ui->netSet->findChildren<QLineEdit*>( rx );
    foreach( QLineEdit* le, ipList )
        le->setStyleSheet( tr( "border-top: 1px solid #999999;"
                               "border-bottom: 1px solid #999999;"
                               "border-left: 0px solid #ffffff;"
                               "border-right: 0px solid #ffffff;"
                               "padding: 0px;"
                               "background: #ffffff" ) );
    ipList.clear();
    rx.setPattern("^.*_3$");
    ipList = ui->netSet->findChildren<QLineEdit*>( rx );
    foreach( QLineEdit* le, ipList )
        le->setStyleSheet( tr( "border-top: 1px solid #999999;"
                               "border-bottom: 1px solid #999999;"
                               "border-left: 0px solid #ffffff;"
                               "border-right: 1px solid #999999;"
                               "padding: 0px;"
                               "background: #ffffff" ) );
}

/*
* Setup GUI lines presentation
*/
void MainWindow::setupLinesView()
{
     /* Set line labels */
    QRegExp rx( "label*" );
    rx.setPatternSyntax( QRegExp::Wildcard );
    lineLabelList = ui->hvacLine->findChildren<QLabel*>( rx );
    foreach( QLabel* label, lineLabelList )
    {
        QFont font ( "MS Shell Dlg2",9 );
        font.setBold(true);
        label->setFont( font );
        label->setStyleSheet( "#group .QLabel {color: #ffffff; padding-left:10px}" );
    }


    /* Set line frames */
    rx.setPattern( "groupBox*" );
    lineFrameBoxesList = ui->group->findChildren<QGroupBox*>( rx );

    foreach( QGroupBox* frame, lineFrameBoxesList )
    {
        frame->setStyleSheet( styleLineFrameDisabled );
        frame->setAutoFillBackground(true);
    }

    /* Set comboBoxes */
    rx.setPattern( "comboBox*" );
    lineCombosList =  ui->hvacLine->findChildren<QComboBox*>( rx );

    /* Set comboboxes connects */

    for ( int i=0; i<lineCombosList.size();i++ )
    {
        connect( lineCombosList[i],SIGNAL( activated( QString ) ),this, SLOT( handleLines( QString ) ) );
        QFont font ( "MS Shell Dlg2",9 );
        lineCombosList[i]->setFont( font );
        lineCombosList[i]->setStyleSheet( " color: #000000" );//background: white;
        lineCombosList[i]->setEnabled(false);
        lineCombosList[i]->hide();
        lineCombosList[i]->setAccessibleName( QString( "L%1" ).arg( i+1 ) );
        lineCombosList[i]->installEventFilter(this);
    }
}

/*
 * Handling with lines presentation
*/
void MainWindow::handleLines( QString str )
{
    QComboBox* senderBox = dynamic_cast<QComboBox*>( sender() );
    QString strLine = senderBox->accessibleName();

    if ( numbHVAC>1 )
        comboBoxFilter();
    else
        foreach ( QComboBox* box, lineCombosList )
            if( senderBox!=box&&senderBox->isEnabled() )
            {
                box->setCurrentIndex( box->findData("Unused",Qt::DisplayRole) );
                sortItems();
            }

    createDipSwitches();

    /* send command */
    commandBuffer.insert( "line type "+strLine+" ", str );
    ui->applyButton->setEnabled(true);
}

/*
 * Devices list filtering
*/
void MainWindow::comboBoxFilter()
{
    int count            = 0;
    int countGRType      = 0;
    int countLineType    = 0;
    int hiddenItemsCount = 0;
    QVariant data;
    QVariant firstData;
    uint8_t dev;
    uint8_t first;
    line_type_descr_t firstDescr;
    line_type_descr_t descr;

    for( int i=0; i<lineCombosList.size(); i++ )
    {
        QComboBox* b = lineCombosList.at(i);
        firstData = b->currentData( Qt::UserRole );
        firstDescr = firstData.value<line_type_descr_t>();
        first = firstDescr.ac_type;

        if ( first == LINE_AC_TYPE_UNUSED
           ||first == LINE_AC_TYPE_PBM )
            continue;

        if( first==LINE_AC_TYPE_GR )
            {
                 countGRType++;
            }
            else countLineType++;
    }

    if( countGRType )
        count = countLineType + 1;
    else count = countLineType;

    if( count >= numbHVAC )
    {

        foreach( QComboBox* box, lineCombosList )
        {
            QListView* listVw = qobject_cast<QListView*>( box->view() );

            firstData = box->currentData();
            firstDescr = firstData.value<line_type_descr_t>();
            first = firstDescr.ac_type;

            //qDebug()<<box->currentText();
            //qDebug()<<box->accessibleName()<<" "<<"items: "<<box->count();
            for( int i=1; i<box->count(); i++ )
            {
                data = box->itemData( i,Qt::UserRole );
                descr = data.value<line_type_descr_t>();
                dev = descr.ac_type;

            if(  first == LINE_AC_TYPE_UNUSED
               ||first == LINE_AC_TYPE_PBM )
            {
                    if(    dev != LINE_AC_TYPE_UNUSED
                        && dev != LINE_AC_TYPE_PBM
                        && dev != LINE_AC_TYPE_GR   )
                    {
                        listVw->setRowHidden( i,true );
                        hiddenItemsCount++;
                    }

                    if ( dev == LINE_AC_TYPE_GR )
                    {
                        if( countGRType == 4 || countLineType>=numbHVAC )
                        {
                            listVw->setRowHidden( i,true );
                            hiddenItemsCount++;
                        }
                    }
            }
            else if ( first == LINE_AC_TYPE_GR )
            {
                    if ( dev != LINE_AC_TYPE_UNUSED
                         &&dev != LINE_AC_TYPE_PBM )
                    {
                        listVw->setRowHidden( i,true );
                        hiddenItemsCount++;
                    }
            }
            else
            {
                    if ( dev == LINE_AC_TYPE_GR )
                    {
                        if ( countGRType == 4 )
                        {
                            listVw->setRowHidden( i,true );
                            hiddenItemsCount++;
                        }
                    }
            }
        }
            if ( hiddenItemsCount==box->count()-1 )
                     box->setEnabled(false);
                    //qDebug()<<"hidden items: "<<hiddenItemsCount;
                hiddenItemsCount=0;
            }
    }
    else
    {
        foreach( QComboBox* box, lineCombosList )
        {
            if( !box->isEnabled() )
                    box->setEnabled(true);
            for( int i=0; i<box->count(); i++ )
            {
                 QListView* listView = qobject_cast<QListView*>( box->view() );
                 listView->setRowHidden( i,false );
            }
        }
    }

    if ( lineCombosList.at(1)->currentText().compare( "unused",Qt::CaseInsensitive ) )
        lineCombosList.at(5)->setEnabled(false);
    else if ( lineCombosList.at(5)->currentText().compare( "unused",Qt::CaseInsensitive ) )
        lineCombosList.at(1)->setEnabled(false);

    sortItems();
}

/*
 * Set lines combobox items
*/
void MainWindow::setLinesInitialState()
{
    int offset = 0;
    uint32_t lineType ;
    foreach( QComboBox* box, lineCombosList )
        box->clear();

    for( int i=0; i<static_cast<int>( sizeof( line_types )/sizeof( line_type_descr_t ) );i++ )
    {
        if ( devType == COOLINK )
            offset = 8;
        else if ( devType== COOLPLUG )
            offset = 16;
        lineType = line_types[i].lines>>offset;

        for( int line = 0; line<nHVACLines[devType]; line++ )
        {
            if ( ( lineType&( 1<<line ) )!=0 )
            {
                QVariant var = QVariant::fromValue( line_types[i] );

                if( QString(line_types[i].func_str).compare("BIST") )
                    lineCombosList.at( line )->addItem( line_types[i].ac_type_str,var );

            }
        }
    }
   // sortItems();
    setLinesViewStyle();
}

/*
 * sort combobox list by device name
*/
void MainWindow::sortItems()
{
    foreach( QComboBox* box, lineCombosList )
    {
            if(box->count() < 2)
                continue;

            QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(box->model());
            model->sort(0);
    }
}

/*
 * Set view style of lines
*/
void MainWindow::setLinesViewStyle()
{
    for ( int i =0 ; i<nHVACLines[devType]; i++ )
    {
        lineLabelList.at(i)->setText( QString( "Line %1" ).arg(i) );
        lineCombosList.at(i)->setEnabled(true);
        lineCombosList.at(i)->show();
        lineFrameBoxesList.at(i)->setStyleSheet( styleLineFrameUsed );
    }
}

/*
 * Operations with dip switches images
*/
void MainWindow::createDipSwitches()
{
    line_type_descr_t descr;
    QVariant var;
    QComboBox* box;
    int code;

    resetSwitches();

    /*P switches*/
    if ( !lineCombosList.at(1)->currentText()
            .contains( "unused",Qt::CaseInsensitive ) )
           switchOn( "p",2,0 );
    else if ( !lineCombosList.at(5)->currentText()
             .contains( "unused",Qt::CaseInsensitive ) )
           switchOn( "p",2,1 );
    else if ( lineCombosList.at(1)->currentText()
             .contains( "unused",Qt::CaseInsensitive )
             && lineCombosList.at(5)->currentText()
             .contains( "unused",Qt::CaseInsensitive ) )
           switchOn( "p",2,0 );
    switchOn( "p",0,0 );
    switchOn( "p",1,0 );
    switchOn( "p",3,0 );

    /* Q switches*/
    box = lineCombosList.at(0);
    var = box->currentData();
    descr = var.value<line_type_descr_t>();
    switch( descr.ac_type )
    {
    case LINE_AC_TYPE_DK:
        code = SW_DK;
        break;
    case LINE_AC_TYPE_ME:
        code = SW_ME;
        break;
    case LINE_AC_TYPE_TO:
        code = SW_TO;
        break;
    case LINE_AC_TYPE_SA:
        code = SW_SA;
        break;
    default:
        code = SW_UNUSED;
        break;
    }
    switchesHandler( "q",code );

    /* R switches*/
    box = lineCombosList.at(1);
    var = box->currentData();
    descr = var.value<line_type_descr_t>();
    switch( descr.ac_type )
    {
        case LINE_AC_TYPE_DK:
            code = SW_DK;
            break;
        case LINE_AC_TYPE_ME:
            code = SW_ME;
            break;
        case LINE_AC_TYPE_TO:
            code = SW_TO;
            break;
        case LINE_AC_TYPE_SA:
            code = SW_SA;
            break;
        default:
            code = SW_UNUSED;
            break;
    }
    switchesHandler( "r",code );

    /*S switches*/
    switchesHandler( "s",0 );
}

/*
 * Clearing dip switches view
*/
void MainWindow::resetSwitches()
{
    QRegExp ex( "^sw_*" );
    QList<QLabel*> switchesList = ui->tabWidget->findChildren<QLabel*>( ex );
    foreach( QLabel* label,switchesList )
        label->setStyleSheet( switchBeginStyle );
}


/*
 * Handling with dip switches line
*/
void MainWindow::switchesHandler( const QString row, int code )
{
    std::bitset <4> bits ( code );

    for( int i=0; i<4; i++ )
        switchOn( row,i,bits[i] );
}

/*
 * Handling with the particular dip switch
*/
void MainWindow::switchOn( const QString row, int column, bool bit )
{
    QLabel* label;
    QString str = QString( "sw_%1%2_%3" ).arg( row ).arg( column+1 ).arg( bit );
    label = ui->tabWidget->findChild<QLabel*>( str );

    if ( bit )
        label->setStyleSheet( switchOnStyle );
    else
        label->setStyleSheet( switchOffStyle );
}

/*
 * Set "Connection" menu
*/
void MainWindow::setupConnectMenu()
{
    portsMenu = ui->menuConnect->addMenu( tr( "&Serial Port" ) );
    ratesMenu = ui->menuConnect->addMenu( tr( "Baud &Rate" ) );
    setPortsMenu();
    setRatesMenu();
    connectAction = new QAction( tr( "&Connect" ), this );
    ui->menuConnect->addAction( connectAction );
    connectAction->setEnabled(false);
    resetConfiguration = new QAction( tr( "&Refresh" ),this );
    resetConfiguration->setEnabled(false);
    ui->menuConnect->addAction( resetConfiguration );
}

/*
 * Set "Baud Rate" menu in the "Connection" menu
*/
void MainWindow::setRatesMenu()
{
    QAction* action;
    action = new QAction( "9600",this );
    action->setCheckable(true);
    action->setChecked(true);
    action->setData( QSerialPort::Baud9600 );
    ratesMenu -> addAction( action );

    action = new QAction( "38400",this );
    action->setCheckable(true);
    action->setChecked(false);
    action->setData( QSerialPort::Baud38400 );
    ratesMenu -> addAction( action );

    action = new QAction( "115200",this );
    action->setCheckable(true);
    action->setChecked(false);
    action->setData( QSerialPort::Baud115200 );
    ratesMenu -> addAction( action );

    baudRate = QSerialPort::Baud9600;
}



/*
 * Set port baud rate for current connection
*/
void MainWindow::setCustomBaudRate( QAction* currentAction )
{
    currentAction->setChecked(true);

    QList<QAction*> actionsList = ratesMenu->actions();

    foreach (QAction* action, actionsList  )
    {
        if (action==currentAction)
            continue;
        action->setChecked(false);
    }

    baudRate = static_cast<QSerialPort::BaudRate>
            ( ( currentAction->data() ).toInt() );
    serial->setBaudRate(baudRate);
    settings->setValue( "settings/baudrate",baudRate ) ;
    settings->sync();
}

/*
 * Set "Serial Port" menu in the "Connection" menu
*/
void MainWindow::setPortsMenu()
{
    QAction* action;
    QString port;
    portsList = QSerialPortInfo::availablePorts();
    bool isChecked = false;
    bool isPresented = false;

    if( !portsList.count() )
    {
        portName.clear();
        ui->labelConnectedPort->setText( "no any serial ports" );
        return;
    }

    qSort( portsList.begin(),portsList.end(),comparator );/* Sort serial ports by name */
    portsMenu->clear();

    /* enum serial port actions in "Serial Port" menu */
    for ( int i=0; i<portsList.count();i++ )
    {
       QString descr = portsList[i].description();

       if ( descr.contains( "Bluetooth",Qt::CaseInsensitive ) )
           continue;

       port = portsList[i].portName();
       action = new QAction( port,this );
       action->setCheckable(true);
       portsMenu->addAction( action );

       if( port==portName )
       {
           isChecked = true;
           action->setChecked(true);
       }
       else action->setChecked(false);

       isPresented = true;
    }

    /* check if any serial port is available */
    if ( !isPresented )
    {
        ui->labelConnectedPort->setText( "no any available ports" );
        portName.clear();
        return;
    }

    /* check if serial port is checked */
    if ( !isChecked )
    {
        QList<QAction*> actionsList = portsMenu->actions();
        portName = actionsList.first()->text() ;
        actionsList.first()->setChecked(true);
    }
    ui->labelConnectedPort->setText( "via "+portName );/* information text on round button */
}

/*
 * SerialPortInfo objects comparator for qSort
*/
bool comparator( const QSerialPortInfo &info1, const QSerialPortInfo &info2 )
{
    return info1.portName()<info2.portName();
}

/*
 * Set name for current connection
*/
void MainWindow::checkCustomPortName( QAction* action )
{
    action->setChecked(true);
    QList<QAction*> actionsList = portsMenu->actions();

    foreach( QAction* act, actionsList )
    {
        if( act == action )
            continue;
        if( act->isChecked() )
            act->setChecked(false);
    }
    portName = action->text();
    ui->labelConnectedPort->setText( "via "+portName );
    settings->setValue( "settings/port",portName ) ;
    settings->sync();
}



/*
 * Setup model of network config elements
*/
void MainWindow::setupModel()
{
    IPWidget* widget;
    widget = new IPWidget(ui->leDNS1_0,ui->leDNS1_1,ui->leDNS1_2,ui->leDNS1_3);
    widget->setAccessibleName("DNS1");
    ipWidgetMap.insert("DNS1",widget);
    widget = new IPWidget(ui->leDNS2_0,ui->leDNS2_1,ui->leDNS2_2,ui->leDNS2_3);
    widget->setAccessibleName("DNS2");
    ipWidgetMap.insert("DNS2",widget);
    widget = new IPWidget(ui->IP_0,ui->IP_1,ui->IP_2,ui->IP_3);
    widget ->setAccessibleName("IP");
    ipWidgetMap.insert("IP",widget);
    widget = new IPWidget(ui->netmask_0,ui->netmask_1,ui->netmask_2,ui->netmask_3);
    widget ->setAccessibleName("Netmask");
    ipWidgetMap.insert("Netmask",widget);
    widget = new IPWidget(ui->gateway_0,ui->gateway_1,ui->gateway_2,ui->gateway_3);
    widget->setAccessibleName("Gateway");
    ipWidgetMap.insert("Gateway",widget);

    QMapIterator<QString, IPWidget*> it(ipWidgetMap);
    while (it.hasNext())
    {
        it.next();
        connect(it.value(),SIGNAL(sendIP(QString)),this,SLOT(handleNetconfigPage(QString))) ;
    }
}

/*
 * Fill commands buffer with changed network config elements
*/
void MainWindow::handleNetconfigPage( QString str )
{
    ui->applyButton->setEnabled(true);
    IPWidget* widget = dynamic_cast<IPWidget*> ( sender() );
    commandBuffer.insert( widget->accessibleName(),str );

}

/*
* Set connections between GUI elements and logical elements
*/
void MainWindow::initGUIConnections()
{
    connect( resetConfiguration, SIGNAL( triggered() ), this, SLOT( getInitInformation() ) );
    //connect( model,SIGNAL( itemChanged( QStandardItem* ) ),this,SLOT( handleNetconfigPage( QStandardItem* ) ) );
    connect( ui->autoipCheckBox,SIGNAL( stateChanged( int ) ),this, SLOT( autoipCheckBoxState( int ) ) );
    connect( serial, SIGNAL( error( QSerialPort::SerialPortError ) ), this, SLOT( handleError( QSerialPort::SerialPortError ) ) );
    connect( portsMenu, SIGNAL( triggered( QAction* ) ),this, SLOT( checkCustomPortName( QAction* ) ) );
    connect( ratesMenu, SIGNAL( triggered( QAction* ) ),this,SLOT( setCustomBaudRate( QAction* ) ) );
    connect( ui->connectBtn, SIGNAL( clicked() ), this, SLOT( handleSerialPort() ) );

    connect( connectAction,SIGNAL( triggered() ),this, SLOT( handleSerialPort() ) );
    connect( ui->applyButton, SIGNAL( clicked() ), this,SLOT( createCommandLine() ) );
    connect( ui->radioButtonSetIPManually, SIGNAL( toggled( bool ) ),this, SLOT( IPSettingsHandler( bool ) ) );
    connect( this, SIGNAL( releaseLoop() ),&loop,SLOT( quit() ) );
    connect( ui->tabWidget,SIGNAL( tabBarClicked( int ) ),this, SLOT( runConsole( int ) ) );

    /* Connection with plugged/unplugged devices */
    connect( this, &MainWindow::signal_DeviceConnected, this, &MainWindow::deviceConnected );
    connect( this, &MainWindow::signal_DeviceDisconnected,this, &MainWindow::deviceDisconnected );
}

/*
* Run console view
*/
void MainWindow::runConsole( int index )
{

    switch(index)
    {
        case 0:
            switchFromConsole();
            dataHandler("line");
            break;
        case 1:
            switchFromConsole();
            dataHandler("ifconfig");
            break;
        case 2:
            consoleTab->runConsole();
            connect( ui->autocompleter,SIGNAL( stateChanged( int ) ),
                     consoleTab->getConsoleController(), SLOT( setAutocomplete( int )) );
            ui->autocompleter->show();
            ui->applyButton->hide();
            ui->rebootBtn->hide();
            break;
        default:
            break;
    }
}

void MainWindow::switchFromConsole()
{
    ui->autocompleter->hide();
    ui->applyButton->show();
    ui->rebootBtn->show();
}

/*
*  slot for plugged device
*/
void MainWindow::deviceConnected()
{
    setPortsMenu();
}

/*
*  slot for unplugged device
*/
void MainWindow::deviceDisconnected()
{
    QTime timer;

    deviceConnected();

    if( !isSessionBegin )
        return;

    if( serial->isOpen() )
        serial->close();

    ui->labelConnected->setText( "Disconnected" );
    ui->labelConnected->setStyleSheet( "color: red" );

    switch ( execWarningWindow( "Connection lost!\n Check cable." ) )
    {
    case QMessageBox::Ok:

        timer.start() ;
        while( timer.elapsed() < 1000 )
                 qApp->processEvents(0);

        openSerialPort();
        break;

    case QMessageBox::Cancel:
        closeSerialPort();
        break;
    }
}

/*
*  Warning messages appering
*/
int MainWindow::execWarningWindow( QString warning )
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Quick Installation Tool");
    msgBox.setText( "Warning..." );
    msgBox.setIcon( QMessageBox::NoIcon );
    msgBox.setInformativeText( warning );

    QPushButton* reconnect = msgBox.addButton( "Reconnect", QMessageBox::AcceptRole );
    msgBox.addButton( "Close Session", QMessageBox::DestructiveRole );

    msgBox.setDefaultButton( reconnect );
    msgBox.exec();
    if ( msgBox.clickedButton() == reconnect )
        return QMessageBox::Ok;
    return QMessageBox::Cancel;
}

/*
 *  Register the application for the device notification in Windows
*/
bool MainWindow::initializeDeviceNotification( void )
{
    DEV_BROADCAST_DEVICEINTERFACE devInt;
    ZeroMemory( &devInt, sizeof( devInt ) );
    devInt.dbcc_size        = sizeof( DEV_BROADCAST_DEVICEINTERFACE );
    devInt.dbcc_devicetype  = DBT_DEVTYP_DEVICEINTERFACE;
    devInt.dbcc_classguid   = GUID_DEVINTERFACE_USBDEVICE;
    m_hDeviceNotify =
            RegisterDeviceNotification( (HANDLE)winId(),&devInt, DEVICE_NOTIFY_WINDOW_HANDLE );
    if( m_hDeviceNotify == 0 )
    {
        qDebug() << "Error: Failed to register device notification!";
        qApp->quit();
    }
    return true;
}

/*
 * Standard redefined method for system event handling (plug/unplug)
 */
bool MainWindow::nativeEvent( const QByteArray& eventType, void* message,
                             long* result )
{
    Q_UNUSED( result );
    Q_UNUSED( eventType );

    MSG* msg = reinterpret_cast<MSG*>( message );

    if( msg->message == WM_DEVICECHANGE )
    {
        switch( msg->wParam )
        {
        case DBT_DEVICEARRIVAL:

            if( sysMsgTimeGate )
                return false;
            timeGateHandler();

            emit signal_DeviceConnected();
            qDebug()<<"device connected";
            break;

        case DBT_DEVICEREMOVECOMPLETE:

            if( sysMsgTimeGate )
                return false;
            timeGateHandler();

            emit signal_DeviceDisconnected();
            qDebug()<<"device disconnected";
            break;
        }
    }
    return false;
}

/*
 *  Set time interval between system events.
 *  if this time interval too short, the next system event is rejected
 */
void MainWindow::timeGateHandler()
{
    sysMsgTimeGate = true;
    QTimer::singleShot( 1000, this, SLOT(openSysMsgTimeGate()) );
}

void MainWindow::openSysMsgTimeGate()
{
    sysMsgTimeGate = false;
}

/*
 * Operate with IP manually setting
 */
void MainWindow::IPSettingsHandler( bool b )
{
    ui->dhcpGroupBox->setEnabled( b );

    if( !b )
        commandBuffer.insert( "IP DHCP","true" );
    else
    {
        commandBuffer.insert( "IP",ipWidgetMap.value("IP")->getIP() );
        commandBuffer.insert( "Gateway",ipWidgetMap.value("Gateway")->getIP() );
        commandBuffer.insert( "Netmask",ipWidgetMap.value("Netmask")->getIP() );
    }
    ui->applyButton->setEnabled(true);
}

/*
 * Set IP manually setting checker
 */
void MainWindow::autoipCheckBoxState( int state )
{
    //if ( isSessionBegin )
        commandBuffer.insert( "Autoip", QString( "%1" ).arg( state ) );
        ui->applyButton->setEnabled(true);
}


/*
 * Create command line for sending to device
 */
void MainWindow::createCommandLine()
{
   QString val;
   QString header;
   QString commandLine;
   QMutableMapIterator<QString,QString> iterator( commandBuffer );

   while( iterator.hasNext() )
    {
        iterator.next();
        if ( !iterator.value().isEmpty() )
        {
            header = iterator.key();
            val = iterator.value();

            if ( header.contains( "DHCP" ) )
            {
                if( val.contains( "true" ) )
                {
                    val = "";
                }
            else continue;
            }
            else if( header.contains("Autoip") )
                val = val.contains( "0" )?"0":"1";

            if( header.contains( "line type" ) )
                commandLine = header+val;
            else commandLine = QString( "%1 %2 %3" ).arg( "ifconfig" ).arg( header ).arg( val );
            qDebug()<<"command to device: "<<commandLine;

            dataHandler( commandLine ) ;
            iterator.setValue( "" );
        }
    }
   ui->applyButton->setEnabled(false);
}

/*
 * Open serial port
 */
void MainWindow::openSerialPort()
{
    while( portName.isNull() )
    {
        if( isSessionBegin )
        {
        switch( execWarningWindow( "Port unavailable. \n Check port state" ) )
            {
                case QMessageBox::Ok:
                    break;
                case QMessageBox::Cancel:
                    closeSerialPort();
                    return;
            }
        }
        else return;
    }

    if( serial->isOpen() )
        return;

    serial->setPortName( portName );
    serial->setBaudRate( baudRate );

    if ( serial->open( QIODevice::ReadWrite ) )
    {
        if ( isSessionBegin )
            return;
        if( !checkDeviceInitialState() )
        {
            switch ( execWarningWindow( "Device unavailable. \n Check device state" ) )
            {
                case QMessageBox::Ok:
                    break;
                case QMessageBox::Cancel:
                    closeSerialPort();
                return;
            }
        }

        while ( !getInitInformation() )
        {
            switch ( execWarningWindow( "Device unavailable. \n Check device state" ) )
            {
                case QMessageBox::Ok:
                    break;
                case QMessageBox::Cancel:
                    closeSerialPort();
                    return;
            }
        }
        isSessionBegin = true;
        openSession();
    }
    else
    {
        QMessageBox::critical( this, tr( "Error" ), serial->errorString() );
        showStatusMessage( tr( "Open error" ),10000 );
        closeSerialPort();
    }
}

/*
 * Check is device ready to answer
 */
bool MainWindow::checkDeviceInitialState()
{
    QString buffer;
    for( int i=0; i<3; i++ )
    {
        sendData( "\n" );
        buffer = getData();

        if( !buffer.compare( "\r\n>" ) )
            return true;
    }
    return false;
}

/*
 * Close serial port and current session
 */
void MainWindow::closeSerialPort()
{
    serial->close();
    closeSession();
    setPortsMenu();
    showStatusMessage( tr( "" ),0 );
}

void MainWindow::closeSession()
{
    foreach( QComboBox* combo, lineCombosList )
         combo->clear();

    ui->connectBtn->show();
    ui->connectBtn->setText( "Connect" );
    connectAction->setText( "Connect" );
    this->setFixedSize(*size );
    ui->labelConnectedPort->show();
    ui->labelConnected->hide();
    ui->labelStatus->hide();
    resetConfiguration->setEnabled(false);
    ui->dipswitch->hide();
    ui->numberOfLines->hide();
    ui->autocompleter->hide();
    ui->deviceName->hide();
    ui->deviceVersion->hide();
    ui->tabWidget->hide();
    ui->applyButton->hide();
    ui->rebootBtn->hide();
    isSessionBegin = false;
}

/*
 * Handling with the serial port state
 */
void MainWindow::handleSerialPort()
{
    if( serial->isOpen() )
    {
        closeSerialPort();
    }
    else openSerialPort();
}

void MainWindow::about()
{
    QMessageBox::about( this, tr( "About CoolAutomation Tool" ),
                       tr( "The <b>CoolAutomation Tool</b> is a tool "
                          "for handling CoolAutomation devices." ) );
}

/*
 * Write data to port
 */
void MainWindow::sendData( const QString &command )
{
    currentCommand = command;
    QByteArray data = command.toLocal8Bit();
    serial->write( data );
}

/*
 * Read data from port
 */
QString MainWindow::getData()
{
    QString readData = serial->readAll();
    while( serial->waitForReadyRead( 100 ) )
       readData.append( serial->readAll() );

    if ( serial->error() == QSerialPort::ReadError )
        showStatusMessage( QString( "Failed to read from port %1, error: %2" )
                                 .arg( serial->portName() ).arg( serial->errorString() ),10000 );
    else if ( serial->error() == QSerialPort::TimeoutError && readData.isEmpty() )
        showStatusMessage( QString( "No data was currently available for reading from port %1" )
                                 .arg( serial->portName() ),10000 );

    return QString( readData );
}

/*
 * Received data processing
 */
int MainWindow::dataHandler( const QString &command )
{
    int i;
    QString buffer;
    QList<QStringList> rows ;

    sendData( command+"\n" );
    buffer = getData();
    qDebug()<<buffer;

    if ( command.contains( "boot 2" ) )
    {

        if( buffer.contains( "Build:",Qt::CaseInsensitive ) )
            return 1;
        else return 0;
    }

    if ( buffer.isEmpty() )
    {
        if ( isSessionBegin )
        {
            showStatusMessage( tr( "Device unavailable" ),10000 );
            ui->labelConnected->setText( "Timeout" );
            ui->labelConnected->setStyleSheet( "color: red" );
        }
        else
        {
            ui->labelConnectedPort->setText( tr( "Device unavailable" ) );
        }

        return 0;
    }

    if( !buffer.contains( "OK" ) )
    {
        if ( buffer.contains( "Dip switch",Qt::CaseInsensitive ) )
             QMessageBox::warning( 0,"Warning!","Dip switches state incorrect. \n Check it, please!" );

        QStringList errors = buffer.split( "\r\n" );
        showStatusMessage( errors.at(1),10000 );
        return 0;
    }
    else
    {
        QStringList list = buffer.split( "\r\n",QString::SkipEmptyParts );

        if ( list[0].contains( command ) )
            list.removeFirst();

        if ( list[0].contains( "OK" ) )
        {
            if( list[0].contains( "Boot Required", Qt::CaseInsensitive )
                ||command.contains( "DHCP" ) )
                ui->rebootBtn->setEnabled(true);
            showStatusMessage( list[0],10000 );
            return 1;
        }

        list.removeLast();
        list.removeLast();

        for ( i=0; i<list.size(); i++ )
            rows.append( list[i].split( ": ", QString::SkipEmptyParts ) );

        if ( command.contains( "ifconfig" ) )
            fillNetworkConfigForm( rows );
        else if ( command.contains( "set" ) )
            fillSettings( rows );
        else if ( command.contains( "line" ) )
            fillLinesForm( rows );

    }
    return 1;
}

/*
 * Set general settings
 */
void MainWindow::fillSettings( QList<QStringList> rows )
{
    foreach ( QStringList row,rows )
        properties.insert( row.at(0).trimmed(),row.at(1).trimmed() );
    numbHVAC = properties.value( "HVAC lines" ).toInt();
    echo = properties.value( "echo" ).toInt();
    getDeviceInfo( properties.value( "S/N" ) );
}

/*
 * Set lines settings
 */
void MainWindow::fillLinesForm( QList<QStringList> rows )
{
    QStringList lineProperty;
    int         nLine;

    for( int i=0;i<rows.size();i=i+2 )
    {
        QString strKey = rows[i].at(0);

        strKey = strKey.trimmed();
        nLine = strKey.data()[1].digitValue(); /*set line number*/

        lineProperty = rows[i].at(1).split( " " );
        lineCombosList.at( nLine-1 )->setCurrentIndex( lineCombosList.at( nLine-1 )->findData( lineProperty.at(0),Qt::DisplayRole ) );
        lineLabelList.at( nLine-1 )->setText( QString( "Line %1" ).arg( nLine ) );
    }
    ui->numberOfLines->setText( QString( "HVAC lines: %1" ).arg( numbHVAC ) );

    if ( numbHVAC>1 )
        comboBoxFilter();
    createDipSwitches();
}

/*
 * Show connected device info
 */
void MainWindow::getDeviceInfo( QString version )
{
    if ( version.contains( QRegExp( "^02C1" ) ) )
        devType = COOLINKHUB;
    else if ( version.contains( QRegExp( "^05" ) ) )
        devType = COOLMASTER;
    else if ( version.contains( QRegExp( "^03D" ) ) )
        devType = COOLPLUG;
    else if ( version.contains( QRegExp( "^02C0" ) ) )
        devType = COOLINK;
    ui->deviceName->setText( devTypeStrings[devType] );
    ui->deviceVersion->setText( "S/N "+version );
}

/*
 * Set network configuration page
 */
void MainWindow::fillNetworkConfigForm( QList<QStringList> rows )
{
    QStringList rowElements;
    bool flag;

    foreach ( rowElements,rows )
    {
        if( rowElements[0].contains( "mac",Qt::CaseInsensitive ) )
                ui->macLineEdit->setText( rowElements[1] );
        else if( rowElements[0].contains( "link",Qt::CaseInsensitive ) )
                continue;
        else if ( rowElements[0].contains( "autoip",Qt::CaseInsensitive ) )
        {
            flag = rowElements[1].contains( "Yes",Qt::CaseInsensitive )? true : false;
            ui->autoipCheckBox->setChecked( flag );
        }
        else if ( rowElements[1].contains( "." ) )
        {
            if ( rowElements[0].contains( "IP", Qt::CaseInsensitive ) )
            {
                if( rowElements[1].contains( "DHCP" ) )
                {
                    ui->dhcpRadioButton->setChecked(true);
                    ui->dhcpGroupBox->setEnabled(false);
                }
                else ui->radioButtonSetIPManually->setChecked(true);

                QString h = rowElements[1].section( " ",0,0 );
                rowElements[1] = h;
            }

            ipWidgetMap.value( QString( "%1" )
                                   .arg( rowElements[0].trimmed() ))->setIP(rowElements[1]);
        }
    }
}

/*
 * Get initial information
 */
bool MainWindow::getInitInformation()
{
    QStringList list;
    list << "set"<<"ifconfig"<<"line";

    for( int i =0 ; i<list.size();i++ )
    {
        dataHandler( list.at(i) );
        if ( i==0 )
            setLinesInitialState();
    }
   return true;
}

/*
 * Reboot processing
 */
void MainWindow::on_rebootBtn_clicked()
{
    QTime timer;

    showStatusMessage( tr( "Wait, please!.." ),10000 );
    ui->labelConnected->setText( "Initializing..." );
    ui->labelConnected->setStyleSheet( "color: red" );

    if ( !rebootProcessing() )
    {
        showStatusMessage( tr( "Reboot error!" ),10000 );
        return;
    }

    timer.start() ;
    while( timer.elapsed() < 2000 )
             qApp->processEvents(0);

    dataHandler( "ifconfig" );

    ui->rebootBtn->setEnabled(false);
    ui->rebootBtn->clearFocus();
    showStatusMessage( tr( "" ),0 );
    ui->labelConnected->setText( "Connected" );
    ui->labelConnected->setStyleSheet( "color: green" );
}

bool MainWindow::rebootProcessing()
{
    int rc;
    rc = dataHandler( "boot 2" );
    if( !rc )
    {
        return false;
    }
    else
    {
        for (int i=0; i<30; i++)
        {
            rc = dataHandler( "line" );
            if( rc )
                return true;
        }
        return false;
    }
}

/*
 * Open session
 */
void MainWindow::openSession()
{
    ui->labelConnectedPort->hide();
    ui->connectBtn->hide();
    connectAction->setText( "Disconnect" );
    connectAction->setEnabled(true);
    ui->labelConnected->show();
    ui->labelStatus->show();
    ui->dipswitch->show();
    ui->numberOfLines->show();
    ui->labelConnected->setText( "Connected" );
    ui->labelConnected->setStyleSheet( "color: rgb( 81, 147, 49 )" );
    ui->deviceName->show();
    ui->deviceVersion->show();

    ui->networkConfigBtn->setEnabled(true);
    ui->hvacLine->setEnabled(true);
    ui->autoipCheckBox->show();

    ui->tabWidget->show();
    ui->tabWidget->setCurrentWidget( ui->netSet );

    ui->applyButton->show();
    ui->applyButton->setEnabled(false);
    ui->rebootBtn->show();
    resetConfiguration->setEnabled(true);

    this->setFixedSize( 543,424 );

}

/*
 * Error processing
 */
void MainWindow::handleError( QSerialPort::SerialPortError error )
{
    if ( error == QSerialPort::ResourceError )
    {
        QMessageBox::critical( this, tr( "Critical Error" ), serial->errorString() );
        closeSerialPort();
    }
}

/*
 * Show status
 */
void MainWindow::showStatusMessage( const QString &message, int time )
{
    //status->setText( message );
    if ( message=="" )
    {
        ui->statusBar->clearMessage();
        return;
    }
    ui->statusBar->showMessage( message,time );
}

MainWindow::~MainWindow()
{
    delete ui;
}
