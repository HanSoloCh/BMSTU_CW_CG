#include "triangle.h"

Triangle::Triangle(const Point &curP1, const Point &curP2, const Point &curP3, const QColor &curColor)
    : p1(curP1)
    , p2(curP2)
    , p3(curP3)
{}

void Triangle::draw(QPainter &painter, const ProjectionStrategy &strategy, QSize canvasSize) const
{
    painter.setPen(QPen(color, 2));
    QPointF projectP1 = strategy.project(p1, canvasSize);
    QPointF projectP2 = strategy.project(p2, canvasSize);
    QPointF projectP3 = strategy.project(p3, canvasSize);

    painter.drawLine(projectP1, projectP2);
    painter.drawLine(projectP2, projectP3);
    painter.drawLine(projectP1, projectP3);
}

void Triangle::rotate(double angleX, double angleY, double angleZ)
{
    p1.rotate(angleX, angleY, angleZ);
    p2.rotate(angleX, angleY, angleZ);
    p3.rotate(angleX, angleY, angleZ);
}
