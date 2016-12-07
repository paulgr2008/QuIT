#include "consolefooter.h"
#include "ui_consolefooter.h"

ConsoleFooter::ConsoleFooter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConsoleFooter)
{
    ui->setupUi(this);
    QHBoxLayout* consoleFooterLayout = new QHBoxLayout;
    consoleFooterLayout->setContentsMargins(0,5,0,0);
    consoleFooterLayout->setSpacing(10);
    cleanButton = new QPushButton;
    cleanButton->setProperty("configButton", true);
    cleanButton->setText("Clear");

    fullScreenButton = new QPushButton;
    fullScreenButton->setProperty("configButton", true);
    fullScreenButton->setText("Terminal Mode");
    autocompleter = new QCheckBox;
    autocompleter->setChecked(true);
    QLabel* autocompleteLabel= new QLabel;
    autocompleteLabel->setText("Autocomplete");
    //autocompleteLabel->setFont(QFont("Calibri",9));

    consoleFooterLayout->addWidget(autocompleter);
    consoleFooterLayout->addWidget(autocompleteLabel);
    consoleFooterLayout->addStretch();
    consoleFooterLayout->addWidget(cleanButton);
    consoleFooterLayout->addWidget(fullScreenButton);
    setLayout(consoleFooterLayout);

    //setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    //setFixedHeight(50);

}

ConsoleFooter::~ConsoleFooter()
{
    delete ui;
}
