#include "ipwidget.h"

IPWidget::IPWidget(QWidget *parent) : QWidget(parent)
{
    seg0  = new QLineEdit;
    seg1 = new QLineEdit;
    seg2 = new QLineEdit;
    seg3 = new QLineEdit;

    ipSegments.append(seg0);
    ipSegments.append(seg1);
    ipSegments.append(seg2);
    ipSegments.append(seg3);

    validator = new QIntValidator( 0,255,this );

    QFont font("Tahoma",9);

    for ( int i=0; i<ipSegments.size(); i++ )
    {
        QLineEdit* ed = ipSegments.at(i);


        ed->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        ed->setFixedSize(33,24);
        ed->setMaxLength(3);
        ed->setFont(font);
        ed->setFrame(false);
        ed->setAlignment(Qt::AlignCenter);
        ed->setAutoFillBackground(true);
        ed->setValidator( validator );
        ed->installEventFilter( this );
        //connect( ed,SIGNAL(textEdited(QString)),this,SLOT(popupWarning(QString)) );
        connect( ed,SIGNAL(textChanged(QString)),this,SLOT(popupWarning(QString)) );
        connect( ed,SIGNAL(returnPressed()),this,SLOT(finishEditIPAddress()) );
    }

    frame = new QFrame;
    frame->setProperty("greyFrame",true);
    //frame->setFrameShape(QFrame::Box);
    frame->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    frame->setFixedSize(150,26);
    frame->setStyleSheet(tr("background: white;"));
    QHBoxLayout* layout = new QHBoxLayout;
    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->setMargin(0);

    QLabel* dot1 = new QLabel;
    setDotLabel(dot1);
    QLabel* dot2 = new QLabel;
    setDotLabel(dot2);
    QLabel* dot3 = new QLabel;
    setDotLabel(dot3);

    layout->addWidget(seg0);
    layout->addWidget(dot1);
    layout->addWidget(seg1);
    layout->addWidget(dot2);
    layout->addWidget(seg2);
    layout->addWidget(dot3);
    layout->addWidget(seg3);
    layout->setSpacing(0);
    layout->setMargin(0);
    frame->setLayout(layout);
    mainLayout->addWidget(frame);
    setLayout(mainLayout);
}
void IPWidget::setIPWidgetEnabled(bool b)
{
    frame->setStyleSheet(b?tr("background:white"):tr("background:transparent"));
}


void IPWidget::setDotLabel(QLabel* label)
{
    QFont font("Tahoma",9);
    label->setProperty("dotLabel",true);
    label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    label->setFixedSize(5,24);
    label->setFont(font);
    label->setAutoFillBackground(true);
    label->setFrameShape(QFrame::NoFrame);
    label->setFrameShadow(QFrame::Plain);
    label->setText(".");
    label->setAlignment(Qt::AlignCenter);

}

void IPWidget::setIP( QString str )
{
    QStringList list = str.split(".");
    for( int i=0; i<list.size();i++ )
    {
        ipSegments.at(i)->setText(list.at(i));
    }
}

QString IPWidget::segmentsTextToString() const
{
    QString strIP;
    for ( int i=0; i<ipSegments.size(); i++ )
    {
       QString str = ipSegments.at(i)->text();

       strIP.append(str);
       if(i==ipSegments.size()-1)
           break;
       strIP.append(".");
    }
   return strIP;
}

QString IPWidget::getIP() const
{
    return segmentsTextToString();
}

void IPWidget::finishEditIPAddress()
{
    ip.clear();
    for ( int i=0; i<ipSegments.size(); i++ )
    {
       QString str = ipSegments.at(i)->text();

       if( str.isEmpty() )
       {
           QToolTip::showText(ipSegments.at(i)->mapToGlobal(QPoint()), "Please, specify value between 0 and 255!");
           return;
       }
       ip.append(str);
       if(i==ipSegments.size()-1)
           break;
       ip.append(".");
    }
    qDebug()<< ip;
    emit sendIP(ip);


}

bool IPWidget::eventFilter( QObject *target, QEvent *event )
{
   int i;
   int size;
   QLineEdit* editCurrent;

   if ( target==seg0||target==seg1||target==seg2||target==seg3 )
   {
       editCurrent = dynamic_cast<QLineEdit*>(target);

       if (event->type() == QEvent::KeyPress)
       {
             QKeyEvent *keyEvent = (QKeyEvent *)event;
             i = ipSegments.indexOf(editCurrent);
             size = editCurrent->text().size();
             if ( keyEvent->key() == Qt::Key_Right )
             {
               if( editCurrent->cursorPosition()<size )
               {
                    editCurrent->cursorForward(false);
               }
               else
               {
                    if( i<3 )
                    {
                        QString str = editCurrent->text();
                        while( str.left(1)=="0" && str.size()!=1 )
                            str.remove(0,1);
                        editCurrent->setText(str);
                        ipSegments.at(i+1)->setFocus();
                        ipSegments.at(i+1)->setCursorPosition(0);
                    }
               }
                return true;
             }
             else if( keyEvent->key()==Qt::Key_Left )
             {
                 if( editCurrent->cursorPosition()>0 )
                 {
                      editCurrent->cursorBackward(false);
                 }
                 else
                 {
                    if( i>0 )
                    {
                        ipSegments.at(i-1)->setFocus();
                        ipSegments.at(i-1)->end(false);
                    }
                 }
                return true;
             }
        }
    }
    return QWidget::eventFilter( target, event );
}

void IPWidget::popupWarning( QString str )
{
    //QIntValidator v(0, 255,ed);
    int pos = 0;
    QLineEdit * ed = dynamic_cast<QLineEdit*>(sender());
    if( ed->hasFocus() )
    {
        qDebug()<<"ip changed";
        finishEditIPAddress();

    }

    if ( validator->validate(str, pos) == QValidator::Invalid)
    {
        QMessageBox::warning(this,"","Please, specify value between 0 and 255!");
        //ed->setText("");
    }
}

IPWidget::~IPWidget()
{

}

