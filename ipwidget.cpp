#include "ipwidget.h"

IPWidget::IPWidget(QLineEdit* ledit,
                   QLineEdit* ledit1,
                   QLineEdit* ledit2,
                   QLineEdit* ledit3,
                   QWidget *parent) : QWidget(parent)
{

    seg0  = ledit;
    seg1 = ledit1;
    seg2 = ledit2;
    seg3 = ledit3;

    ipSegments.append(ledit);
    ipSegments.append(ledit1);
    ipSegments.append(ledit2);
    ipSegments.append(ledit3);

    validator = new QIntValidator( 0,255,this );
    for ( int i=0; i<ipSegments.size(); i++ )
    {
        QLineEdit* ed = ipSegments.at(i);

        ed->setValidator( validator );
        ed->installEventFilter( this );
        connect( ed,SIGNAL(textEdited(QString)),this,SLOT(popupWarning(QString)) );
        connect( ed,SIGNAL(returnPressed()),this,SLOT(finishEditIPAddress()) );
    }
}

void IPWidget::setIP( QString str )
{
    QStringList list = str.split(".");
    for( int i=0; i<list.size();i++ )
        ipSegments.at(i)->setText(list.at(i));
}

QString IPWidget::getIP() const
{
    return ip;
}

void IPWidget::stylesSheetsForEditors()
{
    /*seg0->setStyleSheet(tr( "border-top: 1px solid black;"
                                    "border-bottom: 1px solid black;"
                                    "border-left: 1px solid black;"
                                    "border-right: 0px solid white;"
                                    "padding: 0px;"
                                    "background: white" ));

    seg1->setStyleSheet(tr( "border-top: 1px solid black;"
                                    "border-bottom: 1px solid black;"
                                    "border-left: 0px solid white;"
                                    "border-right: 0px solid white;"
                                    "padding: 0px;"
                                    "background: white" ));

    seg2->setStyleSheet(tr( "border-top: 1px solid black;"
                                    "border-bottom: 1px solid black;"
                                    "border-left: 0px solid white;"
                                    "border-right: 0px solid white;"
                                    "padding: 0px;"
                                    "background: white" ));
    seg3->setStyleSheet(tr( "border-top: 1px solid black;"
                                    "border-bottom: 1px solid black;"
                                    "border-left: 0px solid white;"
                                    "border-right: 1px solid black;"
                                    "padding: 0px;"
                                    "background: white" ));*/
}

void IPWidget::finishEditIPAddress()
{
    for ( int i=0; i<ipSegments.size(); i++ )
    {
       QString str = ipSegments.at(i)->text();

       if( str.isEmpty() )
           return;
       ip.append(str);
       if(i==ipSegments.size()-1)
           break;
       ip.append(".");
    }
    qDebug()<< ip;
    emit sendIP(ip);
    ip.clear();
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
    if ( validator->validate(str, pos) == QValidator::Invalid)
    {
        QMessageBox::warning(this,"","Please, specify value between 0 and 255!");
        //ed->setText("");
    }
}

IPWidget::~IPWidget()
{

}

