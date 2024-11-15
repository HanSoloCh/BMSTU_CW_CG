#ifndef POINT_H
#define POINT_H

#include "abstractobject.h"

#include <QColor>

class Point : public AbstractObject
{
public:
    Point(double curX, double curY, double curZ, QColor curColor = Qt::white, bool isConst = false);
    ~Point() = default;

    void draw(QPainter &painter, const ProjectionStrategy &strategy, QSize canvasSize) const override;

    void move(double xMove, double yMove, double zMove) override;
    void rotate(double xAngle, double yAngle, double zAngle) override;
    void scale(double xScale, double yScale, double zScale) override;

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
    double x, y, z;
};

#endif // POINT_H
