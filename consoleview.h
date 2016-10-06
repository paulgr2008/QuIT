#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <QObject>
#include <QWidget>
#include <QSerialPort>
#include <consolecontroller.h>

class ConsoleView : public QWidget
{
    Q_OBJECT
public:
    explicit ConsoleView(QSerialPort* serial, int echo, QWidget *parent = 0);

    void setConsoleSignalConnections(bool f);
    ConsoleController* getConsoleController() const;
    void runConsole();
    bool consoleIsRunning();
signals:

public slots:

private:
    QSerialPort* serial;
    ConsoleController* cController;
    int echo;
    bool isRunning;
};

#endif // CONSOLEVIEW_H
