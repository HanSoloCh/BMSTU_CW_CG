#include "carcasmodel.h"

#include "drawvisitor.h"

#include <qdebug.h>

[[maybe_unused]] static uint qHash(const Point &point, uint seed) {
    Q_UNUSED(seed);
    size_t hash_x = std::hash<double>()(point.x());
    size_t hash_y = std::hash<double>()(point.y());
    size_t hash_z = std::hash<double>()(point.z());
    return hash_x ^ (hash_y << 1) ^ (hash_z << 2);
}


CarcasModel::CarcasModel(const QVector<Triangle> &triangles, const QColor &color)
    : AbstractModel(color)
    , triangles_(triangles) {
    QHash<Point, QVector<QVector3D>> points_normals;
    for (int i = 0; i < triangles.size(); ++i) {
        Triangle triangle = triangles[i];
        QVector3D normal = triangle.CalculateNormal();
        for (const auto &point : triangle.GetPoints()) {
            points_normals[point].push_back(normal);
        }
    }
    for (auto it = points_normals.constBegin(); it != points_normals.constEnd(); ++it) {
        for (const auto &normal : it.value()) {
            normals_of_points_[it.key()] += normal;
        }
        normals_of_points_[it.key()] /= it.value().size();
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
            triangles.push_back(Triangle({p1, p2, p4}, color));

            // Нижний треугольник
            triangles.push_back(Triangle({p1, p4, p3}, color));
        }
    }

    return CarcasModel(triangles, color);
}
QVector<Triangle> CarcasModel::GetTriangles() const noexcept { return triangles_; }
