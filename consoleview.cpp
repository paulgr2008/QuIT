#include "consoleview.h"

ConsoleView::ConsoleView(QSerialPort* s, int e, QWidget *parent) :
    QWidget(parent)
{
    serial = s;
    echo = e;
    isRunning = false;
}

void ConsoleView::runConsole()
{
    cController = new ConsoleController(serial, echo);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(cController->getConsole());
    setLayout(mainLayout);
    isRunning = true;
}

ConsoleController* ConsoleView::getConsoleController() const
{
    return cController;
}

bool ConsoleView::consoleIsRunning()
{
    return isRunning;
}


void ConsoleView::setConsoleSignalConnections(bool f)
{
    if ( f )
        cController->setConsoleSignalConnections();
    else cController->closeConsoleSignalConnection();
}
