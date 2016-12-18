#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>
#include <QtWidgets>
#include <QSemaphore>
#include "ipwidget.h"
#include "netconfigtab.h"
#include "linestab.h"
#include "warningwidget.h"
#include "headerwidget.h"
#include "globalvars.h"



QT_BEGIN_NAMESPACE

class QLabel;
class ConsoleController;
class ConsoleView;
class IPWidget;
class ClickableLabel;
class NetConfigTab;
class LinesTab;
class ConsoleDialog;
class InfoWidget;

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool initializeDeviceNotification(void);

private slots:

    void checkCustomPortName(QAction* action);
    void setCustomBaudRate(QAction* action);
    void openSysMsgTimeGate();
    void deviceConnected();
    void deviceDisconnected();
    void handleSerialPort();
    void sendData(const QString &command) ;
    int dataHandler(const QString &command);
    void about();
    bool getInitInformation();
    QString getData() ;
    void handleError(QSerialPort::SerialPortError error);
    void getPageInformation();
    void createCommandLine();
    void rebootBtnClicked();
    void tabOperating(int index);
    void closeConsoleView();
    void runConsoleView();
    void openTerminal();
    bool findPortAfterReboot();
    void closeSession();
    void connectButtonClicked();
    void rebootBegin();
    void logHotKeyPressed();
    void applyButtonEnable(bool);
    void setTerminalMode();
    void closeTerminalMode();
    void onTerminalButtonClicked();

protected:
   // bool eventFilter( QObject *target, QEvent *event );

signals:
    void processConnect() ;
    void nameSerialPortChanged(QString portName) ;
    void connectionSuccess();
    void signal_DeviceConnected();
    void signal_DevConnToConsole();
    void signal_DeviceDisconnected();
    void signal_DevDisconnToConsole();
    void releaseLoop();
    void rebootEnd();
    void sendDataNonVCom(QByteArray);

private:
    void showStatusMessage(const QString &message, int time);
    void initGUIConnections();
    void initGeneralGUIConnections();
    bool openSerialPort();
    void closeSerialPort();
    void selectPortName();
    bool isPortAvailable(const QString &portName);

    void getDeviceInfo( QString version );
    void setupModel();
    void changeDeviceProperty(QStringList list);
    void setupConnectMenu();
    void fillCommandBuffer();
    void fillSettings(QList<QStringList> rows );
    void setRatesMenu();
    void enumPorts();

    void timeGateHandler();
    bool openSession();
    bool checkDeviceInitialState();
    void switchesHandler(const QString row, int code);
    bool rebootProcessing();
    void handlingUnavailableState();

    void setTitle();
    void setConnectButton();
    void showWarning(int warningType);
    void setLabelConnectedPort();
    void setConnectButtonIcon(QString path,Qt::LayoutDirection direction);
    void setConfigsGUI();
    void connectBtnNonCom();
    void insertConfigGUIToPage();
    void deleteWelcomePageElements();
    void setCustomedFont();
    void setConsoleGUI();
    void getInfoAboutInstalledSerialPorts(QSerialPortInfo info);
    void setConnectButtonToStartPosition();
    bool nativeEvent(const QByteArray& eventType, void* message, long* result);

    Ui::MainWindow *ui;

    HDEVNOTIFY m_hDeviceNotify;

    QString portName;
    QString prevPortName;
    QString deviceName;
    qint32 baudRate;
    QString stringBaudRate;
    QSerialPort *serial;
    QLabel *status;
    QLabel* ports;
    QStringList commandsToDevice;
    QString currentCommand;
    QString strBuffer;
    QByteArray readData;
    QMap<QString,QString> commandBuffer;
    QMap<QString,QString> properties;
    QStringList devTypeStrings;

    QGridLayout* centralLayout;
    QGridLayout *mainLayout;
    QMenu* portsMenu;
    QMenu* ratesMenu;
    QAction* connectAction;
    QSize* size;
    QSettings* settings;
    QList<QSerialPortInfo> portsList;

    QStringList pagesList;
    QStringList warningStrings;
    QPixmap devicePict;
    QEventLoop loop;
    QTimer timer;
    int portsCount;
    bool isInit;
    bool isSessionBegin;
    bool sysMsgTimeGate;
    bool isSendingSuccess;
    bool isConnection;
    bool isConnectionProcessRun;
    bool isReboot;
    bool isDisconnect;
    bool isTerminalModeSwitchOn;
    bool isConsoleSwitchOn;
    int devType;
    int numbHVAC;
    int timeoutRead;
    int echo;
    bool _isVCOM;
    bool isVCOM;



    ConsoleView* consoleView;
    ConsoleController* consoleController;
    QPushButton *connectBtn;
    QFrame* connectGroup;
    WarningWidget* warning;
    QWidget* centralWindow;
    QLabel* labelConnectedPort;
    //
    QLabel* HVACPorts;
    QLabel* numberOfHVACPort;
    //
    QLabel* labelDeviceVersion;
    QLabel* autocompleteLabel;
    QPushButton* cleanButton ;
    QCheckBox* autocompleter ;
    QPushButton* applyButton;
    QPushButton* rebootBtn;

    QGroupBox* headerFrame;
    QHBoxLayout* connectBtnLayout;
    QHBoxLayout* footerLayout;
    QTabWidget* tabWidget;
    QGridLayout* configPageLayout;
    NetConfigTab* netConfigTab;
    LinesTab* linesTab;
    HeaderWidget* headerWidget;
    InfoWidget* infoWidget;






};

#endif // MAINWINDOW_H
