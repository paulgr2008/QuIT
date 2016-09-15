#ifndef IPWIDGET_H
#define IPWIDGET_H

#include <QObject>
#include <QtWidgets>

class IPWidget : public QWidget
{
        Q_OBJECT
    public:
        explicit IPWidget( QLineEdit* ledit,
                           QLineEdit* ledit1,
                           QLineEdit* ledit2,
                           QLineEdit* ledit3,
                           QWidget *parent=0) ;
        QString getIP() const;
        //QString getName() const;
        void setIP(QString str);
        ~IPWidget();
    signals:
        void sendIP(QString);
    private slots:
        void popupWarning(QString );
        void finishEditIPAddress();
    protected:
    //virtual void keyPressEvent(QKeyEvent *e);
        virtual bool eventFilter(QObject *target, QEvent *event);
    private:

        void stylesSheetsForEditors();
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

        QLabel* labelName;

        QString name;


};

#endif // IPWIDGET_H
