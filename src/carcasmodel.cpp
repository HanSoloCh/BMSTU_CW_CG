#include "carcasmodel.h"

CarcasModel::CarcasModel(const QVector<Triangle> &triangles)
    : triangles_(triangles) {}

CarcasModel::CarcasModel(const QVector<Point> &points,
                         const QVector<QVector<int>> &links,
                         const QVector<QColor> &colors) {
    for (int i = 0; i < links.size(); ++i) {
        QVector<int> link = links[i];
        triangles_.push_back(Triangle(points[link[0]],
                                      points[link[1]],
                                      points[link[2]],
                                      colors[i]));
    }
}
