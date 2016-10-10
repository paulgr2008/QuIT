#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>
#include <QtWidgets>
#include <QSemaphore>
#include "ipwidget.h"

#define N_LINES    8

/* switches */
#define SW_DK     10
#define SW_ME      0
#define SW_TO      5
#define SW_SA      5
#define SW_UNUSED  0

/* CA types */

const int nHVACLines[]={8,5,5,3};
enum DevType {COOLMASTER, COOLINK,COOLINKHUB,COOLPLUG,COOLPLUGADMIN} ;


QT_BEGIN_NAMESPACE

class QLabel;
class ConsoleController;
class ConsoleView;
class IPWidget;
class ClickableLabel;

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
    void tabOperating(int index);
    void checkCustomPortName(QAction* action);
    void autoipCheckBoxState(int state);
    void handleSerialPort();
    void handleError(QSerialPort::SerialPortError error);
    void sendData(const QString &command) ;
    QString getData() ;
    int dataHandler(const QString &command);

    void about();
    void createCommandLine();
    void setCustomBaudRate(QAction* action);
    void handleNetconfigPage(QString);
    void IPSettingsHandler(bool);
    void deviceConnected();
    void deviceDisconnected();
    void openSysMsgTimeGate();
    void handleLines(QString str);
    bool getInitInformation();
    void on_rebootBtn_clicked();

    void on_consoleButton_clicked();
protected:
 bool eventFilter( QObject *target, QEvent *event );

signals:
     void processConnect() ;
     void nameSerialPortChanged(QString portName) ;
     void connectionSuccess();
     void signal_DeviceConnected();
     void signal_DeviceDisconnected();
     void releaseLoop();

private:
    void showStatusMessage(const QString &message, int time);
    void initGUIConnections();
    void openSerialPort();
    void closeSerialPort();
    void selectPortName();

    void getDeviceInfo( QString version );
    void setupModel();
    void changeDeviceProperty(QStringList list);
    void setupConnectMenu();
    void fillCommandBuffer();
    void fillLinesForm(QList<QStringList> rows );
    void fillSettings(QList<QStringList> rows );
    void fillNetworkConfigForm(QList<QStringList> rows );
    void setRatesMenu();
    void setPortsMenu();
    void closeSession();
    void timeGateHandler();
    void setupLinesView();
    void setLinesInitialState();
    void setLinesViewStyle();
    void defineStyles();
    void openSession();
    void manageGlobalElements();
    void comboBoxFilter();
    void sortItems();
    void createDipSwitches();
    void resetSwitches();
    void switchOn(const QString row, int column, bool bit);
    void setTitle();
    void setConnectButton();
    int execWarningWindow(QString warning);
    bool checkDeviceInitialState();
    void switchesHandler(const QString row, int code);
    bool rebootProcessing();
    void handlingLostConnection(QString w);

    bool nativeEvent(const QByteArray& eventType, void* message, long* result);

    Ui::MainWindow *ui;

    HDEVNOTIFY m_hDeviceNotify;

    QString portName;
    QString deviceName;
    qint32 baudRate;
    QString stringBaudRate;
    QSerialPort *serial;
    QLabel *status;
    QLabel* ports;
    QSignalMapper* signalMapper;
    QSignalMapper* netconfigMapper;
    QStringList commandsToDevice;
    QString currentCommand;
    QString strBuffer;
    QByteArray readData;
    QMap<QString,QString> commandBuffer;
    QMap<QString,QString> properties;
    QMap<QString, IPWidget*> ipWidgetMap;



    QStringList devTypeStrings; 

    QRegExp rx;
    QRegExpValidator *validator;
    QList<QStandardItem*> itemsList;
    QStandardItemModel *model;
    QStandardItemModel* linesModel;

    QList<IPWidget*> ipWidgetsList;
    QStandardItemModel *modelIP;
    QDataWidgetMapper *mapperIP;

    QGridLayout* centralLayout;
    QGridLayout *mainLayout;
    QDataWidgetMapper *mapper;
    QMenu* portsMenu;
    QMenu* ratesMenu;
    QAction* connectAction;
    QAction* resetConfiguration;
    QAction* getHVACInfo;
    QSize* size;
    QSettings* settings;
    QList<QSerialPortInfo> portsList;
    QList<QComboBox*> lineCombosList;
    QList<QLabel*> lineLabelList;
    QList<QGroupBox*> lineFrameBoxesList;
    QList<QLabel*> switchesList;
    QPixmap devicePict;
    QMessageBox* msgBox;
    QEventLoop loop;
    QTimer timer;
    int portsCount;
    bool isInit;
    bool isSessionBegin;
    bool sysMsgTimeGate;
    bool isSendingSuccess;
    int devType;
    int numbHVAC;
    int timeoutRead;
    int echo;

    ConsoleView* consoleView;
    ConsoleController* consoleController;
    QPushButton *connectBtn;
    QWidget* centralWindow;
    QLabel* labelConnectedPort;
    //IPWidget* ipWidget;

    /* styles strings*/
    QString styleLineFrameUsed;
    QString styleLineFrameDisabled;
    QString switchOnStyle;
    QString switchOffStyle;
    QString switchBeginStyle;
    QString dotLabelStySheet;
    QString connectButtonStyle;
    QString connectButtonStyleHover;
    QString connectButtonStylePressed;
    QString connectedLabel;






};

#endif // MAINWINDOW_H
