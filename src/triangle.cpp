#include "triangle.h"

#include "drawvisitor.h"


Triangle::Triangle(const Point &p0, const Point &p1, const Point &p2, const QColor &color)
    : AbstractModel(color)
    , points_({p0, p1, p2}) {
    QVector3D normal = CalculateNormal();
    points_normals_ = {normal, normal, normal};
    CalculateUVCoords();
}

Triangle::Triangle(const std::array<Point, 3> &points, const QColor &color)
    : AbstractModel(color)
    , points_(points) {
    QVector3D normal = CalculateNormal();
    points_normals_ = {normal, normal, normal};
    CalculateUVCoords();
}

Triangle::Triangle(const Point points[3], const QColor &color)
    : AbstractModel(color)
    , points_({points[0], points[1], points[2]}) {
    QVector3D normal = CalculateNormal();
    points_normals_ = {normal, normal, normal};
    CalculateUVCoords();
}

Triangle::Triangle(const std::array<Point, 3> &points,
                   const std::array<QVector3D, 3> &points_normals,
                   const QColor &color)
    : AbstractModel(color)
    , points_(points)
    , points_normals_(points_normals) {
    CalculateUVCoords();
}


Triangle::Triangle(const Point points[3], const QVector3D points_normals[3], const QColor &color)
    : AbstractModel(color)
    , points_({points[0], points[1], points[2]})
    , points_normals_({points_normals[0], points_normals[1], points_normals[2]}) {
    CalculateUVCoords();
}

void Triangle::Accept(BaseDrawVisitor *visitor) const {
    visitor->Visit(*this);
}

void Triangle::Transform(const QMatrix4x4 &transform_matrix) {
    for (auto &point : points_) {
        point.Transform(transform_matrix);
    }
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
    QVector3D p10 = (*this)[1] - (*this)[0];
    QVector3D p20 = (*this)[2] - (*this)[0];
    return QVector3D::normal(p10, p20);
}

void Triangle::CalculateUVCoords() {
    double xmin = GetMinX();
    double xmax = GetMaxX();
    double ymin = GetMinY();
    double ymax = GetMaxY();

    for (int i = 0; i < 3; ++i) {
        uv_coords_[i] = QVector2D(((*this)[i].x() - xmin) / (xmax - xmin), ((*this)[i].y() - ymin) / (ymax - ymin));
    }
}

std::array<Point, 3> Triangle::GetPoints() const noexcept { return points_; }

std::array<QVector3D, 3> Triangle::GetNormals() const noexcept { return points_normals_; }

std::array<QVector2D, 3> Triangle::GetUVCoords() const noexcept {
    return uv_coords_;
}

void Triangle::SetUVCoords(const std::array<QVector2D, 3> &uv_coords) {
    uv_coords_ = uv_coords;
}

void Triangle::SetPoints(const std::array<Point, 3> &points) {
    points_ = points;
}

