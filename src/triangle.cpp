#include "triangle.h"

#include "drawvisitor.h"


Triangle::Triangle(const Point &p1, const Point &p2, const Point &p3, const QColor &color)
    : AbstractModel(color)
    , points_({p1, p2, p3}) {}

Triangle::Triangle(const QVector<Point> &points, const QColor &color)
    : AbstractModel(color)
    , points_(points) {}

void Triangle::Accept(BaseDrawVisitor &visitor) const {
    visitor.Visit(*this);
}

double Triangle::GetMaxX() const {
    return std::max({points_[0].x(), points_[1].x(), points_[2].x()});
}

double Triangle::GetMaxY() const {
    return std::max({points_[0].y(), points_[1].y(), points_[2].y()});
}

double Triangle::GetMaxZ() const {
    return std::max({points_[0].z(), points_[1].z(), points_[2].z()});
}

double Triangle::GetMinX() const {
    return std::min({points_[0].x(), points_[1].x(), points_[2].x()});
}

double Triangle::GetMinY() const {
    return std::min({points_[0].y(), points_[1].y(), points_[2].y()});
}

double Triangle::GetMinZ() const {
    return std::min({points_[0].z(), points_[1].z(), points_[2].z()});
}

bool Triangle::IsContains(const Point &p0) {
    double denom = (points_[1].y() - points_[2].y()) * (points_[0].x() - points_[2].x()) + (points_[2].x() - points_[1].x()) * (points_[0].y() - points_[2].y());
    double alpha = ((points_[1].y() - points_[2].y()) * (p0.x() - points_[2].x()) + (points_[2].x() - points_[1].x()) * (p0.y() - points_[2].y())) / denom;
    double beta = ((points_[2].y() - points_[0].y()) * (p0.x() - points_[2].x()) + (points_[0].x() - points_[2].x()) * (p0.y() - points_[2].y())) / denom;
    double gamma = 1.0 - alpha - beta;

    return alpha >= 0 && beta >= 0 && gamma >= 0;
}
