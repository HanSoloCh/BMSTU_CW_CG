#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QColor>

#include "point.h"
#include "abstractmodel.h"
#include "strategy.h"


class Triangle: public AbstractModel {
public:
    Triangle(const Point &p0, const Point &p1, const Point &p2, const QColor &color = Qt::gray);

    Triangle(const std::array<Point, 3> &points,
             const QColor &color = Qt::gray);

    Triangle(const Point points[3],
             const QColor &color = Qt::gray);

    Triangle(const std::array<Point, 3> &points,
             const std::array<QVector3D, 3> &points_normals,
             const QColor &color = Qt::gray);

    Triangle(const Point points[3],
             const QVector3D points_normals[3],
             const QColor &color = Qt::gray);

    ~Triangle() = default;

    std::array<Point, 3> GetPoints() const noexcept;
    std::array<QVector3D, 3> GetNormals() const noexcept;

    void Accept(BaseDrawVisitor &visitor) const override;

    void Transform(const QMatrix4x4 &transform_matrix) override;

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
private:
    std::array<Point, 3> points_;
    std::array<QVector3D, 3> points_normals_;
};


#endif // TRIANGLE_H
