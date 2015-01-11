#include "mainwindow.h"
#include "biddingboxwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    BiddingBoxWindow w;
    w.show();

    return a.exec();
}
