#ifndef CONSOLETAB_H
#define CONSOLETAB_H

#include <QObject>
#include <QtWidgets>
#include <QSerialPort>
#include <QModelIndex>
#include <console.h>
#include <consolecontroller.h>


class TreeModelCompleter;
QT_BEGIN_NAMESPACE
//class QTreeView;
class QAbstractItemModel;

QT_END_NAMESPACE

class ConsoleTab : public QWidget
{
    Q_OBJECT
public:
    explicit ConsoleTab(QSerialPort* serial, int echo, QWidget *parent = 0);

    void setConsoleSignalConnections(bool f);
    ConsoleController* getConsoleController() const;
    void runConsole();
signals:

public slots:
    //void writeData(const QByteArray &data);
    //void readData();
    //void setAutocomplete(int);

private:
    //QAbstractItemModel *modelFromFile(const QString& fileName);
    QSerialPort* serial;
    //Console* console;
    //TreeModelCompleter* completer;
    ConsoleController* cController;
    int echo;

};

#endif // CONSOLETAB_H
