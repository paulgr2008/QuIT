#ifndef NETCONFIGTAB_H
#define NETCONFIGTAB_H

#include <QtWidgets>
#include "ipwidget.h"

QT_BEGIN_NAMESPACE
class IPWidget;

QT_END_NAMESPACE

class NetConfigTab:public QWidget
{
    Q_OBJECT

public:
    explicit NetConfigTab(QMap<QString,QString> *commandBuffer, QWidget *parent = 0);

    void fillNetworkConfigForm(QList<QStringList> rows);
    void toCommandBuffer();
signals:
    void sendStringToBuffer(QString,QString);
    void bufferFilled(bool);

public slots:
    void handleNetconfigPage( QString str );
    void IPSettingsHandler(int);

private:
    QList<IPWidget*> ipWidgetsList;
    QStandardItemModel *modelIP;
    QDataWidgetMapper *mapperIP;
    QString dotLabelStySheet;
    QMap<QString, IPWidget*> ipWidgetMap;
    QMap<QString,QString> *commandBuffer;
    void ipLineEditProperties(QLineEdit* le);
    void setIPWidgets();
    void setIPConfigurationFormsEnabled (bool b);

    /* GUI elements*/
    QLabel* ip;
    QLabel* dns1;
    QLabel* dns2;
    QLabel* netmask;
    QLabel* gateway;

    QLabel* dns1point1;
    QLabel* macValue;
    QLabel* macName;
    QLabel* dhcpLabel;
    QLabel* nameDNSGroup;
    QComboBox* dhcpSelector;
    QHBoxLayout* layoutDNS1WidgetEl ;
    QHBoxLayout* layoutDNS2WidgetEl ;
    QHBoxLayout* layoutIPWidgetEl ;
    QHBoxLayout* layoutNetmaskWidgetEl ;
    QHBoxLayout* layoutGatewayWidgetEl ;
};

#endif // NETCONFIGTAB_H
