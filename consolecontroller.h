#ifndef CONSOLECONTROLLER_H
#define CONSOLECONTROLLER_H

#include <QObject>
#include <QtWidgets>
#include <QSerialPort>
#include <QModelIndex>
#include <console.h>

class TreeModelCompleter;
QT_BEGIN_NAMESPACE
//class QTreeView;
class QAbstractItemModel;

QT_END_NAMESPACE



class ConsoleController : public QObject
{
        Q_OBJECT
    public:
        explicit ConsoleController(QSerialPort* serial, int echo,int _devType, bool _isVCOM, QObject *parent = 0);
        void setConsoleSignalConnections();
        void closeConsoleSignalConnection();
        void disconnectReadWriteConnection();
        bool isRebootProc();
        Console* getConsole() const;
    signals:
        void rebootSignal();
        void rebootBeginSignal();

    public slots:
        void writeData(const QByteArray &data,bool r);
        void readData();
        void setAutocomplete(int);
        void cleanConsole();
        void onRebootEnd();
        void deviceConnected();
        void deviceDisconnected();

    private:
        QAbstractItemModel *modelFromXMLFile(const QString& fileName);
        QSerialPort* serial;
        Console* console;
        TreeModelCompleter* completer;
        bool isReboot;
        bool isVCOM;
        int echo;
};

#endif // CONSOLECONTROLLER_H
