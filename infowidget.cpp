#include "infowidget.h"
#include "ui_infowidget.h"

InfoWidget::InfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoWidget)
{
    ui->setupUi(this);
    ui->groupBox->setStyleSheet("#groupBox{border:1px solid #b2bfc3,background:rgb(240,240,240)}");

}

InfoWidget::~InfoWidget()
{
    delete ui;
}
