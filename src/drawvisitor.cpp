#include "drawvisitor.h"

#include <QVector4D>


BaseDrawVisitor::~BaseDrawVisitor() {}

BaseDrawVisitor::BaseDrawVisitor(QPainter *painter)
    : painter_(painter)
    , color_(painter->pen().color()) {}

DrawVisitor::DrawVisitor(QPainter *painter,
                         QSize canvas_size,
                         const AbstractStrategyProjection *projection,
                         QVector<std::shared_ptr<AbstractLight> > light)
    : BaseDrawVisitor(painter)
    , canvas_size_(canvas_size)
    , z_buffer_(canvas_size_.height(), QVector<double>(canvas_size_.width(), std::numeric_limits<double>::infinity()))
    , projection_(projection)
    , light_(light) {}


void DrawVisitor::Visit(const Point &point) {
    Point project_point = projection_->ProjectPoint(point, canvas_size_);

    SetColor(point.GetColor());
    DrawPoint(project_point);
    ResetColor();
}

void DrawVisitor::DrawPoint(const Point &point) {
    double x = point.x();
    double y = point.y();
    double z = 1 / point.z();
    if (z > z_buffer_[x][y]) {
        painter_->drawPoint(x, y);
        z_buffer_[x][y] = z;
    }
}


QColor IntensityColor(const QColor &color, int intensity) {
    if (intensity < 0) intensity = 0;
    if (intensity > 255) intensity = 255;

    QColor newColor(
        (color.red() * intensity) / 255,
        (color.green() * intensity) / 255,
        (color.blue() * intensity) / 255
        );
    return newColor;
}

void DrawVisitor::Visit(const Triangle &triangle) {
    std::array<Point, 3> points = triangle.GetPoints();

    for (auto &point : points) {
        point = projection_->ProjectPoint(point, {canvas_size_.width(), canvas_size_.width()});
    }
    DrawTriangle(points, triangle.GetNormals(), triangle.GetColor());
}

void DrawVisitor::DrawTriangle(const std::array<Point, 3> &pts, const std::array<QVector3D, 3> &normals, const QColor &color) {
    auto [bbox_min_x, bbox_min_y, bbox_max_x, bbox_max_y] = CalculateBoundingBox(pts);

    for (int x = bbox_min_x; x <= bbox_max_x; ++x) {
        for (int y = bbox_min_y; y <= bbox_max_y; ++y) {
            BarycentricCoords barycentric_coords;

            if (CalculateBarycentricCoords(pts, x, y, barycentric_coords) && IsInsideTriangle(barycentric_coords)) {

                double z = InterpolateValue<double>({pts[0].z(), pts[1].z(), pts[2].z()}, barycentric_coords);
                if (UpdateZBuffer(x, y, z)) {
                    QVector3D normal_in_point = InterpolateValue<QVector3D>({normals[0], normals[1], normals[2]}, barycentric_coords);
                    int intesity = calculateIntensity(Point(x, y), normal_in_point);

                    SetColor(IntensityColor(color, intesity));
                    painter_->drawPoint(x, y);
                    ResetColor();
                }
            }
        }
    }
}

std::tuple<int, int, int, int> DrawVisitor::CalculateBoundingBox(const std::array<Point, 3> &pts) const {
    int bbox_min_x = std::min({pts[0].x(), pts[1].x(), pts[2].x()});
    int bbox_min_y = std::min({pts[0].y(), pts[1].y(), pts[2].y()});
    int bbox_max_x = std::max({pts[0].x(), pts[1].x(), pts[2].x()});
    int bbox_max_y = std::max({pts[0].y(), pts[1].y(), pts[2].y()});

    bbox_min_x = std::max(bbox_min_x, 0);
    bbox_min_y = std::max(bbox_min_y, 0);
    bbox_max_x = std::min(bbox_max_x, static_cast<int>(z_buffer_.size()) - 1);
    bbox_max_y = std::min(bbox_max_y, static_cast<int>(z_buffer_[0].size()) - 1);
    return {bbox_min_x, bbox_min_y, bbox_max_x, bbox_max_y};
}

bool DrawVisitor::CalculateBarycentricCoords(const std::array<Point, 3> &pts, int x, int y, BarycentricCoords &barycentric_coords) const {
    double denom = (pts[1].y() - pts[2].y()) * (pts[0].x() - pts[2].x()) + (pts[2].x() - pts[1].x()) * (pts[0].y() - pts[2].y());
    if (std::abs(denom) < 1e-6) return false;

    barycentric_coords.alpha = ((pts[1].y() - pts[2].y()) * (x - pts[2].x()) + (pts[2].x() - pts[1].x()) * (y - pts[2].y())) / denom;
    barycentric_coords.beta = ((pts[2].y() - pts[0].y()) * (x - pts[2].x()) + (pts[0].x() - pts[2].x()) * (y - pts[2].y())) / denom;
    barycentric_coords.gamma = 1.0 - barycentric_coords.alpha - barycentric_coords.beta;
    return true;
}

bool DrawVisitor::IsInsideTriangle(const BarycentricCoords &barycentric_coords) const {
    return (barycentric_coords.alpha > 0 && barycentric_coords.beta > 0 && barycentric_coords.gamma > 0);
}

bool DrawVisitor::UpdateZBuffer(int x, int y, double z) {
    if (z < z_buffer_[x][y]) {
        z_buffer_[x][y] = z;
        return true;
    }
    return false;
}

int DrawVisitor::calculateIntensity(const Point &point, const QVector3D &normal) const {
    double intensity = 0;
    for (const auto &light: light_) {
        intensity += light->CalculateIntensityInPoint(point, normal);
    }
    return intensity * 255;
}


void DrawVisitor::Visit(const CarcasModel &carcas_model) {
    for (const auto &triangle : carcas_model.GetTriangles()) {
        std::array<Point, 3> points = carcas_model.GetTrianglePoints(triangle);
        std::array<QVector3D, 3> normals = carcas_model.GetNormals(triangle);
        Visit(Triangle(points, normals, carcas_model.GetColor()));
    }
}

Point DrawVisitor::ProjectPoint(const Point &point) const {
    return projection_->ProjectPoint(point, canvas_size_);
}


DrawMappedVisitor::DrawMappedVisitor(QPainter *painter,
                                     QSize canvas_size,
                                     const AbstractStrategyProjection *projection,
                                     QVector<std::shared_ptr<AbstractLight>> light,
                                     const QImage &normal_map)
    : DrawVisitor(painter, canvas_size, projection, light)
    , normal_map_(normal_map) {}


void DrawMappedVisitor::Visit(const CarcasModel &carcas_model) {
    for (const auto &triangle : carcas_model.GetTriangles()) {
        std::array<Point, 3> points = carcas_model.GetTrianglePoints(triangle);
        for (auto &point : points) {
            point = ProjectPoint(point);
        }
        std::array<QVector3D, 3> normals = carcas_model.GetNormals(triangle);
        std::array<QVector2D, 3> uv_coords = carcas_model.GetTriangleUV(triangle);
        DrawMappedTriangle(points, normals, uv_coords, carcas_model.GetColor());
    }

}

void DrawMappedVisitor::DrawMappedTriangle(const std::array<Point, 3> &pts, const std::array<QVector3D, 3> &normals, const std::array<QVector2D, 3> &uv_coords, const QColor &color) {
    auto [bbox_min_x, bbox_min_y, bbox_max_x, bbox_max_y] = CalculateBoundingBox(pts);

    for (int x = bbox_min_x; x <= bbox_max_x; ++x) {
        for (int y = bbox_min_y; y <= bbox_max_y; ++y) {
            BarycentricCoords barycentric_coords;

            if (CalculateBarycentricCoords(pts, x, y, barycentric_coords) && IsInsideTriangle(barycentric_coords)) {

                double z = InterpolateValue<double>({pts[0].z(), pts[1].z(), pts[2].z()}, barycentric_coords);
                if (UpdateZBuffer(x, y, z)) {
                    QVector3D normal_in_point = InterpolateValue<QVector3D>({normals[0], normals[1], normals[2]}, barycentric_coords);
                    QVector2D uv_point = InterpolateValue<QVector2D>({uv_coords[0], uv_coords[1], uv_coords[2]}, barycentric_coords);
                    qDebug() << uv_point;
                    normal_in_point += GetNormalInPoint(uv_point);
                    int intesity = calculateIntensity(Point(x, y), normal_in_point);

                    SetColor(IntensityColor(color, intesity));
                    painter_->drawPoint(x, y);
                    ResetColor();
                }
            }
        }
    }
}

QVector3D DrawMappedVisitor::GetNormalInPoint(const QVector2D &uv) {
    int x = static_cast<int>(uv.x() * normal_map_.width()) % normal_map_.width();
    int y = static_cast<int>(uv.y() * normal_map_.height()) % normal_map_.height();

    QColor color = normal_map_.pixelColor(x, y);
    QVector3D normal;
    normal.setX((color.red() / 255.0) * 2.0 - 1.0);
    normal.setY((color.green() / 255.0) * 2.0 - 1.0);
    normal.setZ(-((color.blue() / 255.0) * 2.0 - 1.0));

    return normal.normalized();
}

