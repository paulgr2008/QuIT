#include "warningwidget.h"
#include "ui_warningwidget.h"

WarningWidget::WarningWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WarningWidget)
{
    ui->setupUi(this);
    warningText = ui->warningText;
    warningText->setObjectName("warningText");
    warningBox = ui->warningBox;
    warningBox->installEventFilter(this);
    warningButton = ui->noButton;
    warningButton->setProperty("warning",true);
    quitButton = ui->quitButton;
    quitButton->setToolTip("Close warning");
    quitButton->setProperty("warning",true);
    connect(warningButton, SIGNAL(clicked()), this, SIGNAL(buttonClicked()));
}

bool WarningWidget:: eventFilter(QObject *target, QEvent *event)
{
    if( target==warningBox )
    {
        if( event->type()==QEvent::MouseButtonPress )
        {
            QMouseEvent* m_event = static_cast<QMouseEvent*>(event);
            if(m_event->button()==Qt::LeftButton)
            {
                hide();
            }
        }
    }
    return QWidget::eventFilter(target,event);
}

void WarningWidget::setWarningText(QString strWarning)
{
    warningText->setText(strWarning);
}

WarningWidget::~WarningWidget()
{
    delete ui;
}
