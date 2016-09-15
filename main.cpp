#include "mainwindow.h"
#include <QApplication>
#include "styleloader.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QFile styleFile;
    styleFile.setFileName("C:/XOL/myfiles/CATools/styles.css");
    styleFile.open(QFile::ReadOnly);
    QString qssString = styleFile.readAll();
    qApp->setStyleSheet(qssString);

//StyleLoader::attach("C:/XOL/myfiles/CATools/styles.css", QKeySequence("F6"));
    return a.exec();
}
