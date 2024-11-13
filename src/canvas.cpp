#include "canvas.h"

#include <QPainter>
#include "line.h"

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
{
}

QPointF Canvas::projectTo2D(float x, float y, float z) {
    float distance = 200.0f;
    float factor = distance / (distance + z);
    float projectedX = x * factor + width() / 2.0f;
    float projectedY = -y * factor + height() / 2.0f;
    return QPointF(projectedX, projectedY);
}

void Canvas::printAxes()
{
    QPointF origin = projectTo2D(0, 0, 0);

    // Конечные точки для осей X, Y, Z
    QPointF xAxisEnd = projectTo2D(100, 0, 0);
    QPointF yAxisEnd = projectTo2D(0, 100, 0);
    QPointF zAxisEnd = projectTo2D(0, 0, 100);

    // QPointF origin = projectTo2D(0, 0, 0);

    Line *axisX = new Line(qMakePair(origin, xAxisEnd), Qt::red, this);
    Line *axisY = new Line(qMakePair(origin, yAxisEnd), Qt::green, this);
    Line *axisZ = new Line(qMakePair(origin, zAxisEnd), Qt::blue, this);

    objectsArray.push_back(axisX);
    objectsArray.push_back(axisY);
    objectsArray.push_back(axisZ);
}

void Canvas::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    printAxes();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    for (const auto &curObject : objectsArray) {
        curObject->draw(painter);
    }
}
