#include "triangle.h"

#include "drawvisitor.h"

QColor IntensityColor(const QColor &color, double intensity) {
    if (intensity < 0) intensity = 0;
    if (intensity > 255) intensity = 255;

    QColor newColor(
        (color.red() * intensity) / 255,
        (color.green() * intensity) / 255,
        (color.blue() * intensity) / 255
        );
    return newColor;
}

Triangle::Triangle(const Point &p1, const Point &p2, const Point &p3, const QColor &color)
    : AbstractModel(color)
    , points_({p1, p2, p3}) {}

Triangle::Triangle(const QVector<Point> &points, const QColor &color)
    : AbstractModel(color)
    , points_(points) {}

void Triangle::Accept(BaseDrawVisitor &visitor) const {
    visitor.Visit(*this);
}
