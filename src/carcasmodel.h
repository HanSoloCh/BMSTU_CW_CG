#ifndef CARCASMODEL_H
#define CARCASMODEL_H

#include <QColor>
#include <QHash>

#include "abstractmodel.h"
#include "point.h"


typedef enum {
    Ox = 1,
    Oy = 2,
} axis_t;

class CarcasModel : public AbstractModel {
public:
    explicit CarcasModel(const QVector<Point> &points,
                         const QVector<std::array<int, 3>> &triangles,
                         const QColor &color,
                         QVector<QVector2D> uv_coords);

    QVector<Point> GetPoints() const noexcept;
    QVector<std::array<int, 3>> GetTriangles() const noexcept;
    std::array<QVector3D, 3> GetNormals(const std::array<int, 3> &triangle) const;
    std::array<Point, 3> GetTrianglePoints(const std::array<int, 3> &triangle) const noexcept;

    std::array<QVector2D, 3> GetTriangleUV(const std::array<int, 3> &triangle) const noexcept;


    void Accept(BaseDrawVisitor *visitor) const override;

    void Transform(const QMatrix4x4 &transform_matrix) override;
private:
    QVector<Point> points_; // Вершины
    QVector<std::array<int, 3>> triangles_; // Индексы вершин

    QHash<Point, QVector3D> normals_;
    QVector<QVector3D> points_normals_;
    QVector<QVector2D> uv_coords_;
};


CarcasModel GenerateShape(double radius, int slices, int stacks, const QColor &color, const Point &center);
CarcasModel GenerateCarcasModelFromCurve(const QVector<QPointF> &curve,
                                         const axis_t rotation_axis,
                                         int segments = 30,
                                         const QColor &color = Qt::gray);
#endif // CARCASMODEL_H
