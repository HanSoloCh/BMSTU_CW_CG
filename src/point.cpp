#include "point.h"

#include <cmath>

#include "drawvisitor.h"

Point::Point(double x, double y, double z, const QColor &color)
    : AbstractModel(color)
    , QVector3D(x, y, z)
{}

Point::Point(QPoint p, double z, const QColor &color)
    : AbstractModel(color)
    , QVector3D(p.x(), p.y(), z) {}

Point::Point(QPointF p, double z, const QColor &color)
    : AbstractModel(color)
    , QVector3D(p.x(), p.y(), z) {}

Point::Point(const QVector3D &vector, const QColor &color)
    : AbstractModel(color)
    , QVector3D(vector) {}

void Point::Accept(BaseDrawVisitor &visitor) const {
    visitor.Visit(*this);
}

void Point::Transform(const QMatrix4x4 &transform_matrix) {
    QVector3D::operator=(transform_matrix.map(*this));
}

bool Point::operator==(const Point &p) const noexcept {
    double epsilon = std::numeric_limits<double>::epsilon();
    return std::fabs(x() - p.x()) < epsilon &&
           std::fabs(y() - p.y()) < epsilon &&
           std::fabs(z() - p.z()) < epsilon;
}



