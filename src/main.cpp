#include "mainwindow.h"

#include <QApplication>

#include "canvas.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Canvas canvas;
    canvas.resize(800, 800);
    canvas.show();
    return a.exec();
}
