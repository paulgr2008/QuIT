#ifndef NETWORKCONFIG_H
#define NETWORKCONFIG_H

#include <QObject>

class NetworkConfig : public QObject
{
    Q_OBJECT
public:
    explicit NetworkConfig(QObject *parent = 0);
    ~NetworkConfig();

    void setMACAddress(const QString& m);
    void setIPAdress(const QString& i);
    void setLink(const QString& i);
    void setNetmask(const QString& i);
    void setGateway(const QString& i);
    void setDNS1(const QString& i);
    void setDNS2(const QString& i);
    void setAutoIP(const QString& i);

    QString getMACddress() const;
    QString getIPAdress()const;
    QString getLink() const;
    QString getNetmask() const;
    QString getGateway() const;
    QString getDNS1() const;
    QString getDNS2() const;
    QString getAutoIP() const;


signals:

public slots:
private:
    QString macAddress;
    QString ipAdress;
    QString link;
    QString netmask;
    QString gateway;
    QString dns1;
    QString dns2;
    QString autoip;
};

#endif // NETWORKCONFIG_H
