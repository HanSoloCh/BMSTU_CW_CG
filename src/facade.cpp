#include "facade.h"

CanvasFacade::CanvasFacade()
    : canvas(std::make_unique<Canvas>())
{}

Canvas *CanvasFacade::getCanvas() const
{
    return canvas.get();
}


void CanvasFacade::addLine(Line *line)
{
    std::shared_ptr<Line> addLine = std::shared_ptr<Line>(line);
    canvas->addObject(addLine);
    canvas->update();
}

void CanvasFacade::addLine(const Point &start, const Point &end, QColor color)
{
    std::shared_ptr<Line> line = std::make_shared<Line>(qMakePair(start, end), color);
    canvas->addObject(line);
    canvas->update();
}

void CanvasFacade::addLine(double x1, double y1, double z1, double x2, double y2, double z2, QColor color)
{
    Point start{x1, y1, z1};
    Point end{x2, y2, z2};
    std::shared_ptr<Line> line = std::make_shared<Line>(qMakePair(start, end), color);
    formings.push_back(line);
    canvas->addObject(line);
    canvas->update();
}

void CanvasFacade::move(double xMove, double yMove, double zMove)
{
    canvas->move(xMove, yMove, zMove);
    canvas->update();
}

void CanvasFacade::rotate(double xAngle, double yAngle, double zAngle)
{
    canvas->rotate(xAngle, yAngle, zAngle);
    canvas->update();
}

void CanvasFacade::scale(double xScale, double yScale, double zScale)
{
    canvas->scale(xScale, yScale, zScale);
    canvas->update();
}

void CanvasFacade::makeRotatinShape()
{
    std::shared_ptr<RotationShape> shape = std::make_shared<RotationShape>(*(formings[0]));
    canvas->addObject(shape);
    canvas->update();
}

void CanvasFacade::deleteAll()
{
    canvas->deleteAll();
    canvas->update();
}
