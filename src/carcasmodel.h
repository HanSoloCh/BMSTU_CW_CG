#ifndef CARCASMODEL_H
#define CARCASMODEL_H

#include <QColor>
#include <QHash>

#include "abstractmodel.h"
#include "triangle.h"

class CarcasModel: public AbstractModel {
public:
    explicit CarcasModel(const QVector<Triangle> &triangles, const QColor &color = Qt::white);

    QVector<Triangle> GetTriangles() const noexcept;

    void Accept(BaseDrawVisitor &visitor) const override;

private:
    QVector<Triangle> triangles_;

    QHash<Point, QVector3D> normals_of_points_;
};


CarcasModel GenerateShape(double radius, int slices, int stacks, const QColor &color, const Point &center);

#endif // CARCASMODEL_H
