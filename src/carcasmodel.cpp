#include "carcasmodel.h"

#include "drawvisitor.h"

#include <qdebug.h>

CarcasModel::CarcasModel(const QVector<Point> &points,
                         const QVector<std::array<int, 3>> &triangles,
                         const QColor &color,
                         QVector<QColor> colors)
    : AbstractModel(color)
    , points_(points)
    , triangles_(triangles)
    , normals_(points.size(), QVector3D(0, 0, 0)) // Инициализация нормалей
    , colors_(colors)
{
    QHash<int, QVector<QVector3D>> point_normals;

    for (const auto &triangle : triangles) {
        QVector3D normal = Triangle({points_[triangle[0]],
                                     points_[triangle[1]],
                                     points_[triangle[2]]}, color).CalculateNormal();
        qDebug() << points_[triangle[0]] << points_[triangle[1]] << points_[triangle[2]];
        for (int index : triangle) {
            point_normals[index].push_back(normal);
        }
    }

    for (auto it = point_normals.constBegin(); it != point_normals.constEnd(); ++it) {
        for (const auto &normal : it.value()) {
            normals_[it.key()] += normal;
        }
        normals_[it.key()] /= it.value().size();
    }
}

QVector<Point> CarcasModel::GetPoints() const noexcept { return points_; }

QVector<std::array<int, 3>> CarcasModel::GetTriangles() const noexcept { return triangles_; }

std::array<QVector3D, 3> CarcasModel::GetNormals(const std::array<int, 3> &triangle) const {
    return {normals_[triangle[0]], normals_[triangle[1]], normals_[triangle[2]]};
}

std::array<Point, 3> CarcasModel::GetTrianglePoints(std::array<int, 3> triangle) const noexcept {
    return {points_[triangle[0]], points_[triangle[1]], points_[triangle[2]]};
}

void CarcasModel::Accept(BaseDrawVisitor &visitor) const {
    visitor.Visit(*this);
}

CarcasModel GenerateShape(double radius, int slices, int stacks, const QColor &color, const Point &center) {
    QVector<Point> points;
    QVector<std::array<int, 3>> triangles;

    // Генерация точек
    points.push_back(Point(center.x(), center.y(), center.z() + radius)); // Верхний полюс
    for (int i = 1; i < stacks; ++i) { // Исключаем полюса
        double phi = M_PI * i / stacks;
        for (int j = 0; j <= slices; ++j) {
            double theta = 2 * M_PI * j / slices;
            points.push_back(Point(center.x() + radius * sin(phi) * cos(theta),
                                   center.y() + radius * sin(phi) * sin(theta),
                                   center.z() + radius * cos(phi)));
        }
    }
    points.push_back(Point(center.x(), center.y(), center.z() - radius)); // Нижний полюс

    // Генерация треугольников
    // Соединяем верхний полюс с первым рядом
    for (int j = 0; j < slices; ++j) {
        triangles.push_back({0, j + 1, j + 2});
    }

    // Соединяем между слоями
    for (int i = 0; i < stacks - 2; ++i) {
        for (int j = 0; j < slices; ++j) {
            int current = 1 + i * (slices + 1) + j;
            int next = current + slices + 1;

            triangles.push_back({current, next, current + 1});
            triangles.push_back({current + 1, next, next + 1});
        }
    }

    // Соединяем нижний полюс с последним рядом
    int bottomPoleIndex = points.size() - 1;
    int lastLayerStart = bottomPoleIndex - (slices + 1);
    for (int j = 0; j < slices; ++j) {
        triangles.push_back({bottomPoleIndex, lastLayerStart + j + 1, lastLayerStart + j});
    }

    return CarcasModel(points, triangles, color);
}


// CarcasModel GenerateShape() {
//     QVector<Point> points = {{-0.5, 1, 8},
//                              {-2.5, 1, 8},
//                              {-2.5, -1, 8},
//                              {-0.5, -1, 8},
//                              {-0.5, 1, 6},
//                              {-2.5, 1, 6},
//                              {-2.5, -1, 6},
//                              {-0.5, -1, 6}};
//     QVector<std::array<int, 3>> triangles = {{0, 1, 2},
//                                               {0, 2, 3},
//                                               {4, 0, 3},
//                                               {4, 3, 7},
//                                               {5, 4, 7},
//                                               {5, 7, 6},
//                                               {1, 5, 6},
//                                               {1, 6, 2},
//                                               {4, 5, 1},
//                                               {4, 1, 0},
//                                               {2, 6, 7},
//                                              {2, 7, 3}};
//     QVector<QColor> colors = {Qt::red,
//                                Qt::red,
//                                Qt::green,
//                                Qt::green,
//                                Qt::blue,
//                                Qt::blue,
//                                Qt::yellow,
//                                Qt::yellow,
//                                Qt::white,
//                                Qt::white,
//                                Qt::cyan,
//                               Qt::cyan};
//     return CarcasModel(points, triangles, Qt::green, colors);
// }
