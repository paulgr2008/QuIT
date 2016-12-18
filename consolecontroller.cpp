#include "consolecontroller.h"
#include "treemodelcompleter.h"
#include "globalvars.h"

ConsoleController::ConsoleController(QSerialPort* s, int e, int _devType, bool _isVCOM, QObject *parent)
    : QObject(parent),completer(0)
{
    serial = s;
    echo = e;
    isReboot = false;
    isVCOM = _isVCOM;
    console = new Console;
    console->setObjectName("Console");

    completer = new TreeModelCompleter(this);
    completer->setSeparator(" ");
    if (_devType==COOLPLUG )
        completer->setModel(modelFromXMLFile(":/xmlfile_cp.txt"));
    else completer->setModel(modelFromXMLFile(":/xmlfile.txt"));
    completer->setModelSorting( QCompleter::UnsortedModel );
    completer->setCaseSensitivity( Qt::CaseInsensitive );
    completer->setWrapAround(true);
    completer->setCompletionMode( QCompleter::PopupCompletion );
    setAutocomplete(Qt::Checked);
    console->setLocalEchoEnabled(e);
    console->setCompleter(completer);
}

bool ConsoleController::isRebootProc()
{
    return isReboot;
}

void ConsoleController::deviceConnected()
{
    if (!isReboot)
    {
        onRebootEnd();
    }
    //console->setReadOnly(false);
}

void ConsoleController::deviceDisconnected()
{
    if (!isReboot)
    {
        console->insertWarningText("Disconnected");
        //console->setReadOnly(true);
    }
}

void ConsoleController::onRebootEnd()
{
    isReboot = false;
    //console->setReadOnly(false);
    console->insertWarningText("");

}


void ConsoleController::writeData( const QByteArray &data,bool r )
{
    serial->write(data);
    qDebug()<<"write data";
    isReboot = r;

    if( isReboot )
    {
        console->insertWarningText("Rebooting");

        if(!isVCOM)
        {
            return;
        }

        //console->setReadOnly(true);
        emit rebootBeginSignal();
        serial->waitForBytesWritten(10)?qDebug()<<"data is written":qDebug()<<"timeout";


        serial->close();
        closeConsoleSignalConnection();

        qDebug()<<"emit reboot signal";

        emit rebootSignal();
    }
}

Console* ConsoleController::getConsole() const
{
    return console;
}

void ConsoleController::readData()
{

    QByteArray data = serial->readAll();
    console->putData( data );
}



void ConsoleController::setConsoleSignalConnections()
{
    connect( serial, SIGNAL(readyRead()),this, SLOT(readData()) );
    connect( console, SIGNAL(getDataFromConsole(QByteArray,bool)),this, SLOT(writeData(QByteArray,bool)) );
}

void ConsoleController::closeConsoleSignalConnection()
{
    disconnect( serial, SIGNAL(readyRead()),this, SLOT(readData()) );
    disconnect( console, SIGNAL(getDataFromConsole(QByteArray,bool)),this, SLOT(writeData(QByteArray,bool)) );
}

QAbstractItemModel* ConsoleController::modelFromXMLFile(const QString& fileName)
{
    QFile file (fileName);
    QXmlStreamReader m_streamReader;
    QStandardItem* m_currentItem;
    QStandardItemModel *model = new QStandardItemModel(completer);

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Can't open file: " << fileName;
        abort();
    }
    m_streamReader.setDevice(&file);
    m_currentItem = model->invisibleRootItem();

    while (!m_streamReader.atEnd())
    {
        QXmlStreamReader::TokenType tt = m_streamReader.readNext();
        switch (tt)
        {
        case QXmlStreamReader::StartElement:
        {
            QString name = m_streamReader.name().toString();

            if(m_streamReader.name()=="root")
                break;

            if (m_streamReader.name()=="item")
                name = m_streamReader.attributes().value("att").toString();

            QStandardItem* item = new QStandardItem(name);
            m_currentItem->appendRow(item);
            m_currentItem = item;
            break;
        }
        case QXmlStreamReader::EndElement:
            if (!m_currentItem->parent())
                m_currentItem = model->invisibleRootItem();
            else m_currentItem = m_currentItem->parent();
                break;
        case QXmlStreamReader::EndDocument:
            default:
                break;
            }
        }
    return model;
}

void ConsoleController::cleanConsole()
{
    console->clear();
    console->insertPrompt(false);
}

void ConsoleController::setAutocomplete( int state )
{
    if( state == Qt::Checked)
        connect( console,SIGNAL(cursorPositionChanged()),console,SLOT(completerHandler()) );
    else disconnect( console,SIGNAL(cursorPositionChanged()),console,SLOT(completerHandler()) );

}

