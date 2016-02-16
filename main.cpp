#include <QApplication>
#include "waitingdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    waitingDialog *w = new waitingDialog();
    w->show();

    return a.exec();
}
