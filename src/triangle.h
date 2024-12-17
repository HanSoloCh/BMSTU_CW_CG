#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QColor>

#include "abstractmodel.h"
#include "point.h"
#include "strategy.h"

class Triangle : public AbstractModel {
   public:
    Triangle(const Point &p0, const Point &p1, const Point &p2,
             const QColor &color = Qt::gray);

    explicit Triangle(const std::array<Point, 3> &points,
                      const QColor &color = Qt::gray);

    explicit Triangle(const Point points[3], const QColor &color = Qt::gray);

    Triangle(const std::array<Point, 3> &points,
             const std::array<QVector3D, 3> &points_normals,
             const QColor &color = Qt::gray);

    Triangle(const Point points[3], const QVector3D points_normals[3],
             const QColor &color = Qt::gray);

    ~Triangle() = default;

    std::array<Point, 3> GetPoints() const noexcept;
    std::array<QVector3D, 3> GetNormals() const noexcept;
    std::array<QVector2D, 3> GetUVCoords() const noexcept;

    void SetUVCoords(const std::array<QVector2D, 3> &uv_coords);
    void SetPoints(const std::array<Point, 3> &points);

    void Accept(BaseDrawVisitor *visitor) const override;

    void Transform(const QMatrix4x4 &transform_matrix) override;

    static bool IsTriangle(const Point &p1, const Point &p2,
                           const Point &p3) noexcept;
    static bool IsTriangle(const QVector<Point> &points) noexcept;

    double GetMaxX() const noexcept;
    double GetMaxY() const noexcept;
    double GetMaxZ() const noexcept;

    double GetMinX() const noexcept;
    double GetMinY() const noexcept;
    double GetMinZ() const noexcept;

    Point operator[](int i) const noexcept;

    QVector3D CalculateNormal() const noexcept;

   private:
    void CalculateUVCoords();
    std::array<Point, 3> points_;
    std::array<QVector3D, 3> points_normals_;
    std::array<QVector2D, 3> uv_coords_;
};

#endif  // TRIANGLE_H
