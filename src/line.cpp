#include "line.h"


Line::Line(QPair<Point, Point> curLine, QColor curColor):
    line(curLine),
    color(curColor)
{}

void Line::draw(QPainter &painter, const ProjectionStrategy &strategy, QSize canvasSize)
{
    painter.setPen(QPen(color, 2));
    QPointF startPoint = strategy.project(line.first, canvasSize);
    QPointF endPoint = strategy.project(line.second, canvasSize);
    painter.drawLine(startPoint, endPoint);

}

void Line::rotate(double angleX, double angleY, double angleZ)
{
    line.first.rotate(angleX, angleY, angleZ);
    line.second.rotate(angleX, angleY, angleZ);
}

