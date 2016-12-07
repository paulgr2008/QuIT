#include "logbrowserdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextBrowser>
#include <QPushButton>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QShortcut>
#include <QDebug>

LogBrowserDialog::LogBrowserDialog(QWidget* w, QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("Trace");

    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    browser = new QTextBrowser(this);
    layout->addWidget(browser);
    setWindowFlags(Qt::Tool);
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    layout->addLayout(buttonLayout);

    buttonLayout->addStretch(10);

    clearButton = new QPushButton(this);
    clearButton->setText("Clear");
    clearButton->setProperty("configButton", true);
    buttonLayout->addWidget(clearButton);
    connect(clearButton, SIGNAL (clicked()), browser, SLOT (clear()));

    saveButton = new QPushButton(this);
    saveButton->setText("Save Trace");
    saveButton->setProperty("configButton", true);
    buttonLayout->addWidget(saveButton);
    connect(saveButton, SIGNAL (clicked()), this, SLOT (save()));

    QPoint p = w->mapToGlobal(w->geometry().topLeft());
    this->setStyleSheet(w->styleSheet());
    setGeometry(p.x()-406,p.y()+5,400,450);


}
void LogBrowserDialog::keyCtrlDPressed()
{
    if (isHidden())
            show();
    else hide();
}

void LogBrowserDialog::outputMessage(QtMsgType type, const QString &msg)
{
    switch (type)
    {
    case QtDebugMsg:
        browser->append(msg);
        break;

    case QtWarningMsg:
        browser->append(tr("— WARNING: %1").arg(msg));
        break;

    case QtCriticalMsg:
        browser->append(tr("— CRITICAL: %1").arg(msg));
        break;

    case QtFatalMsg:
        browser->append(tr("— FATAL: %1").arg(msg));
    break;
    default:break;
    }
}


void LogBrowserDialog::save()
{
    QString saveFileName = QFileDialog::getSaveFileName(this,tr("Save Log Output"),
                            tr("%1/logfile.txt").arg(QDir::homePath()),
                            tr("Text Files ('''.txt);;All Files (*)") );

    if(saveFileName.isEmpty())
        return;

    QFile file(saveFileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning( this, tr("Error"),
            QString(tr("<nobr>File '%1'<br/>cannot be opened for writing.<br/><br/>"
                    "The log output could <b>not</b> be saved!</nobr>")).arg(saveFileName));
    return;
    }

    QTextStream stream(&file);
    stream << browser->toPlainText();
    file.close();
}

LogBrowserDialog::~LogBrowserDialog()
{

}
