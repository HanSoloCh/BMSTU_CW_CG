#ifndef FACADE_H
#define FACADE_H

#include <QObject>
#include <QPointF>
#include <memory>

#include "canvas.h"
#include "line.h"

class CanvasFacade
{
public:
    explicit CanvasFacade();
    virtual ~CanvasFacade() = default;

    Canvas *getCanvas() const;
    void update();

    void addLine(Line *line);
    void addLine(const Point &start, const Point &end, QColor color = Qt::black);
    void addLine(double x1, double y1, double z1, double x2, double y2, double z2, QColor color = Qt::black);

    void rotate(double xAngle, double yAngle, double zAngle);
    Line line;
private:
    std::unique_ptr<Canvas> canvas;
};


#endif // FACADE_H
