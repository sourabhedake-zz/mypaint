#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    extern drawingSheet *l;
    l = new drawingSheet();
    return a.exec();
}
