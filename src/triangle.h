#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "abstractobject.h"

#include "point.h"

class Triangle : public AbstractObject
{
public:
    Triangle(const Point &curP1, const Point &curP2, const Point &curP3, const QColor &curColor);
    ~Triangle() = default;

    void draw(QPainter &painter, const ProjectionStrategy &strategy, QSize canvasSize) override;
    void rotate(double angleX, double angleY, double angleZ) override;

private:
    Point p1;
    Point p2;
    Point p3;
    QColor color;
};

#endif // TRIANGLE_H
