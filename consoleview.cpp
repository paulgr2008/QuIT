#include "consoleview.h"
bool isConsoleDialogRunning;
bool isTerminalModeRunning;
ConsoleView::ConsoleView(QSerialPort* s, int e, int _devType, bool _isVCOM, QWidget *_parent) :
    QWidget(_parent)
{
    parent = _parent;
    serial = s;
    echo = e;
    isRunning = false;
    devType = _devType;
    isConsoleDialogRunning = false;
    isTerminalModeRunning = false;
    isVCOM = _isVCOM;
}

void ConsoleView::runConsole()
{
    cController = new ConsoleController(serial, echo, devType, isVCOM);
    mainLayout = new QVBoxLayout;

    footer = new ConsoleFooter;
    mainLayout->addWidget(cController->getConsole());
    mainLayout->addWidget(footer);

    setLayout(mainLayout);
    isRunning = true;

    connect( footer->autocompleter,SIGNAL( stateChanged(int) ),
                    getConsoleController(), SLOT( setAutocomplete(int)) );
    connect(footer->fullScreenButton,SIGNAL(clicked(bool)),this,SLOT(consoleFullScreen()));
    connect(footer->cleanButton,SIGNAL(clicked()),this,SLOT(clearConsole()));
    cController->getConsole()->setFocus();
}

void ConsoleView::addConsole(Console* console)
{
    mainLayout->insertWidget(0,console);
}

void ConsoleView::clearConsole()
{
    cController->cleanConsole();
}

void ConsoleView::consoleFullScreen()
{
    if( !isConsoleDialogRunning )
    {
        consoleDlg = new ConsoleDialog(getConsoleController()->getConsole(),footer, parent);
        parent->setWindowOpacity(0.0);
        footer->fullScreenButton->setText("Tab mode");
        connect(consoleDlg,SIGNAL(closeDialog()),this,SLOT(manageFullScreenClose()));
        isConsoleDialogRunning = true;
    }
    else
        manageFullScreenClose();

}

void ConsoleView::runInTerminalMode()
{
    if( !isTerminalModeRunning )
    {
        consoleDlg = new ConsoleDialog(getConsoleController()->getConsole(),footer, parent);
        parent->setWindowOpacity(0.0);
        footer->fullScreenButton->setText("Disconnect");
        connect(consoleDlg,SIGNAL(closeDialog()),this,SLOT(manageFullScreenClose()));
        isTerminalModeRunning = true;
        isConsoleDialogRunning = true;
    }
}

void ConsoleView::manageFullScreenClose()
{

    if( !isTerminalModeRunning )
    {
        consoleDlg->close();
        addConsole( getConsoleController()->getConsole());
        footer->fullScreenButton->setText("Terminal mode");
        mainLayout->addWidget(footer);
        isConsoleDialogRunning = false;
        parent->setWindowOpacity(1.0);
    }
    else
    {
        delete consoleDlg;
        delete cController;
        isConsoleDialogRunning = false;
        emit terminalModeClose();
    }

}

void ConsoleView::manageAutocompleter()
{
    if (footer->autocompleter->isChecked())
        footer->autocompleter->setChecked(false);
    else footer->autocompleter->setChecked(true);
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
