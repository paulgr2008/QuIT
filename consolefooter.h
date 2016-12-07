#ifndef CONSOLEFOOTER_H
#define CONSOLEFOOTER_H

#include <QtWidgets>

namespace Ui {
class ConsoleFooter;
}

class ConsoleFooter : public QWidget
{
    Q_OBJECT

public:
    explicit ConsoleFooter(QWidget *parent = 0);
    ~ConsoleFooter();
    QPushButton* cleanButton;
    QPushButton* fullScreenButton;
    QCheckBox* autocompleter;

private:
    Ui::ConsoleFooter *ui;

};

#endif // CONSOLEFOOTER_H
