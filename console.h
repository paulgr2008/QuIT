/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef CONSOLE_H
#define CONSOLE_H

#include <QPlainTextEdit>
#include <QtGui>
#include <QCompleter>
#include "treemodelcompleter.h"
#include "myhighlighter.h"

QT_BEGIN_NAMESPACE
class TreeModelCompleter;
class QFontDatabase;
QT_END_NAMESPACE

class Console : public QPlainTextEdit
{
    Q_OBJECT

signals:
    void getDataFromConsole(const QByteArray &data,bool r);
    void consoleClose();

public:
    explicit Console(QWidget *parent = 0);
    ~Console();
    void setCompleter(TreeModelCompleter* completer);
    TreeModelCompleter* completer() const;
    void putData(const QByteArray &data);
    void insertPrompt(bool insertNewBlock = true);
    void setLocalEchoEnabled(bool set);
    void insertWarningText(const QString str );


protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
  //  virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void mouseDoubleClickEvent(QMouseEvent *e);
    virtual void contextMenuEvent(QContextMenuEvent *e);
    void mouseMoveEvent(QMouseEvent* e);
    void focusInEvent(QFocusEvent *e);
private slots:
    void insertCompletion(QString completion);
    void completerHandler(  );

private:
    void historyAdd(QString);
    void historyBack();
    void historyForward();
    void scrollDown();
    void onEnter();
    void highlightWord(QTextCursor cursor);
    void highlightLine(QTextCursor cursor);

    //void completionHighlighted(QString str);
    QString textUnderCursor() const;
    QString prompt;
    bool localEchoEnabled;
    int historyPos;
    int count;
    QStringList* history;
    QStringList commandList;
    TreeModelCompleter* comp;
    QByteArray data;
    QString word;
    bool isDoubleClick;
    bool isClickSelected;
    bool isSelection;
    bool autocompleteOn;
    int pos;
    QString cmd;
    MyHighlighter* highlighter;
    QTextEdit::ExtraSelection currentWord;
    int countClick;


};

#endif // CONSOLE_H
