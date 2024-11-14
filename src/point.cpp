#include "point.h"

Point::Point(double curX, double curY, double curZ, QColor curColor): x(curX), y(curY), z(curZ), color(curColor)
{}

void Point::draw(QPainter &painter, const ProjectionStrategy &strategy, QSize canvasSize)
{
    painter.setPen(QPen(color, 2));
    QPointF point = strategy.project(*this, canvasSize);
    painter.drawPoint(point);
}

void Point::rotate(double angleX, double angleY, double angleZ)
{
    xRotate(*this, angleX);
    yRotate(*this, angleY);
    zRotate(*this, angleZ);
}

void Point::xRotate(Point &point, double angle)
{
    double radianAngle = qDegreesToRadians(angle);
    double cosAng = cos(radianAngle);
    double sinAng = sin(radianAngle);

    double yTmp = point.y;
    double zTmp = point.z;

    point.y = yTmp * cosAng - zTmp * sinAng;
    point.z = yTmp * sinAng + zTmp * cosAng;
}

void Point::yRotate(Point &point, double angle)
{
    double radianAngle = qDegreesToRadians(angle);
    double cosAng = cos(radianAngle);
    double sinAng = sin(radianAngle);

    double xTmp = point.x;
    double zTmp = point.z;

    point.x = xTmp * cosAng + zTmp * sinAng;
    point.z = -xTmp * sinAng + zTmp * cosAng;

}

void Point::zRotate(Point &point, double angle)
{
    double radianAngle = qDegreesToRadians(angle);
    double cosAng = cos(radianAngle);
    double sinAng = sin(radianAngle);

    double xTmp = point.x;
    double yTmp = point.y;

    point.x = xTmp * cosAng - yTmp * sinAng;
    point.y = xTmp * sinAng + yTmp * cosAng;
}

