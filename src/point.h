#ifndef POINT_H
#define POINT_H

#include "abstractobject.h"

#include <QColor>

class Point : public AbstractObject
{
public:
    Point(double curX, double curY, double curZ, QColor curColor = Qt::white);
    ~Point() = default;

    void draw(QPainter &painter, const ProjectionStrategy &strategy, QSize canvasSize) override;
    void rotate(double angleX, double angleY, double angleZ) override;

    double getX() const {
        return x;
    }
    double getY() const {
        return y;
    }
    double getZ() const {
        return z;
    }

protected:
    void xRotate(Point &point, double angle);
    void yRotate(Point &point, double angle);
    void zRotate(Point &point, double angle);
private:
    double x;
    double y;
    double z;
    QColor color;
};

#endif // POINT_H
