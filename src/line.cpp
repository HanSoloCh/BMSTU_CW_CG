#include "line.h"


Line::Line(QPair<Point, Point> curLine, QColor curColor, bool isConst):
    AbstractObject(curColor, isConst),
    line(curLine)
{}

Point Line::start() const
{
    return line.first;
}

Point Line::end() const
{
    return line.second;
}

void Line::draw(QPainter &painter, const ProjectionStrategy &strategy, QSize canvasSize) const
{
    painter.setPen(QPen(color, 2));
    QPointF startPoint = strategy.project(start(), canvasSize);
    QPointF endPoint = strategy.project(end(), canvasSize);
    painter.drawLine(startPoint, endPoint);

}

void Line::move(double xMove, double yMove, double zMove)
{
    start().move(xMove, yMove, zMove);
    end().move(xMove, yMove, zMove);
}

void Line::rotate(double xAngle, double yAngle, double zAngle)
{
    start().rotate(xAngle, yAngle, zAngle);
    end().rotate(xAngle, yAngle, zAngle);
}

void Line::scale(double xScale, double yScale, double zScale)
{
    start().scale(xScale, yScale, zScale);
    end().scale(xScale, yScale, zScale);
}

