
#define _WINSOCKAPI_
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    w.GPSplotInit();
    return a.exec();
}
