#ifndef CONSOLEDIALOG_H
#define CONSOLEDIALOG_H

#include <QtWidgets>
#include "console.h"
#include "consolefooter.h"

class ConsoleDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConsoleDialog(Console* _console, ConsoleFooter* _footer, QWidget *parent );

signals:
    void closeDialog();

private slots:

private:
    void closeEvent(QCloseEvent *event);
    QVBoxLayout* layout;
    Console* console;
    ConsoleFooter* footer;

};

#endif // CONSOLEDIALOG_H
