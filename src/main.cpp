#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("tPATH");
    a.setApplicationVersion("0.0.2");

    MainWindow w;
    w.show();

    return a.exec();
}
