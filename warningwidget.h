#ifndef WARNINGWIDGET_H
#define WARNINGWIDGET_H

#include <QtWidgets>

namespace Ui {
class WarningWidget;
}

class WarningWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WarningWidget(QWidget *parent = 0);
    void setWarningText(QString strWarning);

    QPushButton* warningButton;
    QPushButton* quitButton;
    ~WarningWidget();

signals:
    void buttonClicked();
protected:
    bool eventFilter(QObject *, QEvent *);
private:
    Ui::WarningWidget *ui;
    QLabel* warningText;

    QSignalMapper *signalMapper;
    QGroupBox* warningBox;

};

#endif // WARNINGWIDGET_H
