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
                         const QColor &color,
                         QVector<QVector2D> uv_coords)
    : AbstractModel(color)
    , points_(points)
    , triangles_(triangles)
    , points_normals_(points.size())
    , uv_coords_(uv_coords) {
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

std::array<QVector2D, 3> CarcasModel::GetTriangleUV(const std::array<int, 3> &triangle) const noexcept {
    return {uv_coords_[triangle[0]], uv_coords_[triangle[1]], uv_coords_[triangle[2]]};
}

void CarcasModel::Accept(BaseDrawVisitor *visitor) const {
    visitor->Visit(*this);
}

void CarcasModel::Transform(const QMatrix4x4 &transform_matrix) {
    for (auto &point : points_) {
        point.Transform(transform_matrix);
    }
}

CarcasModel GenerateCarcasModelFromCurve(const QVector<QPointF> &curve,
                                         const axis_t rotation_axis,
                                         int segments,
                                         const QColor &color) {
    QVector<Point> points;
    QVector<std::array<int, 3>> triangles;

    // Шаг 1. Вычисляем длины сегментов кривой и общую длину
    QVector<double> segmentLengths;
    double totalLength = 0.0;
    for (int i = 0; i < curve.size() - 1; ++i) {
        double dx = curve[i + 1].x() - curve[i].x();
        double dy = curve[i + 1].y() - curve[i].y();
        double length = std::sqrt(dx * dx + dy * dy);
        segmentLengths.append(length);
        totalLength += length;
    }

    // Шаг 2. Генерируем точки и UV-координаты
    QVector<QVector2D> uvCoords;
    double angleStep = 2 * M_PI / segments;

    for (int j = 0; j < curve.size(); ++j) {
        // Рассчитываем V для текущей точки кривой
        double curveLengthToPoint = 0.0;
        for (int k = 0; k < j; ++k) {
            curveLengthToPoint += segmentLengths[k];
        }
        double V = curveLengthToPoint / totalLength;

        for (int i = 0; i < segments; ++i) {
            double angle = i * angleStep;
            double x = 0.0, y = 0.0, z = 0.0;

            if (rotation_axis == Ox) {
                x = curve[j].x();
                y = curve[j].y();
                z = 0;
            } else if (rotation_axis == Oy) {
                y = curve[j].x();
                x = curve[j].y();
                z = 0;
            }

            // Координаты вращения
            double rotatedY = y * cos(angle) - z * sin(angle);
            double rotatedZ = y * sin(angle) + z * cos(angle);
            points.append(Point(x, rotatedY, rotatedZ));

            // Рассчитываем U как долю угла
            double U = static_cast<double>(i) / segments;
            uvCoords.append(QVector2D(U, V));
        }
    }

    // Шаг 3. Генерируем треугольники
    int curvePointCount = curve.size();
    for (int j = 0; j < curvePointCount - 1; ++j) {
        int baseIndex = j * segments;
        int nextBaseIndex = baseIndex + segments;

        for (int i = 0; i < segments; ++i) {
            int current = baseIndex + i;
            int next = baseIndex + (i + 1) % segments;
            int nextLayerCurrent = nextBaseIndex + i;
            int nextLayerNext = nextBaseIndex + (i + 1) % segments;

            triangles.push_back({current, nextLayerCurrent, next});
            triangles.push_back({next, nextLayerCurrent, nextLayerNext});
        }
    }
    // Шаг 4. Возвращаем каркас с UV
    return CarcasModel(points, triangles, color, uvCoords);
}
