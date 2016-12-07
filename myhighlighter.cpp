#include "myhighlighter.h"
#include <QDebug>

MyHighlighter::MyHighlighter(QTextDocument *parent)
: QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    errorFormat.setForeground(Qt::red);
    errorFormat.setFontWeight(QFont::Bold);
    QStringList errorPatterns;
    errorPatterns << "\\bRebooting\\b" << "\\bDisconnected\\b";
    foreach (const QString &pattern, errorPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = errorFormat;
        highlightingRules.append(rule);
    }
  /*  QTextCharFormat connectFormat;
    connectFormat.setForeground(Qt::green);
    connectFormat.setFontWeight(QFont::Bold);
    QString connectPattern = "\\bConnected\\b";
    rule.pattern = QRegExp(connectPattern);
    rule.format = connectFormat;
    highlightingRules.append(rule);*/
}

void MyHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);
}

void MyHighlighter::setParameters(int _index, int _length)
{
    index =_index;
    length = _length;

}

