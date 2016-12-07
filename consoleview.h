#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <QObject>
#include <QWidget>
#include <QSerialPort>
#include <consolecontroller.h>
#include "consoledialog.h"
#include "consolefooter.h"

class ConsoleView : public QWidget
{
    Q_OBJECT
public:
    explicit ConsoleView(QSerialPort* serial, int echo, int _devType, bool _isVCOM, QWidget *parent);

    void setConsoleSignalConnections(bool f);
    ConsoleController* getConsoleController() const;
    void runConsole();
    bool consoleIsRunning();
    void setConsoleRunning(bool b)
    {
        isRunning = b;
    }
    void addConsole(Console* console);
    void runInTerminalMode();

signals:
    void terminalModeClose();

public slots:
    void manageFullScreenClose();
    void consoleFullScreen();
    void clearConsole();
    void manageAutocompleter();

private:



    QSerialPort* serial;
    ConsoleController* cController;
    QVBoxLayout *mainLayout;

    Console* console;
    ConsoleDialog* consoleDlg;
    QWidget *parent;
    ConsoleFooter* footer;

    int echo;
    int devType;
    bool isRunning;
    bool isVCOM;
};

#endif // CONSOLEVIEW_H
