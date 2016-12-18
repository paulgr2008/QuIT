
#include "console.h"
#include <QScrollBar>
#include <QAbstractItemView>
#include <QtCore/QDebug>
#include "treemodelcompleter.h"
#include <QFontDatabase>
#include "globalvars.h"
//#include <QTest>
//#include <QSignalSpy>


int n =0;

Console::Console( QWidget *parent )
    : QPlainTextEdit( parent),
    comp(0)
{
    document()->setMaximumBlockCount(1000);
    QFont font("Lucida Console");
    font.setPointSize(10);
    document()->setDefaultFont(font);
    isDoubleClick = false;
    isClickSelected = true;
    autocompleteOn = true;
    highlighter = new MyHighlighter(document());
    QPalette p = palette();
    p.setColor( QPalette::Base, Qt::black );
    p.setColor( QPalette::Text, Qt::green );
    p.setColor(QPalette::Highlight, Qt::gray);
    p.setColor(QPalette::HighlightedText, Qt::white);
    setPalette(p);
    prompt = ">";
    insertPrompt(false);
    history = new QStringList;
    historyPos = 0;

}

void Console::insertPrompt( bool insertNewBlock )
{
    QTextCursor tc = textCursor();
    if( insertNewBlock )
        tc.insertBlock();
    QTextCharFormat format;
    format.setForeground( Qt::green );
    tc.setBlockCharFormat( format );
    tc.insertText( prompt );
    tc.movePosition( QTextCursor::EndOfWord );
    ensureCursorVisible();
    scrollDown();
    setFocus();
}

void Console::scrollDown()
{
    QScrollBar *vbar = verticalScrollBar();
    vbar->setValue( vbar->maximum() );
}

void Console::putData( const QByteArray &d )
{
    QString str (d);

    if (!cmd.isEmpty())
    {
        while ( str.contains(cmd) )
        {
            str.remove(cmd);
        }
        qDebug()<<str;

        if (str.contains("\r\n>\r\n"))
            str.remove(str.indexOf("\r\n>"),3);
        if (str.contains(">>"))
            str.remove(str.indexOf(">"),1);

        insertPlainText( str );
        scrollDown();
    }
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
    QObject::connect( comp, SIGNAL(activated(QString)),
                      this, SLOT(insertCompletion(QString)) );
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
    if( autocompleteOn )
    {
        QString underCursor = textUnderCursor();
        QString complPref = comp->completionPrefix();
       // qDebug()<<underCursor<<" "<<comp->completionPrefix();
       // qDebug()<<comp->currentCompletion();
        static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-=");
        if ( underCursor.compare(comp->currentCompletion())==0 ||
            (complPref.endsWith(" ") && !complPref.trimmed().compare(textUnderCursor())) ||
            underCursor.isEmpty() || eow.contains(underCursor.right(1)) )
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
        if (!comp->currentCompletion().compare(comp->completionPrefix()))
            comp->popup()->hide();
        //qDebug()<<"after complete "<<underCursor<<" "<<comp->completionPrefix();
        //qDebug()<<comp->currentCompletion();
    }

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
             return; /* let the completer do default behavior */
        default:
            break;
        }
    }


    if(e->modifiers().testFlag(Qt::AltModifier))
    {
        switch( e->key() )
        {
            case Qt::Key_C:
                clear();
                insertPrompt(false);
                e->accept();
                return;
            case Qt::Key_A:
                if (autocompleteOn)
                    autocompleteOn = false;
                else autocompleteOn = true;
                e->accept();
                return;
            case Qt::Key_Q:
                emit consoleClose();
                e->accept();
            default:
                e->ignore();
                return;
        }
    }
    /* The following keys are forwarded by the completer to the widget */
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
            //if ( localEchoEnabled )
            QPlainTextEdit::keyPressEvent(e);
    }
}

void Console::onEnter()
{
    QTextCursor cursor =textCursor() ;
    if( textCursor().positionInBlock() == prompt.length() )
    {
       //return;
    }
    bool r = false;
    cmd = textUnderCursor();
    historyAdd(cmd);
    if (!cmd.compare("echo 0"))
        localEchoEnabled = false;
    else if(!cmd.compare("echo 1"))
        localEchoEnabled = true;
    else if (!cmd.compare("clear"))
    {
        clear();
        insertPrompt(false);
        return;
    }
    else if (!cmd.compare("autocomplete on"))
    {
        autocompleteOn = true;
        insertPrompt(false);
        return;
    }
    else if (!cmd.compare("autocomplete off"))
    {
        autocompleteOn = false;
        insertPrompt(false);
        return;
    }
    else if(!cmd.compare("quit"))
    {
        emit consoleClose();
        return;
    }

    cmd.append("\r\n");
    cursor.insertBlock();
    if( cmd.contains("boot") )
    {
        r = true;
       // cursor.insertBlock();
    }
    qDebug()<<cmd;
    emit getDataFromConsole( cmd.toLocal8Bit(),r );


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
    autocompleteOn = false;
    QTextCursor cursor = textCursor();
    cursor.movePosition( QTextCursor::StartOfBlock );
    cursor.movePosition( QTextCursor::EndOfBlock, QTextCursor::KeepAnchor );
    cursor.removeSelectedText();
    cursor.insertText( prompt + history->at(historyPos-1) );
    setTextCursor(cursor);
    autocompleteOn = true;
    historyPos--;
}

void Console::historyForward()
{
    if( historyPos == history->length() )
        return;
    autocompleteOn = false;
    QTextCursor cursor = textCursor();
    cursor.movePosition( QTextCursor::StartOfBlock );
    cursor.movePosition( QTextCursor::EndOfBlock, QTextCursor::KeepAnchor );
    cursor.removeSelectedText();
    if( historyPos == history->length() - 1 )
        cursor.insertText(prompt);
    else
        cursor.insertText( prompt + history->at(historyPos + 1) );
    setTextCursor(cursor);
    autocompleteOn = true;
    historyPos++;
}

void Console::mousePressEvent( QMouseEvent *e )
{
    if (Qt::RightButton == e->button())
    {
        QTextCursor cursor = textCursor();
        cursor.movePosition(QTextCursor::End);
        setTextCursor(cursor);
        cursor.insertText(word);
        scrollDown();
    }
    else if(Qt::LeftButton == e->button()  )
    {
        QTextCursor cursor = cursorForPosition(e->pos());
        pos = cursor.position();
        if (isDoubleClick )
        {
           QTextCursor cursor = cursorForPosition(e->pos());
           highlightLine(cursor);
           cursor.movePosition(QTextCursor::End);
           setTextCursor(cursor);
           isDoubleClick = false;
           isClickSelected = false;
           return;
        }

        if ( !extraSelections().isEmpty() )
        {
            QList<QTextEdit::ExtraSelection> extraSelections;//empty list
            setExtraSelections(extraSelections);
            isClickSelected = true;
        }
    }
}

void Console::mouseDoubleClickEvent( QMouseEvent *e )
{
    if (Qt::LeftButton == e->button())
    {
        if(isClickSelected)
        {
            QTextCursor cursor = cursorForPosition(e->pos());
            highlightWord(cursor);
            cursor.movePosition(QTextCursor::End);
            setTextCursor(cursor);
            isDoubleClick = true;
        }
    }
}

void Console::mouseMoveEvent(QMouseEvent* e)
{
    if(e->buttons() && Qt::LeftButton )
    {
        QTextCursor cursor = cursorForPosition(e->pos());
        cursor.setPosition( pos, QTextCursor::KeepAnchor );
        word = cursor.selectedText();
        QList<QTextEdit::ExtraSelection> extraSelections;
        QColor color = Qt::white;
        currentWord.format.setBackground(color);
        currentWord.cursor = cursor;
        extraSelections.append(currentWord);
        setExtraSelections(extraSelections);

    }
}
void Console::insertWarningText(const QString str )
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();

    cursor.insertText(str);
    if(str.contains(""))
    {
        //cursor.insertBlock();
        emit getDataFromConsole( QString("\r\n").toLocal8Bit(),false );
        //insertPlainText("\r\n");
    }

    setTextCursor(cursor);
}

void Console::highlightLine(QTextCursor cursor)
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    cursor.movePosition( QTextCursor::StartOfBlock );
    if(cursor.block().text().contains(prompt))
    {
        cursor.movePosition(QTextCursor::NextCharacter);
    }

    cursor.movePosition( QTextCursor::EndOfBlock, QTextCursor::KeepAnchor );
    word = cursor.selectedText();

    QColor color = Qt::white;
    currentWord.format.setBackground(color);
    currentWord.cursor = cursor;
    extraSelections.append(currentWord);
    setExtraSelections(extraSelections);

}

void Console::highlightWord(QTextCursor cursor)
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    cursor.movePosition( QTextCursor::StartOfWord );
    cursor.select(QTextCursor::WordUnderCursor);
    if(cursor.selectedText().contains(prompt))
    {
        cursor.movePosition(QTextCursor::NextCharacter);
    }
    cursor.movePosition( QTextCursor::EndOfWord, QTextCursor::KeepAnchor );
    word = cursor.selectedText();

    QColor color = Qt::white;
    currentWord.format.setBackground(color);
    currentWord.cursor = cursor;
    extraSelections.append(currentWord);
    setExtraSelections(extraSelections);
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

