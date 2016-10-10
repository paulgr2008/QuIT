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
        explicit ConsoleController(QSerialPort* serial, int echo,QObject *parent = 0);
        void setConsoleSignalConnections();
        void closeConsoleSignalConnection();
        Console* getConsole() const;
    signals:

    public slots:
        void writeData(const QByteArray &data);
        void readData();
        void setAutocomplete(int);
        void cleanConsole();

    private:
        QAbstractItemModel *modelFromXMLFile(const QString& fileName);
        QSerialPort* serial;
        Console* console;
        TreeModelCompleter* completer;
        int echo;
};

#endif // CONSOLECONTROLLER_H
