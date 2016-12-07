#include "netconfigtab.h"

NetConfigTab::NetConfigTab( QMap<QString,QString> *_commandBuffer,QWidget *parent)
    : QWidget(parent)
{
    commandBuffer = _commandBuffer;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QGroupBox* netconfigFrame = new QGroupBox;
    netconfigFrame->setAutoFillBackground(true);
    netconfigFrame->setObjectName("netconfigFrame");

    QHBoxLayout *mainLayoutWithColumns = new QHBoxLayout;
    setIPWidgets();
    QHBoxLayout* layoutIP = new QHBoxLayout;
    layoutIP->setMargin(0);

    QHBoxLayout* layoutNetmask = new QHBoxLayout;
    layoutNetmask->setMargin(0);

    QHBoxLayout* layoutGateway = new QHBoxLayout;
    QHBoxLayout* layoutDNS1 = new QHBoxLayout;
    QHBoxLayout* layoutDNS2 = new QHBoxLayout;
    QHBoxLayout* layoutMAC = new QHBoxLayout;
    QHBoxLayout* layoutDHCP = new QHBoxLayout;

    layoutMAC->addStretch();
    layoutMAC->addWidget(macName);
    layoutMAC->addWidget(macValue);
    layoutMAC->addSpacing(20);

    layoutDHCP->addStretch();
    layoutDHCP->addWidget(dhcpLabel);
    layoutDHCP->addWidget(dhcpSelector);
    layoutDHCP->addSpacing(20);


    layoutIP->addStretch();
    layoutIP->addWidget(ip);
    layoutIP->addWidget(ipWidgetMap.value(tr("IP")));
    layoutIP->addSpacing(20);

    layoutNetmask->addStretch();
    layoutNetmask->addWidget(netmask);
    layoutNetmask->addWidget(ipWidgetMap.value(tr("Netmask")));
    layoutNetmask->addSpacing(20);

    layoutGateway->addStretch();
    layoutGateway->addWidget(gateway);
    layoutGateway->addWidget(ipWidgetMap.value(tr("Gateway")));
    layoutGateway->addSpacing(20);

    layoutDNS1->addStretch();
    layoutDNS1->addWidget(dns1);
    layoutDNS1->addWidget(ipWidgetMap.value(tr("DNS1")));
    layoutDNS1->addSpacing(20);

    layoutDNS2->addStretch();
    layoutDNS2->addWidget(dns2);
    layoutDNS2->addWidget(ipWidgetMap.value(tr("DNS2")));
    layoutDNS2->addSpacing(20);

    QVBoxLayout* column1 = new QVBoxLayout;
    column1->addStretch();
    column1->addLayout(layoutDHCP);
    column1->addLayout(layoutIP);
    column1->addLayout(layoutNetmask);
    column1->addLayout(layoutGateway);
    column1->addStretch();

    QVBoxLayout* column2 = new QVBoxLayout;

    QWidget *dnsSeparator = new QWidget;
    dnsSeparator->setFixedHeight(1);
    dnsSeparator->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    dnsSeparator->setStyleSheet(QString("background-color: #b2bfc3;"));
    nameDNSGroup = new QLabel;
    nameDNSGroup->setText("DNS Servers");
    nameDNSGroup->setProperty("networkLabels",true);
    nameDNSGroup->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    nameDNSGroup->setFixedHeight(16);
    column2->addStretch();
    column2->addLayout(layoutMAC);
    QVBoxLayout* dnsGroupLayout = new QVBoxLayout;
    dnsGroupLayout->setContentsMargins(18,3,7,0);
    dnsGroupLayout->addWidget(nameDNSGroup);
    dnsGroupLayout->addWidget(dnsSeparator);
    column2->addLayout(dnsGroupLayout);
    column2->addLayout(layoutDNS1);
    column2->addLayout(layoutDNS2);
    column2->addStretch();

    mainLayoutWithColumns->addLayout(column1);
    QWidget *columnsSeparator = new QWidget;
    columnsSeparator->setFixedWidth(1);
    columnsSeparator->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding );
    columnsSeparator->setStyleSheet(QString("background-color: #b2bfc3;"));

    mainLayoutWithColumns->addWidget(columnsSeparator);
    mainLayoutWithColumns->addLayout(column2);

    netconfigFrame->setLayout(mainLayoutWithColumns);
    mainLayout->addWidget(netconfigFrame);
    setLayout(mainLayout);
}

/*
 * Setup model of network config elements
*/
void NetConfigTab::setIPWidgets()
{
    IPWidget* widget;

    ip = new QLabel;
    ip->setText("IPv4 Address");
    ip->setProperty("networkLabels", true);
    ip->setAlignment(Qt::AlignVCenter);
    ip->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    ip->setMaximumSize(150,24);

    dns1 = new QLabel;
    dns1->setText("DNS1");
    dns1->setProperty("networkLabels", true);
    dns1->setAlignment(Qt::AlignVCenter);
    dns1->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    dns1->setMaximumSize(150,24);

    dns2 = new QLabel;
    dns2->setText("DNS2");
    dns2->setAlignment(Qt::AlignVCenter);
    dns2->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    dns2->setProperty("networkLabels", true);
    dns2->setMaximumSize(150,24);

    netmask = new QLabel;
    netmask->setText("Subnet Mask");
    netmask->setAlignment(Qt::AlignVCenter);
    netmask->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    netmask->setMaximumSize(150,24);
    netmask->setProperty("networkLabels", true);

    gateway= new QLabel;
    gateway->setText("Router/Gateway");
    gateway->setAlignment(Qt::AlignVCenter);
    gateway->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    gateway->setMaximumSize(150,24);
    gateway->setProperty("networkLabels", true);

   // QFont font("Calibri",9);
    macValue = new QLabel;
    macValue->setText("EE:EE:EE:EE:EE:EE");
    macValue->setProperty("greyFrame", true);
    //macValue->setFont(font);
    macValue->setAlignment(Qt::AlignCenter);
    //macValue->setFrameShape(QFrame::Box);
    macValue->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    macValue->setFixedSize(150,26);

    macName = new QLabel;
    macName->setText("MAC ");
    macName->setAlignment(Qt::AlignVCenter);
    macName->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    macName->setMaximumSize(300,24);
    macName->setProperty("networkLabels", true);

    dhcpSelector = new QComboBox;
    dhcpSelector->setObjectName("dhcpSelector");
    dhcpSelector->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    dhcpSelector->setFixedSize(150,28);
    dhcpSelector->addItem("Using DHCP");
    dhcpSelector->addItem("Manual");

    dhcpLabel = new QLabel;
    dhcpLabel->setText("Configure IPv4");
    dhcpLabel->setAlignment(Qt::AlignVCenter);
    dhcpLabel->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    dhcpLabel->setMaximumSize(300,24);
    dhcpLabel->setProperty("networkLabels", true);

    widget = new IPWidget();
    widget->setAccessibleName("DNS1");
    ipWidgetMap.insert("DNS1",widget);
    widget = new IPWidget();
    widget->setAccessibleName("DNS2");
    ipWidgetMap.insert("DNS2",widget);
    widget = new IPWidget();
    widget ->setAccessibleName("IP");
    ipWidgetMap.insert("IP",widget);
    widget = new IPWidget();
    widget ->setAccessibleName("Netmask");
    ipWidgetMap.insert("Netmask",widget);
    widget = new IPWidget();
    widget->setAccessibleName("Gateway");
    ipWidgetMap.insert("Gateway",widget);

    QMapIterator<QString, IPWidget*> it(ipWidgetMap);
    while (it.hasNext())
    {
        it.next();
        connect(it.value(),SIGNAL(sendIP(QString)),this,SLOT(handleNetconfigPage(QString))) ;
    }
    connect( dhcpSelector, SIGNAL(activated(int)),this, SLOT(IPSettingsHandler(int) ) );
}

/*
 * Operate with IP manually setting
 */
void NetConfigTab::IPSettingsHandler( int b )
{
    setIPConfigurationFormsEnabled(b);

    if( !b )
        commandBuffer->insert( "IP DHCP","true" );
    else
    {
        qDebug()<<ipWidgetMap.value("IP")->getIP();
        commandBuffer->insert( "IP",ipWidgetMap.value("IP")->getIP() );
        commandBuffer->insert( "Gateway",ipWidgetMap.value("Gateway")->getIP() );
        commandBuffer->insert( "Netmask",ipWidgetMap.value("Netmask")->getIP() );
    }
    emit bufferFilled(true);
}


/*
 * Fill commands buffer with changed network config elements
*/
void NetConfigTab::handleNetconfigPage( QString str )
{
    IPWidget* widget = dynamic_cast<IPWidget*> ( sender() );
    commandBuffer->insert( widget->accessibleName(),str );
    emit bufferFilled(true);
}

/*
 * Set network configuration page
 */
void NetConfigTab::fillNetworkConfigForm( QList<QStringList> rows )
{
    QStringList rowElements;
    QRegExp exp ("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}");

    foreach ( rowElements,rows )
    {
        if( rowElements[0].contains( "mac",Qt::CaseInsensitive ) )
                macValue->setText( rowElements[1] );
        else if( rowElements[0].contains( "link",Qt::CaseInsensitive ) )
                continue;
        else if ( rowElements[1].contains( exp ) )
        {
            if ( rowElements[0].contains( "IP", Qt::CaseInsensitive ) )
            {
                if( rowElements[1].contains( "DHCP" ) )
                {
                    dhcpSelector->setCurrentIndex(0);

                    setIPConfigurationFormsEnabled(false);
                }
                else
                {
                    dhcpSelector->setCurrentIndex(1);
                    setIPConfigurationFormsEnabled(true);
                }
                QString h = rowElements[1].section( " ",0,0 );
                rowElements[1] = h;
            }
            QString str = QString("%1")
                    .arg(rowElements[0].trimmed());
            ipWidgetMap.value( str )->setIP(rowElements[1]);
            qDebug()<<str<<" "<<rowElements[1];
            qDebug()<<ipWidgetMap.value( str )->getIP();
        }
    }
}

void NetConfigTab::setIPConfigurationFormsEnabled (bool b)
{
    QStringList list;
    list << "IP"<<"Netmask"<<"Gateway";

    foreach(QString key, list)
    {
        IPWidget* w;
        w = ipWidgetMap.value(key);
        w->setEnabled(b);
        w ->setIPWidgetEnabled(b);
    }
}
