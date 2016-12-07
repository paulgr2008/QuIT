#include "headerwidget.h"

HeaderWidget::HeaderWidget(QWidget *parent) : QWidget(parent)
{
    /* list of possible states */
    typesList<<"CONNECTED"<<"DISCONNECTED"<<"REBOOTING...";
    //QFont font("Neue Haas Unica Pro",9);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    updateButton = new QPushButton;
    updateButton->setText("Refresh");
    updateButton->setProperty("configButton", true);

    deviceNameLabel = new QLabel;
    deviceNameLabel->setObjectName("deviceNameLabel");
    //deviceNameLabel->setFont(font);

    labelConnected = new QLabel;
    labelConnected->setObjectName("labelConnected");
    labelConnected->setProperty("connected", true);
    labelConnected->setAlignment(Qt::AlignBottom);
    labelConnected->setText("CONNECTED");
    //labelConnected->setFont(font);

    headerLayout = new QHBoxLayout;
    headerLayout->setMargin(6);
    headerLayout->addWidget(deviceNameLabel);
    headerLayout->addWidget(labelConnected);
    headerLayout->addStretch();
    headerLayout->addWidget(updateButton);

    headerFrame = new QGroupBox;
    headerFrame->setProperty("greyFrame",true);
    headerFrame->setLayout(headerLayout);
    headerFrame->setObjectName("headerFrame");

    mainLayout->addWidget(headerFrame);
    setLayout(mainLayout);
}

void HeaderWidget::handleLabelConnected(int type)
{
    labelConnected->setText(typesList.at(type));

    switch(type)
    {
    case CONNECTED:
        labelConnected->setStyleSheet("color:green");
        break;
    case DISCONNECTED:
    case REBOOTING:
        labelConnected->setStyleSheet("color:#9a27f3");
        break;

    }

}

HeaderWidget::~HeaderWidget(){}

