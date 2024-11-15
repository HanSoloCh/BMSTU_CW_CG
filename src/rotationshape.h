#ifndef ROTATIONSHAPE_H
#define ROTATIONSHAPE_H

#include "abstractobject.h"

#include "line.h"
#include "triangle.h"

class RotationShape : public AbstractObject
{
public:
    RotationShape(const Line &forming, QColor color = Qt::white, bool isConst = false);
    ~RotationShape() = default;

    void draw(QPainter &painter, const ProjectionStrategy &strategy, QSize canvasSize) const override;

    void move(double xMove, double yMove, double zMove) override;
    void rotate(double xAngle, double yAngle, double zAngle) override;
    void scale(double xScale, double yScale, double zScale) override;

private:
    // QVector<Triangle> poligonArray;

    std::vector<std::vector<Point>> surface;
};

#endif // ROTATIONSHAPE_H
