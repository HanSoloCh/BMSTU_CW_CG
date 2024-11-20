#include "carcasmodel.h"

#include "drawvisitor.h"
#include "qdebug.h"

CarcasModel::CarcasModel(const QVector<Triangle> &triangles, const QColor &color)
    : AbstractModel(color)
    , triangles_(triangles) {}

CarcasModel::CarcasModel(const QVector<Point> &points,
                         const QVector<QVector<int>> &links,
                         const QColor &color)
    : AbstractModel(color) {
    for (int i = 0; i < links.size(); ++i) {
        QVector<int> link = links[i];
        triangles_.push_back(Triangle(points[link[0]],
                                      points[link[1]],
                                      points[link[2]]));
    }
}

void CarcasModel::Accept(BaseDrawVisitor &visitor) const {
    visitor.Visit(*this);
}


CarcasModel GenerateShape(double radius, int slices, int stacks, const QColor &color, const Point &center)     // Центр сферы
{
    QVector<Triangle> triangles;

    for (int i = 0; i < stacks; ++i) {
        double phi1 = M_PI * i / stacks;       // Текущая широта
        double phi2 = M_PI * (i + 1) / stacks; // Следующая широта

        for (int j = 0; j < slices; ++j) {
            double theta1 = 2 * M_PI * j / slices;       // Текущая долгота
            double theta2 = 2 * M_PI * (j + 1) / slices; // Следующая долгота

            // Четыре точки на текущем "квадрате"
            Point p1(center.x() + radius * sin(phi1) * cos(theta1),
                     center.y() + radius * sin(phi1) * sin(theta1),
                     center.z() + radius * cos(phi1));

            Point p2(center.x() + radius * sin(phi1) * cos(theta2),
                     center.y() + radius * sin(phi1) * sin(theta2),
                     center.z() + radius * cos(phi1));

            Point p3(center.x() + radius * sin(phi2) * cos(theta1),
                     center.y() + radius * sin(phi2) * sin(theta1),
                     center.z() + radius * cos(phi2));

            Point p4(center.x() + radius * sin(phi2) * cos(theta2),
                     center.y() + radius * sin(phi2) * sin(theta2),
                     center.z() + radius * cos(phi2));

            // Верхний треугольник
            triangles.push_back(Triangle(QVector<Point>{p1, p2, p4}, color));

            // Нижний треугольник
            triangles.push_back(Triangle(QVector<Point>{p1, p4, p3}, color));
        }
    }

    return CarcasModel(triangles, color);
}
