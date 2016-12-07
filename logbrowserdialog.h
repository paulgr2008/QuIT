#ifndef LOGBROWSERDIALOG_H
#define LOGBROWSERDIALOG_H

#include <QDialog>

namespace Ui {
class LogBrowserDialog;
}

class QTextBrowser;
class QPushButton;

class LogBrowserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogBrowserDialog(QWidget *w, QWidget *parent = 0);
    ~LogBrowserDialog();

public slots:
    void outputMessage( QtMsgType type, const QString &msg );

protected slots:
    void save();
    void keyCtrlDPressed();

protected:
    //virtual void keyPressEvent( QKeyEvent *e );
    //virtual void closeEvent( QCloseEvent *e );

    QTextBrowser *browser;
    QPushButton *clearButton;
    QPushButton *saveButton;


};

#endif // LOGBROWSERDIALOG_H
