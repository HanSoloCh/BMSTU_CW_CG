#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "abstractobject.h"

#include "point.h"

class Triangle : public AbstractObject
{
public:
    Triangle(const Point &curP1, const Point &curP2, const Point &curP3, const QColor &curColor);
    ~Triangle() = default;

    void draw(QPainter &painter, const ProjectionStrategy &strategy, QSize canvasSize) const override;

    // void move(double xMove, double yMove, double zMove) override;
    void rotate(double xAngle, double yAngle, double zAngle) override;
    // void scale(double xScale, double yScale, double zScale) override;

private:
    Point p1;
    Point p2;
    Point p3;
};

#endif // TRIANGLE_H
