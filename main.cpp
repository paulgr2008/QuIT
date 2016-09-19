#include "mainwindow.h"
#include <QApplication>
#include "styleloader.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();



//StyleLoader::attach("C:/XOL/myfiles/CATools/styles.css", QKeySequence("F6"));
    return a.exec();
}
