#include "carcasmodel.h"

#include "drawvisitor.h"

[[maybe_unused]] static uint qHash(const Point &point, uint seed) {
    Q_UNUSED(seed);
    size_t hash_x = std::hash<double>()(point.x());
    size_t hash_y = std::hash<double>()(point.y());
    size_t hash_z = std::hash<double>()(point.z());
    return hash_x ^ (hash_y << 1) ^ (hash_z << 2);
}

CarcasModel::CarcasModel(const QVector<Point> &points,
                         const QVector<std::array<int, 3>> &triangles,
                         const QColor &color)
    : AbstractModel(color)
    , points_(points)
    , triangles_(triangles)
    , points_normals_(points.size()) {
    QVector<QVector<QVector3D>> points_normals(points.size());

    for (const auto &triangle : triangles) {
        QVector3D normal = Triangle(GetTrianglePoints(triangle), color).CalculateNormal();

        for (int point_index : triangle) {
            points_normals[point_index].push_back(normal);
        }
    }

    for (int i = 0; i < points_normals.size(); ++i) {
        for (const auto &normal : points_normals[i]) {
            points_normals_[i] += normal;
        }
        points_normals_[i] = (points_normals_[i] / points_normals[i].size()).normalized();
    }
}

QVector<Point> CarcasModel::GetPoints() const noexcept { return points_; }

QVector<std::array<int, 3>> CarcasModel::GetTriangles() const noexcept { return triangles_; }

std::array<QVector3D, 3> CarcasModel::GetNormals(const std::array<int, 3> &triangle) const {
    return {points_normals_[triangle[0]], points_normals_[triangle[1]], points_normals_[triangle[2]]};
}

std::array<Point, 3> CarcasModel::GetTrianglePoints(const std::array<int, 3> &triangle) const noexcept {
    return {points_[triangle[0]], points_[triangle[1]], points_[triangle[2]]};
}

void CarcasModel::Accept(BaseDrawVisitor &visitor) const {
    visitor.Visit(*this);
}

void CarcasModel::Transform(const QMatrix4x4 &transform_matrix) {
    for (auto &point : points_) {
        point.Transform(transform_matrix);
    }
}

CarcasModel GenerateShape(double radius, int slices, int stacks, const QColor &color, const Point &center) {
    QVector<Point> points;
    QVector<std::array<int, 3>> triangles;

    auto CloseToZero = [](double num) {
        return std::abs(num) < 1e-9 ? 0 : num;
    };

    // Генерация точек
    points.push_back(Point(center.x(), center.y(), center.z() + radius)); // Верхний полюс
    for (int i = 1; i < stacks; ++i) { // Исключаем полюса
        double phi = M_PI * i / stacks;
        for (int j = 0; j < slices; ++j) { // Убираем точку для угла 2*pi
            double theta = 2 * M_PI * j / slices;
            points.push_back(Point(CloseToZero(center.x() + radius * sin(phi) * cos(theta)),
                                   CloseToZero(center.y() + radius * sin(phi) * sin(theta)),
                                   CloseToZero(center.z() + radius * cos(phi))));
        }
    }
    points.push_back(Point(center.x(), center.y(), center.z() - radius)); // Нижний полюс

    // Генерация треугольников
    // Соединяем верхний полюс с первым рядом
    for (int j = 0; j < slices; ++j) {
        int next = (j + 1) % slices; // Замыкаем первый ряд
        triangles.push_back({0, j + 1, next + 1});
    }

    // Соединяем между слоями
    for (int i = 0; i < stacks - 2; ++i) {
        for (int j = 0; j < slices; ++j) {
            int current = 1 + i * slices + j;
            int next = current + slices;

            int nextCurrentLayer = (j + 1) % slices + i * slices + 1;
            int nextNextLayer = (j + 1) % slices + (i + 1) * slices + 1;

            triangles.push_back({current, next, nextCurrentLayer});
            triangles.push_back({nextCurrentLayer, next, nextNextLayer});
        }
    }

    // Соединяем нижний полюс с последним рядом
    int bottomPoleIndex = points.size() - 1;
    int lastLayerStart = bottomPoleIndex - slices;
    for (int j = 0; j < slices; ++j) {
        int next = (j + 1) % slices; // Замыкаем последний ряд
        triangles.push_back({bottomPoleIndex, lastLayerStart + next, lastLayerStart + j});
    }

    return CarcasModel(points, triangles, color);
}

