#ifndef CARCASMODEL_H
#define CARCASMODEL_H

#include <QColor>

#include "abstractmodel.h"
#include "triangle.h"

class CarcasModel: public AbstractModel {
public:
    explicit CarcasModel(const QVector<Triangle> &triangles, const QColor &color = Qt::white);
    CarcasModel(const QVector<Point> &points, const QVector<QVector<int>> &links, const QColor &color = Qt::white);

    QVector<Triangle> GetTriangles() const {
        return triangles_;
    }

    void Accept(BaseDrawVisitor &visitor) const override;

private:
    QVector<Triangle> triangles_;
};


CarcasModel GenerateShape(double radius, int slices, int stacks, const QColor &color, const Point &center);

#endif // CARCASMODEL_H
