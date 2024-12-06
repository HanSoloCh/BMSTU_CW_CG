#include <QTimer>
#include <QMatrix4x4>
#include "canvas.h"
#include "qapplication.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Canvas canvas;
    canvas.resize(1000, 1000);
    canvas.show();

    // QTimer timer;
    // QObject::connect(&timer, &QTimer::timeout, [&]() {
    //     QMatrix4x4 matrix;
    //     // matrix.translate(0, 0, -1);
    //     matrix.translate(0, 0, 8);
    //     matrix.rotate(1, 1, 1, 0);
    //     matrix.translate(0, 0, -8);

    //     canvas.Transform(matrix);
    //     canvas.update();
    // });

    // timer.start(100); // Каждую секунду перемещать
    return a.exec();
}
