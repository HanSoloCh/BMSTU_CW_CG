#ifndef CARCASMODEL_H
#define CARCASMODEL_H

#include <QColor>

#include "abstractmodel.h"
#include "point.h"

class CarcasModel : public AbstractModel {
public:
    explicit CarcasModel(const QVector<Point> &points,
                         const QVector<std::array<int, 3>> &triangles,
                         const QColor &color);

    QVector<Point> GetPoints() const noexcept;
    QVector<std::array<int, 3>> GetTriangles() const noexcept;
    std::array<QVector3D, 3> GetNormals(const std::array<int, 3> &triangle) const;
    std::array<Point, 3> GetTrianglePoints(std::array<int, 3> triangle) const noexcept;

    void Accept(BaseDrawVisitor &visitor) const override;
    QVector<QColor> colors_;

private:
    QVector<Point> points_; // Вершины
    QVector<std::array<int, 3>> triangles_; // Индексы вершин

    QVector<QVector3D> normals_; // Нормали (соответственно индексам вершин)
};


CarcasModel GenerateShape(double radius, int slices, int stacks, const QColor &color, const Point &center);

#endif // CARCASMODEL_H
