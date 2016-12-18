#include "mainwindow.h"
#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <qDebug>
#include <Dbt.h>
#include <initguid.h>
#include "ui_mainwindow.h"
#include "consolecontroller.h"
#include "consoleview.h"
#include <QFontDatabase>
#include <qlogging.h>
#include "infowidget.h"
#include <QPointer>
#include "logbrowser.h"

#define NAMEAPP "Quick Installation Tool"
#define VERSION " v1.0.3"
#define N_INFO_TABS 2

DEFINE_GUID( GUID_DEVINTERFACE_USBDEVICE,0x4D36E978L, 0xE325,
             0x11CE,0xBF,0xC1,0x08,0x00,0x2B,0xE1,0x03,0x18 );
#define VENDOR_ID 10668

/* Hot keys:
*  alt+t = terminal mode
*   CTRL+SHIFT+} = debug/trace
*   alt+a = autocomplete
*   alt+c = clear
*/

bool comparator( const QSerialPortInfo &info1, const QSerialPortInfo &info2 );
bool comparatorLineTypeDescr(int t1, int t2);
void sortDescrList(QList<int> dev);
void insertItemToLineCombo(QList<int> dev, QComboBox* box);
QPointer<LogBrowser> logBrowser;
bool isLogBrowserRunning;

void myMessageOutput(QtMsgType type, const QMessageLogContext &, const QString & str)
{
    //const char * msg = str.toStdString().c_str();
    if( logBrowser )
        logBrowser->outputMessage( type, str );
}

MainWindow::MainWindow( QWidget *parent ):QMainWindow( parent ),
            ui( new Ui::MainWindow )
{
    this->setWindowFlags( Qt::Dialog | Qt::WindowSystemMenuHint |
                          Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint |
                          Qt::MSWindowsFixedSizeDialogHint |
                          Qt::MaximizeUsingFullscreenGeometryHint );
    ui->setupUi(this);
    setTitle();
    /* Size of window - the first appearence */
    size = new QSize( 600,450 );
    setFixedSize(*size);

    warning = new WarningWidget;
    connect(warning,SIGNAL(buttonClicked()),this,SLOT(closeSession()));
    connect(warning,SIGNAL(terminalButtonClicked()),this , SLOT(onTerminalButtonClicked()));

    _isVCOM = true;
    isConnection = false;
    isConnectionProcessRun=false;
    isDisconnect = false;
    isTerminalModeSwitchOn = false;
    isConsoleSwitchOn = false;

    /* List of device names in used */
    devTypeStrings<<"CoolMasterNet"<<"CoolLinkNet"<<"CoolLinkHub"<<"CoolPlug"<<"CoolPlugSuperviser";
    pagesList<<"line"<<"ifconfig";
    warningStrings<<"No Way To Connect"<<"Connection Lost"
                  <<"Port Unavailable"<<"Device Unavailable"<<"Open Error"
                  <<"Switches: Incorrect State"<<"Resource Error"<<"Reboot Requied"
                  <<"Reading Error"<<"No Data In Port"<<"Indefinite Device Type";

    devType = -1;
    centralWindow = ui->centralWidget;

    /* install welcome page connect button */
    connectGroup = new QFrame;
    connectBtn = new QPushButton(connectGroup);
    connectBtn->setProperty("connectionStart", false);
    connectBtn->setObjectName("connectBtn");
    connectBtnLayout = new QHBoxLayout;
    connectBtnLayout->addStretch();
    connectBtnLayout->addWidget(connectBtn);
    connectBtnLayout->addStretch();
    QVBoxLayout* connectGrLayout = new QVBoxLayout;
    labelConnectedPort = new QLabel(connectGroup);
    labelConnectedPort->setProperty("labelConnectedPort",true);
    labelConnectedPort->setAlignment(Qt::AlignCenter);
    connectGroup->setLayout(connectGrLayout);
    connectGrLayout->addWidget(connectBtn);
    connectGrLayout->addWidget(labelConnectedPort);
    setConnectButton();


    /* set warning layout */
    ui->mainLayout->addWidget(warning,0,0,1,3);
    ui->mainLayout->addWidget(connectGroup,1,1,Qt::AlignCenter);
    warning->hide();

    initializeDeviceNotification();

    /* read from and set application properties and styles */
    settings = new QSettings( "settings.ini",QSettings::IniFormat );
    portName = ( settings->value("settings/port") ).toString();
    baudRate = ( settings->value("settings/baudrate") ).toInt();
    serial = new QSerialPort(this);
    QFile file(":/styles.css");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);
    qApp->setFont(QFont("Tahoma", 9));
    setCustomedFont();
    setupConnectMenu();

    labelDeviceVersion = new QLabel;
    isSessionBegin = false;
    sysMsgTimeGate = false;
    isReboot = false;

    initGeneralGUIConnections();

    /* create trace window */
    logBrowser = new LogBrowser(this);
    isLogBrowserRunning = false;
    QShortcut *logHotKey = new QShortcut(this);
    logHotKey->setContext(Qt::ApplicationShortcut);
    logHotKey->setKey(Qt::CTRL +Qt::SHIFT +Qt::Key_BracketRight);
    connect(logHotKey,SIGNAL(activated()),this, SLOT(logHotKeyPressed()));
    QShortcut *terminalMode = new QShortcut(this);
    terminalMode->setKey(Qt::ALT+Qt::Key_T);
    connect(terminalMode,SIGNAL(activated()),this,SLOT(setTerminalMode()));
}

/*
 *  setup general connections between GUI and logical elements
*/
void MainWindow::initGeneralGUIConnections()
{
     connect( connectAction,SIGNAL(triggered()), this, SLOT(handleSerialPort()) );
     connect( serial, SIGNAL( error( QSerialPort::SerialPortError ) ),
              this, SLOT( handleError( QSerialPort::SerialPortError ) ) );
     connect( portsMenu, SIGNAL( triggered( QAction* ) ),this, SLOT( checkCustomPortName( QAction* ) ) );
     connect( ratesMenu, SIGNAL( triggered( QAction* ) ),this,SLOT( setCustomBaudRate( QAction* ) ) );
}

void MainWindow::logHotKeyPressed()
{
    if( !isLogBrowserRunning )
    {
        qInstallMessageHandler(myMessageOutput);
        isLogBrowserRunning = true;
    }
    else
    {
        qInstallMessageHandler(0);
        isLogBrowserRunning = false;
    }
    logBrowser->hideBrowser();
}

void MainWindow::setCustomedFont()
{
    QStringList list;
    list << "Linotype.otf" << "LinotypeBold.otf" << "LinotypeLight.otf" << "LinotypeMedium.otf"<<"LinotypeThin.otf";
    int fontID(-1);
    bool fontWarningShown(false);
    for (QStringList::const_iterator constIterator = list.constBegin(); constIterator != list.constEnd(); ++constIterator) {
       // qDebug()<<*constIterator;
        QFile res(":/otf/" + *constIterator);
        if (res.open(QIODevice::ReadOnly) == false) {
            if (fontWarningShown == false) {
                QMessageBox::warning(0, "Application", (QString)"Fonts not founded " + QChar(0x00AB) + " Neue Haas Unica " + QChar(0x00BB) + ".");
                fontWarningShown = true;
            }
        } else {
            fontID = QFontDatabase::addApplicationFontFromData(res.readAll());
            if (fontID == -1 && fontWarningShown == false) {
                QMessageBox::warning(0, "Application", (QString)"Fonts not founded " + QChar(0x00AB) + " Neue Haas Unica " + QChar(0x00BB) + ".");
                fontWarningShown = true;
            }
        }
    }
 /* Used  "Neue Haas Unica Pro","Neue Haas Unica Pro Light","Neue Haas Unica Pro Medium","Neue Haas Unica Pro Thin"*/
}


void MainWindow::setTitle()
{
    QCoreApplication::setApplicationName(NAMEAPP);
    QCoreApplication::setApplicationVersion(VERSION);
    QString str = QCoreApplication::applicationName()+QCoreApplication::applicationVersion();
    setWindowTitle(str);
}

/******************** operations with connect button *********************************/

void MainWindow::setConnectButton()
{
    connect(connectBtn,SIGNAL(clicked()),this, SLOT(connectButtonClicked()));
    connectBtn->setFixedWidth(250);
    connectBtn->setFixedHeight(45);
    connectBtn->setAutoFillBackground(true);
    connectBtn->installEventFilter(this);
    connectBtn->setText("Connect");
    setConnectButtonIcon(tr(":/images/Arrow-right.png"),Qt::RightToLeft);
}

void MainWindow::setConnectButtonIcon(QString path,Qt::LayoutDirection direction)
{
    QPixmap pix(path);
    QIcon icon(pix);
    connectBtn->setIcon(icon);
    connectBtn->setLayoutDirection(direction);
    QSize icoSize = pix.size();
    icoSize.scale( 20, 20, Qt::KeepAspectRatio );
    connectBtn->setIconSize(icoSize);
}

void MainWindow::connectButtonClicked()
{
    if (!warning->isHidden())
        warning->hide();

    QTime timer;
    if ( isConnection && (!isConnectionProcessRun) )
    {
        connectBtn->setText("  Connecting...");
        connectBtn->setProperty("connectionStart", true);
        setConnectButtonIcon(tr(":/images/Arrow-right.png"),Qt::LeftToRight);

        timer.start() ;
        while( timer.elapsed() < 200 )
            qApp->processEvents(0);

        isConnectionProcessRun = openSerialPort();
    }
}

void MainWindow::connectBtnNonCom()
{
    connectBtn->setEnabled(false);
    setConnectButtonIcon(tr(":/images/Arrow-right-grey.png"),Qt::RightToLeft);
}

/******************************************************************************/
/*****************************Error processing********************************/

void MainWindow::handleError( QSerialPort::SerialPortError error )
{
    qDebug()<<"serial error: "<<error<<": "<<serial->errorString();

    if ( error == QSerialPort::ResourceError||
         error == QSerialPort::PermissionError )
    {
        if((serial->errorString().contains("Access is denied")))
                return;

        if( isReboot )
            return;
        isDisconnect = true;

        if ( isTerminalModeSwitchOn )
            emit signal_DevDisconnToConsole();
        else
        {
           showWarning(SESSION_CONNECTION_LOST);
           headerWidget->handleLabelConnected(DISCONNECTED);
           headerWidget->updateButton->setEnabled(false);
           if ( consoleView->consoleIsRunning() )
               emit signal_DevDisconnToConsole();
           // serial->close();
        }
    }
}

void MainWindow::showWarning(int type)
{
    switch( type )
    {
    case INDEF_DEV_TYPE:
        warning->terminalButton->setText(" Run terminal ");
        warning->terminalButton->show();
        warning->warningButton->hide();
        break;
    case SESSION_CONNECTION_LOST:
        warning->warningButton->setText(" Disconnect ");
        warning->terminalButton->hide();
        warning->warningButton->show();
        break;
    case DEVICE_UNAVAILABLE:
    case OPEN_ERROR:

    case SWITCHES_INCORRECT_STATE:
    case REBOOT_REQUIRED:
    case RESOURCE_ERROR:
        warning->terminalButton->hide();
        warning->warningButton->hide();
        break;
    case USB_NOT_CONNECTED:
        warning->terminalButton->hide();
        warning->warningButton->hide();
        break;
    default:
        break;
    }
    warning->setWarningText(warningStrings.at(type));
    warning->show();
}

void MainWindow::handlingUnavailableState()
{
    showWarning( DEVICE_UNAVAILABLE );

    if (isConnectionProcessRun)
    {
        connectBtn->setProperty("connectionStart", false);
        connectBtn->setText("Connect");
        setConnectButtonIcon(tr(":/images/Arrow-right.png"),Qt::RightToLeft);
        isConnectionProcessRun = false;
        closeSerialPort();
    }
}

/************************************************************************************/
/*********************** Operations with connection menu *****************************/
 /*
  * Set "Connection" menu
 */
 void MainWindow::setupConnectMenu()
 {
     QFont font("Calibri");
     ui->menuConnect_2->setFont(font);
     portsMenu = ui->menuConnect_2->addMenu( tr( "&Serial Port" ) );
     ratesMenu = ui->menuConnect_2->addMenu( tr( "Baud &Rate" ) );
     connectAction = new QAction( tr( "&Connect" ), this );
     ui->menuConnect_2->addAction( connectAction );
     enumPorts();
     setRatesMenu();
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
     baudRate = static_cast<QSerialPort::BaudRate>( (currentAction->data()).toInt() );
     serial->setBaudRate(baudRate);
     settings->setValue( "settings/baudrate",baudRate ) ;
     settings->sync();
 }

 /*
  * Set "Serial Port" menu in the "Connection" menu
 */
 void MainWindow::enumPorts()
 {
     QAction* action;
     QString port;
     portsList = QSerialPortInfo::availablePorts();
     bool isChecked = false;
     bool isPresented = false;
     portsMenu->clear();
     if( !portsList.count() )
     {
         portName.clear();
         labelConnectedPort->setText( "No COM Connection Found" );
         connectBtnNonCom();
         if( !isReboot )
         {
            showWarning(USB_NOT_CONNECTED);
            connectAction->setEnabled(false);
            portsMenu->setEnabled(false);
         }
         isConnection=false;
         return;
     }
     /* Sort serial ports by name */
     qSort( portsList.begin(),portsList.end(),comparator );

     /* enum serial port actions in "Serial Port" menu */
     for ( int i=0; i<portsList.count(); i++ )
     {
        getInfoAboutInstalledSerialPorts(portsList.at(i));
        QString descr = portsList[i].description();

        if ( !descr.contains( "Bluetooth",Qt::CaseInsensitive )
             && portsList[i].isValid())
        {
            port = portsList[i].portName();
            action = new QAction( port,this );
            action->setProperty("vid",portsList[i].vendorIdentifier());

            switch( portsList[i].vendorIdentifier())
            {
            case VENDOR_ID:
            action->setIcon(QPixmap(":/images/data_cable.png"));
                break;
            case 0:
                action->setIcon(QPixmap(":/images/port-icon.png"));
                break;
            default:
                action->setIcon(QPixmap(":/images/usb.png"));
                break;
            }

            action->setIconVisibleInMenu(true);
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
     }

     /* check if any serial port is available */
     if ( !isPresented )
     {
         labelConnectedPort->setText( "None Available Port" );
         if( !isReboot )
         {
            showWarning(USB_NOT_CONNECTED);
            connectAction->setEnabled(false);
            portsMenu->setEnabled(false);
         }
         connectBtnNonCom();
         portName.clear();
         isConnection=false;
         return;
     }

     /* check if serial port is checked */
     if ( !isChecked )
     {
         QList<QAction*> actionsList = portsMenu->actions();
         portName = actionsList.first()->text() ;
         int y = (actionsList.first()->property("vid")).toInt();
         if (y==VENDOR_ID)
                _isVCOM = true;
         else _isVCOM = false;
         actionsList.first()->setChecked(true);
     }

     /* information text in welcome box */

     connectBtn->setEnabled(true);
     connectAction->setEnabled(true);
     portsMenu->setEnabled(true);

     setConnectButtonIcon(tr(":/images/Arrow-right.png"),Qt::RightToLeft);
     labelConnectedPort->setText( "via "+portName );
     isConnection=true;
     qDebug()<<_isVCOM;
 }

 void MainWindow::getInfoAboutInstalledSerialPorts(QSerialPortInfo info)
 {
     qDebug()<<"description: "<<info.description()<<"\nmanufacturer: "<<info.manufacturer()
            <<"\nport name: "<<info.portName()<<"\nserial number: "<<info.serialNumber()
            <<"\npid: "<<info.productIdentifier()<<"\nvid: "<<info.vendorIdentifier()
            <<"\nsystem location:"<<info.systemLocation();
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
     QList<QAction*> actionsList = portsMenu->actions();
     foreach( QAction* act, actionsList )
        act->setChecked(false);

     action->setChecked(true);
     portName = action->text();

     if( (action->property("vid")).toInt()==VENDOR_ID )
        _isVCOM = true;
     else _isVCOM = false;

     labelConnectedPort->setText( "via "+portName );
     settings->setValue( "settings/port",portName ) ;
     settings->sync();
 }
/****************************************************************************/



/*
 *  Register the application for the device notification in Windows
*/
bool MainWindow::initializeDeviceNotification( void )
{
    DEV_BROADCAST_DEVICEINTERFACE devInt;
    ZeroMemory( &devInt, sizeof(devInt) );
    devInt.dbcc_size        = sizeof( DEV_BROADCAST_DEVICEINTERFACE );
    devInt.dbcc_devicetype  = DBT_DEVTYP_DEVICEINTERFACE;
    devInt.dbcc_classguid   = GUID_DEVINTERFACE_USBDEVICE;
    m_hDeviceNotify = RegisterDeviceNotification( (HANDLE)winId(),&devInt,DEVICE_NOTIFY_WINDOW_HANDLE );
    if( m_hDeviceNotify == 0 )
    {
        qDebug() << "Error: Failed to register device notification!";
        qApp->quit();
    }
    return true;
}

/*
 * Standard overrided method for system event handling (plug/unplug)
 */
bool MainWindow::nativeEvent( const QByteArray& eventType, void* message,
                             long* result )
{
    Q_UNUSED( result );
    Q_UNUSED( eventType );

    MSG* msg = reinterpret_cast<MSG*>( message );
    PDEV_BROADCAST_HDR pHdr;
    PDEV_BROADCAST_DEVICEINTERFACE pDevInf;

    if( msg->message == WM_DEVICECHANGE )
    {
        QString str;
        pHdr = (PDEV_BROADCAST_HDR)msg->lParam;
        pDevInf = (PDEV_BROADCAST_DEVICEINTERFACE)pHdr;
        switch( msg->wParam )
        {
        case DBT_DEVICEARRIVAL:

            str = QString::fromWCharArray( pDevInf->dbcc_name).remove(0,7);
            qDebug()<<str<<" device connected";
            deviceConnected();
            break;
        case DBT_DEVICEREMOVECOMPLETE:
            str = QString::fromWCharArray( pDevInf->dbcc_name).remove(0,7);
            qDebug()<<str<<" device disconnected";
            deviceDisconnected();
            break;
        }
    }
    return false;
}

/*
*  slot for plugged device
*/
void MainWindow::deviceConnected()
{
    enumPorts();

    if( isDisconnect && (isSessionBegin||isTerminalModeSwitchOn) )
    {
        if( isPortAvailable(prevPortName) )
        {
            portName = prevPortName;
            if (openSerialPort())
            {
                isDisconnect = false;
                emit signal_DevConnToConsole();
                if ( isTerminalModeSwitchOn )
                    return;
                if (!warning->isHidden())
                    warning->hide();
                headerWidget->handleLabelConnected(CONNECTED);
                if(!consoleView->consoleIsRunning())
                    headerWidget->updateButton->setEnabled(true);
                return;
            }
        }
        else
        {
            if ( !isTerminalModeSwitchOn )
                closeSession();
        }
    }
}

/*
*  slot for unplugged device
*/
void MainWindow::deviceDisconnected()
{
    if (isDisconnect)
    {
        if( serial->isOpen() )
            closeSerialPort();
    }
    enumPorts();
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
* Check is current port available
*/
bool MainWindow::isPortAvailable(const QString &port)
{
    QList<QAction*> actionList = portsMenu->actions();
    foreach(QAction* action, actionList)
    {
        if( !port.compare(action->text()) )
             return true;
    }
    return false;
}

/*
 * Handling with the serial port state
 */
void MainWindow::handleSerialPort()
{
    if( isSessionBegin )
    {
        closeSession();
        isDisconnect = false;
    }
    else
        connectButtonClicked();
}

/*
 * Open serial port
 */
bool MainWindow::openSerialPort()
{
    if( portName.isNull() )
    {
        showWarning(PORT_UNAVAILABLE);

        return false;
    }

    if( serial->isOpen() )
        return false;

    serial->setPortName( portName );
    serial->setBaudRate( baudRate );
    isVCOM = _isVCOM;

    if ( serial->open( QIODevice::ReadWrite ) )
    {
        prevPortName = portName;


        if ( isTerminalModeSwitchOn )
        {
            if (isReboot )
                return false;
            if (isDisconnect)
                return true;
            openTerminal();
            isSessionBegin = true;
            return true;
        }

        if ( isSessionBegin )
            return true;

        if( !checkDeviceInitialState() )
        {
            handlingUnavailableState();
            closeSerialPort();
            setConnectButtonToStartPosition();
            return false;
        }

        while ( !getInitInformation() )
        {
            handlingUnavailableState();
            closeSerialPort();
            setConnectButtonToStartPosition();
            return false;
        }
        isSessionBegin = true;
        return openSession();
    }
    else
    {
        qDebug()<<serial->errorString();
        showWarning(OPEN_ERROR);
        closeSerialPort();
        setConnectButtonToStartPosition();
        return false;
    }
}

void MainWindow::setConnectButtonToStartPosition()
{
    connectBtn->setProperty("connectionStart", false);
    connectBtn->setText("Connect");
    setConnectButtonIcon(tr(":/images/Arrow-right.png"),Qt::RightToLeft);
}

/*
* Set connections between GUI elements and logical elements
*/
void MainWindow::initGUIConnections()
{
  connect( applyButton, SIGNAL(clicked()),this,SLOT(createCommandLine()) );
  connect(linesTab, SIGNAL(bufferFilled(bool)),this,SLOT(applyButtonEnable(bool)));
  connect(rebootBtn, SIGNAL(clicked()),linesTab,SLOT(switchNumbersToInitialState()));
  connect(netConfigTab, SIGNAL(bufferFilled(bool)),this,SLOT(applyButtonEnable(bool)));
  connect( this, SIGNAL( releaseLoop() ),&loop,SLOT( quit() ) );
  connect( rebootBtn, SIGNAL(clicked()),this, SLOT(rebootBtnClicked() ));
}

void MainWindow::setConfigsGUI()
{
    applyButton = new QPushButton;
    netConfigTab = new NetConfigTab(&commandBuffer);
    netConfigTab->setProperty("rightConfigTab",true);
    linesTab = new LinesTab(&commandBuffer,  devType, numbHVAC);
    linesTab ->setProperty("rightConfigTab",true);
    tabWidget->addTab(netConfigTab,tr("Network Settings"));
    tabWidget->addTab(linesTab, tr("HVAC Lines"));
    tabWidget->tabBar()->setProperty("singleType",false);
    //tabWidget->addTab(infoWidget,tr("Info"));
    connectAction->setText("Disconnect");
}

void MainWindow::insertConfigGUIToPage()
{
    configPageLayout = new QGridLayout;
    configPageLayout->setObjectName("configPageLayout");

    /* set header string*/
    headerWidget = new HeaderWidget;
    connect( headerWidget->updateButton, SIGNAL(clicked()),this, SLOT(getPageInformation()) );
    headerWidget->deviceNameLabel->setText( devTypeStrings[devType] );

    /* set footer string*/
    applyButton->setObjectName("applyButton");
    applyButton->setEnabled(false);
    applyButton->setText("Apply");
    applyButton->setProperty("configButton", true);

    rebootBtn = new QPushButton;
    rebootBtn->setText("Reboot");
    rebootBtn->setObjectName("rebootButton");
   // rebootBtn->setProperty("rebootButton", true);
    rebootBtn->setEnabled(false);

    footerLayout = new QHBoxLayout;
    footerLayout->addStretch();
    footerLayout->addWidget(applyButton);
    footerLayout->addWidget(rebootBtn);
    footerLayout->setContentsMargins(0,0,15,0);
    footerLayout->setSpacing(10);

    configPageLayout->addWidget(headerWidget,0,0,1,3);
    configPageLayout->addWidget(tabWidget,1,0,1,3);
    configPageLayout->addLayout(footerLayout,2,0,1,3);
    configPageLayout->setMargin(10);
    configPageLayout->setSpacing(10);
    ui->mainLayout->addLayout(configPageLayout,1,0,1,3);

    initGUIConnections();
}

void MainWindow::deleteWelcomePageElements()
{
    connectGroup->hide();
    connectBtn->hide();
}

/*
*  Handling with tab widget pages
*/
void MainWindow::tabOperating( int index )
{
    switch(index)
    {
        case 1:
            if ( consoleView->consoleIsRunning() )
                closeConsoleView();
            if( !isDisconnect )
                dataHandler("line");
            this->setFixedSize(650,575);
            break;
        case 0:
            if ( consoleView->consoleIsRunning() )
                closeConsoleView();
            if( !isDisconnect )
                dataHandler("ifconfig");
            this->setFixedSize(650,400);
            break;
        case 2:
            runConsoleView();
            break;
        default:
            break;
    }
}
/*************** interactions with device *******************/

/*
 * Check is device ready to answer
 */
bool MainWindow::checkDeviceInitialState()
{
    QString buffer;
    for( int i=0; i<10; i++ )
    {
        sendData( "\n" );
        buffer = getData();
        qDebug()<<buffer;
        if( buffer.contains( ">" ) )
           return true;
    }
    return false;
}

/*
 * Get initial information
 */
bool MainWindow::getInitInformation()
{
   dataHandler( "set" );
   return true;
}

/*
 * Received data processing
 */
int MainWindow::dataHandler( const QString &command )
{
    int i;
    QString buffer;
    QList<QStringList> rows ;
    sendData( command+"\r\n" );
    buffer = getData();
    qDebug()<<"buffer: "<<buffer;

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
            showWarning( SESSION_CONNECTION_LOST );
            headerWidget->handleLabelConnected(DISCONNECTED);
        }
        else
            handlingUnavailableState();
        return 0;
    }

    if ( isSessionBegin )
    {
        headerWidget->handleLabelConnected(CONNECTED);
    }

    if( !buffer.contains( "OK" ) )
    {
        if ( buffer.contains( "Dip switch",Qt::CaseInsensitive ) )
        {
             showWarning( SWITCHES_INCORRECT_STATE );
             rebootBtn->setEnabled(true);
        }
        return 0;
    }
    else
    {
        QStringList list = buffer.split( "\r\n",QString::SkipEmptyParts );

        if ( list[0].contains("build", Qt::CaseInsensitive) )
             list.removeFirst();

        if ( list[0].contains( command ) )
             list.removeFirst();

        if ( list[0].contains( "OK" ) )
        {
            if( list[0].contains( "Boot Required", Qt::CaseInsensitive )
                ||command.contains( "DHCP" ) )
               rebootBtn->setEnabled(true);

            showWarning(REBOOT_REQUIRED);
            return 1;
        }
        i = list.size()-1;

        while (i>0)
        {
           if(list[i].contains(">")||list[i].contains("OK"))
               list.removeAt(i);
           else break;
           i--;
        }

        for ( i=0; i<list.size(); i++ )
            rows.append( list[i].split( ": ", QString::SkipEmptyParts ) );

        if ( command.contains( "ifconfig" ) )
           netConfigTab->fillNetworkConfigForm( rows );
        else if ( command.contains( "set" ) )
            fillSettings( rows );
        else if ( command.contains( "line" ) )
        {
            if( devType!=INDEF_DEV )
                linesTab->fillLinesForm( rows );
        }
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
 * Show connected device info
 */
void MainWindow::getDeviceInfo( QString version )
{
    if ( version.contains(QRegExp("^02C1")) )
        devType = COOLINKHUB;
    else if ( version.contains(QRegExp("^05")) )
        devType = COOLMASTER;
    else if ( version.contains(QRegExp("^03D")) )
        devType = COOLPLUG;
    else if ( version.contains(QRegExp("^02C0")) )
        devType = COOLINK;
    else {
        devType = INDEF_DEV;
    }
    //deviceVersion->setText( version );
}

/*
 * Close serial port
 */
void MainWindow::closeSerialPort()
{
    if(serial->isOpen())
        serial->close();
}

/*
 * Open session
 */
bool MainWindow::openSession()
{
    if(devType==INDEF_DEV)
    {
        showWarning(INDEF_DEV_TYPE);
        setConnectButtonToStartPosition();
        closeSerialPort();
        isSessionBegin = false;
        isDisconnect = false;
        return false;
    }

    deleteWelcomePageElements();

    tabWidget = new QTabWidget;
    tabWidget->setObjectName("tabWidget");
    tabWidget->setAutoFillBackground(true);
    connect( tabWidget,SIGNAL(tabBarClicked(int)), this, SLOT(tabOperating(int)) );

    switch(devType)
    {

    case COOLPLUG:
        tabWidget->tabBar()->setProperty("singleType",true);
        setConsoleGUI();
        runConsoleView();
        break;
    default:
        setConfigsGUI();
        insertConfigGUIToPage();
        getPageInformation();
        setConsoleGUI();
        break;
    }
    this->setFixedSize(650,400);
    return true;
}

void MainWindow::openTerminal()
{
    consoleView = new ConsoleView(serial,echo,devType, isVCOM, this );
    consoleView->runConsole();
    consoleView->setConsoleSignalConnections(true);
    consoleView->runInTerminalMode();
    connectAction->setText("Disconnect");
    connect(this,SIGNAL(rebootEnd()),consoleView->getConsoleController(),SLOT(onRebootEnd()));
    connect(consoleView->getConsoleController(),SIGNAL(rebootSignal()),this,SLOT(findPortAfterReboot()));
    connect(consoleView->getConsoleController(),SIGNAL(rebootBeginSignal()),this,SLOT(rebootBegin()),Qt::DirectConnection);
    connect (this,SIGNAL(signal_DevDisconnToConsole()),consoleView->getConsoleController(),SLOT(deviceDisconnected()));
    connect (this, SIGNAL(signal_DevConnToConsole()),consoleView->getConsoleController(),SLOT(deviceConnected()));
    connect(consoleView, SIGNAL(terminalModeClose()),this,SLOT(closeTerminalMode()));

    isConnection = true;
    isConnectionProcessRun = false;
}

void MainWindow::onTerminalButtonClicked()
{
    warning->hide();

    setTerminalMode();
    openSerialPort();
}


void MainWindow::closeTerminalMode()
{
    disconnect(consoleView, SIGNAL(terminalModeClose()),this,SLOT(closeTerminalMode()));
    delete consoleView;
    setWindowOpacity(1.0);
    if(serial->isOpen())
        serial->close();
    isSessionBegin = false;
    isConnectionProcessRun=false;
    isDisconnect = false;
    connectBtn->setText("Connect");
    connectAction->setText("Connect");
    isTerminalModeSwitchOn = false;
    enumPorts();
}

void MainWindow::setTerminalMode()
{
    if(!isTerminalModeSwitchOn)
        isTerminalModeSwitchOn = true;
    else isTerminalModeSwitchOn = false;
}

void MainWindow::setConsoleGUI()
{
    consoleView = new ConsoleView(serial,echo,devType, isVCOM, this );
    tabWidget->addTab(consoleView,tr("Console"));
}

/*
 * Write data to port
 */
void MainWindow::sendData( const QString &command )
{
    qDebug()<<"send data...";
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
    while( serial->waitForReadyRead(100) )
       readData.append( serial->readAll() );

    if ( serial->error() == QSerialPort::ReadError )
    {
        showWarning(READING_ERROR);
        /*qDebug()<<(QString("Failed to read from port %1, error: %2")
                       .arg(serial->portName()).arg(serial->errorString() ));*/
    }
    else if ( serial->error()==QSerialPort::TimeoutError && readData.isEmpty() )
        showWarning(NO_DATA_IN_PORT);
        /*qDebug()<<( QString("No data was currently available for reading from port %1")
                           .arg(serial->portName()) );*/
    return QString( readData );
}

/*
 * Get information for current info page
 */
void MainWindow::getPageInformation()
{
    dataHandler ( pagesList.at(0) );
    dataHandler ( pagesList.at(1) );
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

            if( header.contains( "line type" ) )
                commandLine = header+val;
            else commandLine = QString( "%1 %2 %3" ).arg( "ifconfig" ).
                                                    arg( header ).arg( val );
            qDebug()<<"command to device: "<<commandLine;

            dataHandler( commandLine ) ;
            iterator.setValue( "" );
        }
    }
   applyButton->setEnabled(false);
}

/*
* Handling with apply button
*/
void MainWindow::applyButtonEnable(bool b)
{
    if (!isDisconnect)
        applyButton->setEnabled(b);
}

/*
 * Reboot processing
 */
void MainWindow::rebootBtnClicked()
{
    isReboot = true;
    headerWidget->handleLabelConnected(REBOOTING);
    headerWidget->updateButton->setEnabled(false);
    rebootBtn->setEnabled(false);
    rebootBtn->clearFocus();

    if ( !rebootProcessing() )
    {
        headerWidget->handleLabelConnected(DISCONNECTED);
        return;
    }
    headerWidget->updateButton->setEnabled(true);
    headerWidget->handleLabelConnected(CONNECTED );
}

bool MainWindow::rebootProcessing()
{
    if( !warning->isHidden() )
        warning->hide();
    rebootBegin();
    sendData("boot 2\r\n");
    serial->waitForBytesWritten(10)?qDebug()<<"data is written":qDebug()<<"timeout";
    serial->close();
    return findPortAfterReboot();
}

bool MainWindow::findPortAfterReboot()
{
    QString _portName (prevPortName);
    QTime timer;
    if ( !isTerminalModeSwitchOn )
        headerWidget->handleLabelConnected(REBOOTING);
    timer.start() ;
    while( timer.elapsed() < 5000 )
        qApp->processEvents(0);
    timer.restart() ;

    while(true)
    {
        enumPorts();
        for ( int i=0; i<portsList.count();i++ )
        {
            if(_portName.contains(portsList.at(i).portName()))
            {
                portName = _portName;
                openSerialPort();
                if (isTerminalModeSwitchOn)
                {
                    consoleView->setConsoleSignalConnections(true);
                    qDebug()<<"reboot end";
                    emit rebootEnd();
                }
                else
                {
                    qDebug()<<isConsoleSwitchOn;
                    if ( isConsoleSwitchOn )
                    {
                        consoleView->setConsoleSignalConnections(true);
                        headerWidget->handleLabelConnected(CONNECTED);
                        qDebug()<<"reboot end";
                        emit rebootEnd();
                    }
                }
                isReboot = false;
                return true;
            }
        }
        if( timer.elapsed() > 5000 )
        {
            isReboot = false;
            return false;
        }
    }
    return true;
}

void MainWindow::rebootBegin()
{
    isReboot = true;
}


/*
* Run console view
*/
void MainWindow::runConsoleView()
{
    if ( !consoleView->consoleIsRunning() )
                consoleView->runConsole();

    if ( consoleView->isHidden() )
    {
        consoleView->show();
    }
    headerWidget->updateButton->setEnabled(false);
    rebootBtn->hide();
    applyButton->hide();

    consoleView->setConsoleSignalConnections(true);
    connect(this,SIGNAL(rebootEnd()),consoleView->getConsoleController(),SLOT(onRebootEnd()));
    connect(consoleView->getConsoleController(),SIGNAL(rebootSignal()),this,SLOT(findPortAfterReboot()));
    connect(consoleView->getConsoleController(),SIGNAL(rebootBeginSignal()),this,SLOT(rebootBegin()),Qt::DirectConnection);
    connect (this,SIGNAL(signal_DevDisconnToConsole()),consoleView->getConsoleController(),SLOT(deviceDisconnected()));
    connect (this, SIGNAL(signal_DevConnToConsole()),consoleView->getConsoleController(),SLOT(deviceConnected()));
    isConsoleSwitchOn = true;

    this->setBaseSize(650,575);
    this->setMinimumSize(650,575);
    QRect rect = QApplication::desktop()->screenGeometry();
    this->setMaximumSize(rect.width(),rect.height());
}

void MainWindow::closeConsoleView()
{
    consoleView->setConsoleSignalConnections(false);
    disconnect(consoleView->getConsoleController(),SIGNAL(rebootSignal()),this,SLOT(findPortAfterReboot()));
    disconnect(consoleView->getConsoleController(),SIGNAL(rebootBeginSignal()),this,SLOT(rebootBegin()));
    disconnect (this,SIGNAL(signal_DevDisconnToConsole()),consoleView->getConsoleController(),SLOT(deviceDisconnected()));
    disconnect (this, SIGNAL(signal_DevConnToConsole()),consoleView->getConsoleController(),SLOT(deviceConnected()));
    disconnect(this,SIGNAL(rebootEnd()),consoleView->getConsoleController(),SLOT(onRebootEnd()));
    headerWidget->updateButton->setEnabled(true);
    applyButton->show();
    rebootBtn->show();
    rebootBtn->setEnabled(false);
    headerWidget->updateButton->setText("Refresh");
    isConsoleSwitchOn = false;
}

void MainWindow::closeSession()
{
    if (consoleView->consoleIsRunning())
            closeConsoleView();

    connectGroup->show();
    ui->mainLayout->removeItem(configPageLayout);
    delete tabWidget;
    delete headerWidget;
    warning->hide();
    labelDeviceVersion->hide();
    applyButton->hide();
    rebootBtn->hide();
    this->setFixedSize(*size );
    isSessionBegin = false;
    isConnectionProcessRun=false;
    isDisconnect = false;
    closeSerialPort();
    connectBtn->setText("Connect");
    connectBtn->setProperty("connectionStart", false);
    setConnectButtonIcon(tr(":/images/Arrow-right.png"),Qt::RightToLeft);
    connectAction->setText("Connect");
    connectBtn->show();
    connectBtn->clearFocus();
    enumPorts();
}

void MainWindow::about()
{
    QMessageBox::about( this, tr( "About CoolAutomation Tool" ),
                       tr( "The <b>CoolAutomation Tool</b> is a tool "
                          "for handling CoolAutomation devices." ) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

