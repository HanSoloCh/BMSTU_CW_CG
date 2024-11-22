#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QColor>

#include "point.h"
#include "abstractmodel.h"
#include "strategy.h"


class Triangle: public AbstractModel {
public:
    Triangle(const Point &p0, const Point &p1, const Point &p2, const QColor &color = Qt::white);
    explicit Triangle(const std::array<Point, 3> &points, const QColor &color = Qt::white);
    explicit Triangle(const Point points[3], const QColor &color = Qt::white);

    ~Triangle() = default;

    std::array<Point, 3> GetPoints() const noexcept;

    void Accept(BaseDrawVisitor &visitor) const override;

    static bool IsTriangle(const Point &p1, const Point &p2, const Point &p3) noexcept;
    static bool IsTriangle(const QVector<Point> &points) noexcept;

    double GetMaxX() const noexcept;
    double GetMaxY() const noexcept;
    double GetMaxZ() const noexcept;

    double GetMinX() const noexcept;
    double GetMinY() const noexcept;
    double GetMinZ() const noexcept;

    Point operator[](int i) noexcept;
    Point operator[](int i) const noexcept;

    QVector3D CalculateNormal() const noexcept;

    // Ignore coordinate Z. Supposed to be used for the designed triangle
    bool IsContains(const Point &p0);

    template <typename T>
    T InterpolateValue(const T &value_in_p0, const T &value_in_p1, const T &value_in_p2, const Point &cur_point);

private:
    std::array<Point, 3> points_;
};

template<typename T>
T Triangle::InterpolateValue(const T &value_in_p0, const T &value_in_p1, const T &value_in_p2, const Point &cur_point) {

    double denom = (points_[1].y() - points_[2].y()) * (points_[0].x() - points_[2].x()) + (points_[2].x() - points_[1].x()) * (points_[0].y() - points_[2].y());
    double alpha = ((points_[1].y() - points_[2].y()) * (cur_point.x() - points_[2].x()) + (points_[2].x() - points_[1].x()) * (cur_point.y() - points_[2].y())) / denom;
    double beta = ((points_[2].y() - points_[0].y()) * (cur_point.x() - points_[2].x()) + (points_[0].x() - points_[2].x()) * (cur_point.y() - points_[2].y())) / denom;
    double gamma = 1.0 - alpha - beta;

    return T(alpha * value_in_p0 + beta * value_in_p1 + gamma * value_in_p2);
}

#endif // TRIANGLE_H
