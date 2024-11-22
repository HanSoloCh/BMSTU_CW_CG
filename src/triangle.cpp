#include "triangle.h"

#include "drawvisitor.h"


Triangle::Triangle(const Point &p0, const Point &p1, const Point &p2, const QColor &color)
    : AbstractModel(color)
    , points_{p0, p1, p2} {
}

Triangle::Triangle(const std::array<Point, 3> &points, const QColor &color)
    : AbstractModel(color)
    , points_(points) {
}

Triangle::Triangle(const Point points[3], const QColor &color)
    : AbstractModel(color)
    , points_({points[0], points[1], points[2]}) {
}

void Triangle::Accept(BaseDrawVisitor &visitor) const {
    visitor.Visit(*this);
}

double Triangle::GetMaxX() const noexcept {
    return std::max({points_[0].x(), points_[1].x(), points_[2].x()});
}

double Triangle::GetMaxY() const noexcept {
    return std::max({points_[0].y(), points_[1].y(), points_[2].y()});
}

double Triangle::GetMaxZ() const noexcept {
    return std::max({points_[0].z(), points_[1].z(), points_[2].z()});
}

double Triangle::GetMinX() const noexcept {
    return std::min({points_[0].x(), points_[1].x(), points_[2].x()});
}

double Triangle::GetMinY() const noexcept {
    return std::min({points_[0].y(), points_[1].y(), points_[2].y()});
}

double Triangle::GetMinZ() const noexcept {
    return std::min({points_[0].z(), points_[1].z(), points_[2].z()});
}

Point Triangle::operator[](int i) noexcept {
    return points_[i];
}

Point Triangle::operator[](int i) const noexcept {
    return points_[i];
}

QVector3D Triangle::CalculateNormal() const noexcept {
    std::array<Point, 3> points = GetPoints();
    return QVector3D::normal(points[2] - points[0], points[1] - points[0]);
}

bool Triangle::IsContains(const Point &p0) {
    double denom = (points_[1].y() - points_[2].y()) * (points_[0].x() - points_[2].x()) + (points_[2].x() - points_[1].x()) * (points_[0].y() - points_[2].y());
    double alpha = ((points_[1].y() - points_[2].y()) * (p0.x() - points_[2].x()) + (points_[2].x() - points_[1].x()) * (p0.y() - points_[2].y())) / denom;
    double beta = ((points_[2].y() - points_[0].y()) * (p0.x() - points_[2].x()) + (points_[0].x() - points_[2].x()) * (p0.y() - points_[2].y())) / denom;
    double gamma = 1.0 - alpha - beta;

    return alpha >= 0 && beta >= 0 && gamma >= 0;
}

std::array<Point, 3> Triangle::GetPoints() const noexcept { return points_; }
