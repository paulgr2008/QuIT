
#include "console.h"
#include <QScrollBar>
#include <QAbstractItemView>
#include <QtCore/QDebug>
#include "treemodelcompleter.h"

Console::Console( QWidget *parent )
    : QPlainTextEdit( parent),
    localEchoEnabled(false),comp(0)
{

    document()->setMaximumBlockCount(100);
    QPalette p = palette();
    p.setColor( QPalette::Base, Qt::black );
    p.setColor( QPalette::Text, Qt::green );
    setPalette(p);
    prompt = ">";
    insertPrompt(false);
    history = new QStringList;
    historyPos = 0;
}

void Console::insertPrompt( bool insertNewBlock )
{
    if( insertNewBlock )
        textCursor().insertBlock();

    QTextCharFormat format;
    format.setForeground( Qt::green );
    textCursor().setBlockCharFormat( format );
    textCursor().insertText( prompt );
    scrollDown();
}

void Console::scrollDown()
{
    QScrollBar *vbar = verticalScrollBar();
    vbar->setValue( vbar->maximum() );
}

void Console::putData( const QByteArray &d )
{

    QString str (d);

    if ( localEchoEnabled && str.contains(cmd))
    {
        int index = str.indexOf("\r\n");

        str.remove( 0,index+2 );
    }

    insertPlainText( str );

    QScrollBar *bar = verticalScrollBar();
    bar->setValue( bar->maximum() );


    /*data.append(d);

    if ( data.contains(0x0A) )
    {
        QString str (data);

        if ( localEchoEnabled && str.contains(cmd))
        {
            int index = str.indexOf("\r\n");

            str.remove( 0,index+2 );
        }
        qDebug()<<str;
        insertPlainText( str );

        QScrollBar *bar = verticalScrollBar();
        bar->setValue( bar->maximum() );
    }*/
}

void Console::setLocalEchoEnabled( bool set )
{
    localEchoEnabled = set;
}

void Console::setCompleter( TreeModelCompleter *completer )
{
    if ( comp )
        QObject::disconnect( comp, 0, this, 0 );

    comp = completer;
    if ( !comp )
        return;

    if( comp->widget()==0 )
        comp->setWidget(this);
    QObject::connect( comp, SIGNAL(activated(QString)),this, SLOT(insertCompletion(QString)) );
}

TreeModelCompleter *Console::completer() const
{
    return comp;
}

void Console::insertCompletion( QString completion )
{
    QString command;
    if (comp->widget() != this)
        return;

    QTextCursor tc = textCursor();
    int extra = completion.length() - comp->completionPrefix().length() ;
    tc.movePosition( QTextCursor::EndOfWord );
    command = completion.right(extra);
    tc.insertText(command);
    tc.movePosition( QTextCursor::Right );
    setTextCursor(tc);
}

QString Console::textUnderCursor() const
{
    return textCursor().block().text().mid( prompt.length() );
}

void Console::completerHandler(  )
{
    QString underCursor = textUnderCursor();
    qDebug()<<underCursor<<" "<<comp->completionPrefix();
    qDebug()<<comp->currentCompletion();
    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-=");

    if ( underCursor.compare(comp->currentCompletion())==0
         ||comp->currentCompletion().contains(underCursor+" ")
         ||underCursor.isEmpty()|| eow.contains(underCursor.right(1)) )
    {
        comp->popup()->hide();
        return;
    }

    if ( underCursor != comp->completionPrefix() )
    {
        comp->setCompletionPrefix(underCursor);
        comp->popup()->setCurrentIndex( comp->completionModel()->index(0, 0) );
    }
    QRect cr = cursorRect();
    cr.setWidth( comp->popup()->sizeHintForColumn(0)
                + comp->popup()->verticalScrollBar()->sizeHint().width() );
    comp->complete( cr ); // popup it up!
}


void Console::keyPressEvent( QKeyEvent *e )
{
    if ( comp && comp->popup()->isVisible() )
    {
        switch( e->key() ) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
             e->ignore();
             return; // let the completer do default behavior
        default:
            break;
        }
    }

        // The following keys are forwarded by the completer to the widget

    switch( e->key() )
    {
        case Qt::Key_Escape:
            break;
        case Qt::Key_Backspace:
            if( textCursor().positionInBlock() > prompt.length() )
                QPlainTextEdit::keyPressEvent(e);
            break;
        case Qt::Key_Left:
            if( textCursor().positionInBlock() > prompt.length() )
                QPlainTextEdit::keyPressEvent(e);
            break;
        case Qt::Key_Right:
            QPlainTextEdit::keyPressEvent(e);
            break;
        case Qt::Key_Up:
            historyBack();
            break;
        case Qt::Key_Down:
            historyForward();
            break;
        case Qt::Key_Return:
            onEnter();
            break;
        default:
            if ( localEchoEnabled )
                QPlainTextEdit::keyPressEvent(e);
    }
}

void Console::onEnter()
{
   if( textCursor().positionInBlock() == prompt.length() )
   {
        insertPrompt();
        return;
   }

    cmd = textUnderCursor();
    historyAdd(cmd);
    cmd.append("\n");
    emit getData(cmd.toLocal8Bit());
    insertPrompt();
}

void Console::historyAdd( QString cmd )
{
    history->append(cmd);
    historyPos = history->length();
}

void Console::historyBack()
{
    if( !historyPos )
        return;
    QTextCursor cursor = textCursor();
    cursor.movePosition( QTextCursor::StartOfBlock );
    cursor.movePosition( QTextCursor::EndOfBlock, QTextCursor::KeepAnchor );
    cursor.removeSelectedText();
    cursor.insertText( ">" + history->at(historyPos-1) );
    setTextCursor(cursor);
    historyPos--;
}

void Console::historyForward()
{
    if( historyPos == history->length() )
        return;
    QTextCursor cursor = textCursor();
    cursor.movePosition( QTextCursor::StartOfBlock );
    cursor.movePosition( QTextCursor::EndOfBlock, QTextCursor::KeepAnchor );
    cursor.removeSelectedText();
    if( historyPos == history->length() - 1 )
        cursor.insertText(">");
    else
        cursor.insertText( ">" + history->at(historyPos + 1) );
    setTextCursor(cursor);
    historyPos++;
}

void Console::mousePressEvent( QMouseEvent *e )
{
    Q_UNUSED(e)
    setFocus();
}

void Console::mouseDoubleClickEvent( QMouseEvent *e )
{
    Q_UNUSED(e)
}

void Console::contextMenuEvent( QContextMenuEvent *e )
{
    Q_UNUSED(e)
}

void Console::focusInEvent( QFocusEvent *e )
{
    if (comp)
        comp->setWidget(this);
    QPlainTextEdit::focusInEvent(e);
}

Console::~Console()
{

}

