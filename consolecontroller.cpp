#include "consolecontroller.h"
#include "treemodelcompleter.h"
#include <QtWidgets>

ConsoleController::ConsoleController(QSerialPort* s, int e, QObject *parent)
    : QObject(parent),completer(0)
{
    serial = s;
    echo = e;
    console = new Console;
    completer = new TreeModelCompleter(this);
    completer->setSeparator(" ");
    completer->setModel(modelFromXMLFile(":/xmlfile.txt"));
    completer->setModelSorting( QCompleter::UnsortedModel );
    completer->setCaseSensitivity( Qt::CaseInsensitive );
    completer->setWrapAround(true);
    completer->setCompletionMode( QCompleter::PopupCompletion );

    console->setLocalEchoEnabled(e);
    console->setCompleter(completer);
    setConsoleSignalConnections();
}
void ConsoleController::writeData( const QByteArray &data )
{
    qDebug()<<"data to device: "<<data;
    serial->write(data);
}

Console* ConsoleController::getConsole() const
{
    return console;
}

void ConsoleController::readData()
{
    //if(serial->canReadLine())
    //{
        QByteArray data = serial->readAll();
        console->putData( data );
        qDebug()<<data;
    //}
}

void ConsoleController::setConsoleSignalConnections()
{
    connect( serial, SIGNAL(readyRead()), this, SLOT(readData()) );
    connect( console, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)) );
}

void ConsoleController::closeConsoleSignalConnection()
{
    disconnect( serial, SIGNAL(readyRead()), this, SLOT(readData()) );
    disconnect( console, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)) );
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
void ConsoleController::setAutocomplete( int state )
{
    if( state == Qt::Checked)
        connect( console,SIGNAL(cursorPositionChanged()),console,SLOT(completerHandler()) );
    else disconnect( console,SIGNAL(cursorPositionChanged()),console,SLOT(completerHandler()) );

}

