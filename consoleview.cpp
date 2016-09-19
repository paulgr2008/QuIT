#include "consoleview.h"

ConsoleView::ConsoleView(QSerialPort* s, int e, QWidget *parent) :
    QWidget(parent)
{
    serial = s;
    echo = e;
}

void ConsoleView::runConsole()
{
    cController = new ConsoleController(serial, echo);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(cController->getConsole());
    setLayout(mainLayout);
}

ConsoleController* ConsoleView::getConsoleController() const
{
    return cController;
}

void ConsoleView::setConsoleSignalConnections(bool f)
{
    if ( f )
        cController->setConsoleSignalConnections();
    else cController->closeConsoleSignalConnection();
}
