#include "logbrowser.h"

#include <QMetaType>

#include "logbrowserdialog.h"

LogBrowser::LogBrowser(QWidget* w, QObject *parent) :
 QObject(parent)
{
    qRegisterMetaType<QtMsgType>("QtMsgType");
    browserDialog = new LogBrowserDialog(w);
    connect(this, SIGNAL (sendMessage(QtMsgType,QString)), browserDialog, SLOT (outputMessage(QtMsgType,QString)), Qt::QueuedConnection);
    //browserDialog->show();
}

LogBrowser::~LogBrowser()
{
 delete browserDialog;
}

void LogBrowser::outputMessage(QtMsgType type, const QString &msg)
{
 emit sendMessage( type, msg );
}

void LogBrowser::hideBrowser()
{
    if (browserDialog->isHidden())
        browserDialog->show();
    else browserDialog->hide();
}



