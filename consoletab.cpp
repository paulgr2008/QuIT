#include "consoletab.h"
#include "treemodelcompleter.h"
#include <QtWidgets>

ConsoleTab::ConsoleTab( QSerialPort* s, int e, QWidget *parent )
    : QWidget(parent)
{
    serial = s;
    echo = e;
}

void ConsoleTab::runConsole()
{
    cController = new ConsoleController(serial, echo);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(cController->getConsole());
    setLayout(mainLayout);
}

ConsoleController* ConsoleTab::getConsoleController() const
{
    return cController;
}

void ConsoleTab::setConsoleSignalConnections(bool f)
{
    if ( f )
        cController->setConsoleSignalConnections();
    else cController->closeConsoleSignalConnection();
}


