#include "canvas.h"

#include <QPainter>
#include <QMouseEvent>


#include "strategy.h"
#include "line.h"

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
{
}

void Canvas::addObject(AbstractObject *newObject)
{
    objectsArray.push_back(std::shared_ptr<AbstractObject>(newObject));
}

void Canvas::rotate(double xAngle, double yAngle, double zAngle)
{
    for (const auto &curObject : objectsArray) {
        curObject->rotate(xAngle, yAngle, zAngle);
    }
}

void Canvas::addAxes()
{
    Point origin{0, 0, 0};

    Point xAxisEnd{100, 0, 0};
    Point yAxisEnd{0, 100, 0};
    Point zAxisEnd{0, 0, 100};

    Line *axisX = new Line(qMakePair(origin, xAxisEnd), Qt::red);
    Line *axisY = new Line(qMakePair(origin, yAxisEnd), Qt::green);
    Line *axisZ = new Line(qMakePair(origin, zAxisEnd), Qt::blue);

    addObject(axisX);
    addObject(axisY);
    addObject(axisZ);
}

void Canvas::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    if (objectsArray.empty())
        addAxes();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    DefaultProjectionStrategy strategy;
    for (const auto &curObject : objectsArray) {
        curObject->draw(painter, strategy, size());
    }
}

