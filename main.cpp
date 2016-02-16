#include "mainwindow.h"
#include <QApplication>

#include "headers/qt_stuff.h"
#include "simulacion.h"

int main(int argc, char *argv[])
{

   QApplication a(argc, argv);
    MainWindow *w = new MainWindow();
    //w->resize(400,400);
    w->show();

    return a.exec();
}
