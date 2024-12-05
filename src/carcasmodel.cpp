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
    , triangles_(triangles) {

    QHash<Point, QVector<QVector3D>> points_normals;

    for (const auto &triangle : triangles) {
        std::array<Point, 3> points = GetTrianglePoints(triangle);
        QVector3D normal = Triangle(points, color).CalculateNormal();

        for (const auto &point : points) {
            points_normals[point].push_back(normal);
        }
    }

    for (auto it = points_normals.constBegin(); it != points_normals.constEnd(); ++it) {
        for (const auto &normal : it.value()) {
            normals_[it.key()] += normal;
        }
        normals_[it.key()] = (normals_[it.key()] / it.value().size()).normalized();
    }
}

QVector<Point> CarcasModel::GetPoints() const noexcept { return points_; }

QVector<std::array<int, 3>> CarcasModel::GetTriangles() const noexcept { return triangles_; }

std::array<QVector3D, 3> CarcasModel::GetNormals(const std::array<int, 3> &triangle) const {
    std::array<Point, 3> points = GetTrianglePoints(triangle);
    return {normals_[points[0]], normals_[points[1]], normals_[points[2]]};
}

std::array<Point, 3> CarcasModel::GetTrianglePoints(const std::array<int, 3> &triangle) const noexcept {
    return {points_[triangle[0]], points_[triangle[1]], points_[triangle[2]]};
}

void CarcasModel::Accept(BaseDrawVisitor &visitor) const {
    visitor.Visit(*this);
}

double eps = 1e-9;

CarcasModel GenerateShape(double radius, int slices, int stacks, const QColor &color, const Point &center) {
    QVector<Point> points;
    QVector<std::array<int, 3>> triangles;

    auto CloseToZero = [](double num) {
        return std::abs(num) < eps ? 0 : num;
    };

    // Генерация точек
    points.push_back(Point(center.x(), center.y(), center.z() + radius)); // Верхний полюс
    for (int i = 1; i < stacks; ++i) { // Исключаем полюса
        double phi = M_PI * i / stacks;
        for (int j = 0; j <= slices; ++j) {
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
