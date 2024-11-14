#include "facade.h"

CanvasFacade::CanvasFacade()
    : canvas(std::make_unique<Canvas>())
{}

Canvas *CanvasFacade::getCanvas() const
{
    return canvas.get();
}

void CanvasFacade::update()
{
    canvas->update();
}

void CanvasFacade::addLine(Line *line)
{
    canvas->addObject(line);
}

void CanvasFacade::addLine(const Point &start, const Point &end, QColor color)
{
    Line *line = new Line(qMakePair(start, end), color);
    canvas->addObject(line);
}

void CanvasFacade::addLine(double x1, double y1, double z1, double x2, double y2, double z2, QColor color)
{
    Point start{x1, y1, z1};
    Point end{x2, y2, z2};
    Line *line = new Line(qMakePair(start, end), color);
    canvas->addObject(line);
}

void CanvasFacade::rotate(double xAngle, double yAngle, double zAngle)
{
    canvas->rotate(xAngle, yAngle, zAngle);
    canvas->update();
}
