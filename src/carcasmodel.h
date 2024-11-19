#ifndef CARCASMODEL_H
#define CARCASMODEL_H

#include "triangle.h"

class CarcasModel {
public:
    explicit CarcasModel(const QVector<Triangle> &triangles);
    CarcasModel(const QVector<Point> &points, const QVector<QVector<int>> &links, const QVector<QColor> &colors);

    QVector<Triangle> GetTriangles() const {
        return triangles_;
    }

private:
    QVector<Triangle> triangles_;
};

#endif // CARCASMODEL_H
