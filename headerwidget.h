#ifndef HEADERWIDGET_H
#define HEADERWIDGET_H

#include <QtWidgets>
#define CONNECTED 0
#define DISCONNECTED 1
#define REBOOTING 2

QT_BEGIN_NAMESPACE

class QLabel;
class QGroupBox;

QT_END_NAMESPACE

class HeaderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HeaderWidget(QWidget *parent = 0);
    ~HeaderWidget();
    void handleLabelConnected(int type);
    QPushButton* updateButton;
    QLabel* deviceNameLabel;
    QLabel* labelConnected;
    QHBoxLayout* headerLayout;
    QGroupBox* headerFrame;

signals:

public slots:
private:
    QStringList typesList;
};

#endif // HEADERWIDGET_H
