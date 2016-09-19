#ifndef CONSOLETAB_H
#define CONSOLETAB_H

#include <QObject>
#include <QtWidgets>
#include <QSerialPort>
#include <consolecontroller.h>

class ConsoleTab : public QWidget
{
    Q_OBJECT
public:
    explicit ConsoleTab(QSerialPort* serial, int echo, QWidget *parent = 0);

    void setConsoleSignalConnections(bool f);
    ConsoleController* getConsoleController() const;
    void runConsole();
signals:

private:
    QSerialPort* serial;
    ConsoleController* cController;
    int echo;

};

#endif // CONSOLETAB_H
