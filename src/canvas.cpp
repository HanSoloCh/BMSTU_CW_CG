#include "canvas.h"

#include <QPainter>
#include <QMouseEvent>


#include "strategy.h"
#include "line.h"

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
{
}

void Canvas::addObject(std::shared_ptr<AbstractObject> newObject)
{
    // Не очень нравится, что скачу по уровням абстракции
    newObject->move(Transformation.MoveTransform.xMove, Transformation.MoveTransform.yMove, Transformation.MoveTransform.zMove);
    newObject->rotate(Transformation.RotateTransform.xAngle, Transformation.RotateTransform.yAngle, Transformation.RotateTransform.zAngle);
    newObject->scale(Transformation.ScaleTransform.xScale, Transformation.ScaleTransform.yScale, Transformation.ScaleTransform.zScale);

    objectsArray.push_back(newObject);
}

void Canvas::move(double xMove, double yMove, double zMove)
{
    saveMove(xMove, yMove, zMove);
    for (auto &curObject : objectsArray) {
        curObject->move(xMove, yMove, zMove);
    }
}

void Canvas::rotate(double xAngle, double yAngle, double zAngle)
{
    // Вращение присходит не относительно начала координат, а относительно центра экрана (ХЗ испралять ли)
    saveRotate(xAngle, yAngle, zAngle);
    for (auto &curObject : objectsArray) {
        curObject->rotate(xAngle, yAngle, zAngle);
    }
}

void Canvas::scale(double xScale, double yScale, double zScale)
{
    saveScale(xScale, yScale, zScale);
    for (auto &curObject : objectsArray) {
        curObject->scale(xScale, yScale, zScale);
    }
}

void Canvas::deleteAll()
{
    int i = 0;
    while (i < objectsArray.size()) {
        if (!objectsArray[i]->isConst())
            objectsArray.remove(i);
        else
            ++i;
    }
    addAxes();
}

void Canvas::addAxes()
{
    Point origin{0, 0, 0};

    Point xAxisEnd{100, 0, 0};
    Point yAxisEnd{0, 100, 0};
    Point zAxisEnd{0, 0, 100};

    auto axisX = std::make_shared<Line>(qMakePair(origin, xAxisEnd), Qt::red);
    auto axisY = std::make_shared<Line>(qMakePair(origin, yAxisEnd), Qt::green);
    auto axisZ = std::make_shared<Line>(qMakePair(origin, zAxisEnd), Qt::blue);

    addObject(axisX);
    addObject(axisY);
    addObject(axisZ);
}

void Canvas::saveMove(double xMove, double yMove, double zMove)
{
    Transformation.MoveTransform.xMove += xMove;
    Transformation.MoveTransform.yMove += yMove;
    Transformation.MoveTransform.zMove += zMove;
}

void Canvas::saveRotate(double xAngle, double yAngle, double zAngle)
{
    Transformation.RotateTransform.xAngle += xAngle;
    Transformation.RotateTransform.yAngle += yAngle;
    Transformation.RotateTransform.zAngle += zAngle;
}

void Canvas::saveScale(double xScale, double yScale, double zScale)
{
    Transformation.ScaleTransform.xScale *= xScale;
    Transformation.ScaleTransform.yScale *= yScale;
    Transformation.ScaleTransform.zScale *= zScale;
}

void Canvas::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    // Рисуем здесь оси, потому что иначе будет неверное определен размер окна, некуда больше это впихнуть
    if (objectsArray.empty())
        addAxes();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    DefaultProjectionStrategy strategy;
    for (const auto &curObject : objectsArray) {
        curObject->draw(painter, strategy, size());
    }
}
