#include "consoledialog.h"

ConsoleDialog::ConsoleDialog(Console* _console, ConsoleFooter* _footer, QWidget *parent) : QDialog(parent)
{
    console = _console;
    footer = _footer;
    setSizeGripEnabled(true);
    setWindowModality(Qt::WindowModal);
    setWindowTitle("Quick Installation Tool: Terminal Mode");
    setGeometry(parent->geometry());
    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    //setWindowFlags(Qt::SplashScreen);
    resize(650,600);
    setModal(true);
    layout = new QVBoxLayout;
    layout->addWidget(console);
    layout->addWidget(footer);
    setLayout(layout);
    connect(console, SIGNAL(consoleClose()),this,SLOT(close()));
    show();
}


void ConsoleDialog::closeEvent(QCloseEvent *event)
{
    layout->removeWidget(console);
    event->accept();
    emit closeDialog();
}





