#include "networkconfig.h"

NetworkConfig::NetworkConfig(QObject *parent)
    : QObject(parent)

{

}
/* setters */
void NetworkConfig::setMACAddress(const QString& m)
{
    macAddress = m;
}
void NetworkConfig::setIPAdress(const QString& i)
{
    ipAdress=i;
}
void NetworkConfig::setLink(const QString& i)
{
    link=i;
}
void NetworkConfig::setNetmask(const QString& i)
{
    netmask=i;
}
void NetworkConfig::setGateway(const QString& i)
{
    gateway=i;
}
void NetworkConfig::setDNS1(const QString& i)
{
    dns1=i;
}
void NetworkConfig::setDNS2(const QString& i)
{
    dns2=i;
}
void NetworkConfig::setAutoIP(const QString& i)
{
    autoip=i;
}
/* getters */
QString NetworkConfig::getMACddress() const
{
    return macAddress;
}
QString NetworkConfig::getIPAdress()const
{
    return ipAdress;
}
QString NetworkConfig::getLink() const
{
    return link;
}
QString NetworkConfig::getNetmask() const
{
    return netmask;
}
QString NetworkConfig::getGateway() const
{
    return gateway;
}
QString NetworkConfig::getDNS1() const
{
    return dns1;
}
QString NetworkConfig::getDNS2() const
{
    return dns2;
}
QString NetworkConfig::getAutoIP() const
{
    return autoip;
}







NetworkConfig::~NetworkConfig(){}
