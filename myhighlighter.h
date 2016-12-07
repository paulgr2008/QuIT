#ifndef MYHIGHLIGHTER_H
#define MYHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class MyHighlighter: public QSyntaxHighlighter
{
    Q_OBJECT

public:
    MyHighlighter(QTextDocument *parent = 0);
    void setParameters(int _index, int _length);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;
private:
    struct HighlightingRule
        {
            QRegExp pattern;
            QTextCharFormat format;
        };
    QVector<HighlightingRule> highlightingRules;
    QTextCharFormat errorFormat;
    int index;
    int length;
    QString word;
};

#endif // MYHIGHLIGHTER_H
