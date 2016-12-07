#ifndef IPWIDGET_H
#define IPWIDGET_H

#include <QObject>
#include <QtWidgets>

class IPWidget : public QWidget
{
        Q_OBJECT
    public:
        explicit IPWidget(QWidget *parent=0);

        explicit IPWidget( QLineEdit* ledit,
                           QLineEdit* ledit1,
                           QLineEdit* ledit2,
                           QLineEdit* ledit3,
                           QWidget *parent=0) ;
        QString getIP() const;
        void setIPWidgetEnabled(bool);
        //QString getName() const;
        void setIP(QString str);
        ~IPWidget();
    signals:
        void sendIP(QString);
    private slots:
        void popupWarning(QString );
        void finishEditIPAddress();
        void hideToolTip()
        {
            QToolTip::hideText();
        }
    protected:

        virtual bool eventFilter(QObject *target, QEvent *event);
    private:
        void setDotLabel(QLabel* label);
        QString segmentsTextToString() const;
        QStandardItemModel* model;
        QDataWidgetMapper* mapper;
        QList<QStandardItem*> itemsList;
        QValidator* validator;
        QSignalMapper* signalMapper;

        QLineEdit* seg0;
        QLineEdit* seg1;
        QLineEdit* seg2;
        QLineEdit* seg3;
        QList<QLineEdit*> ipSegments;
        QString ip;
        QFrame* frame;

        QLabel* labelName;

        QString name;


};

#endif // IPWIDGET_H
