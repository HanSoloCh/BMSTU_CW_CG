#ifndef FACADE_H
#define FACADE_H

#include <QObject>
#include <QPointF>
#include <memory>

#include "canvas.h"

#include "line.h"
#include "rotationshape.h"

class CanvasFacade
{
public:
    explicit CanvasFacade();
    virtual ~CanvasFacade() = default;

    Canvas *getCanvas() const;

    void addLine(Line *line);
    void addLine(const Point &start, const Point &end, QColor color = Qt::black);
    void addLine(double x1, double y1, double z1, double x2, double y2, double z2, QColor color = Qt::black);

    void move(double xMove, double yMove, double zMove);
    void rotate(double xAngle, double yAngle, double zAngle);
    void scale(double xScale, double yScale, double zScale);

    void makeRotatinShape();

    void deleteAll();
private:
    std::unique_ptr<Canvas> canvas;
    QVector<std::shared_ptr<Line>> formings;
};


#endif // FACADE_H
