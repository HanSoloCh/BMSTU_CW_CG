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
    Point project_point = ProjectPoint(point);

    SetColor(point.GetColor());
    DrawPoint(project_point);
    ResetColor();
}

void DrawVisitor::DrawPoint(const Point &point) {
    double x = point.x();
    double y = point.y();
    double z = point.z();
    if (z < z_buffer_[x][y]) {
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
    Triangle tmp_triangle(triangle);
    tmp_triangle.SetPoints(points);
    DrawTriangle(tmp_triangle);
}

void DrawVisitor::DrawTriangle(const Triangle &triangle) {
    auto [bbox_min_x, bbox_min_y, bbox_max_x, bbox_max_y] = CalculateBoundingBox(triangle.GetPoints());
    for (int x = bbox_min_x; x <= bbox_max_x; ++x) {
        for (int y = bbox_min_y; y <= bbox_max_y; ++y) {
            BarycentricCoords barycentric_coords;
            if (CalculateBarycentricCoords(triangle.GetPoints(), x, y, barycentric_coords) && IsInsideTriangle(barycentric_coords)) {
                double z = InterpolateValue<double>({triangle[0].z(), triangle[1].z(), triangle[2].z()}, barycentric_coords);
                if (UpdateZBuffer(x, y, z)) {
                    DrawTrianglePixel({x, y}, triangle, barycentric_coords);
                }
            }
        }
    }
}

void DrawVisitor::DrawTrianglePixel(const QPoint &point,
                                    const Triangle &triangle,
                                    const BarycentricCoords &barycentric_coords) {
    std::array<QVector3D, 3> normals = triangle.GetNormals();
    QVector3D normal_in_point = InterpolateValue<QVector3D>({normals[0], normals[1], normals[2]}, barycentric_coords);
    int intesity = CalculateIntensity(point, normal_in_point);

    SetColor(IntensityColor(triangle.GetColor(), intesity));
    painter_->drawPoint(point);
    ResetColor();
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
    if (std::abs(denom) < 1e-9) return false;

    barycentric_coords.alpha = ((pts[1].y() - pts[2].y()) * (x - pts[2].x()) + (pts[2].x() - pts[1].x()) * (y - pts[2].y())) / denom;
    barycentric_coords.beta = ((pts[2].y() - pts[0].y()) * (x - pts[2].x()) + (pts[0].x() - pts[2].x()) * (y - pts[2].y())) / denom;
    barycentric_coords.gamma = 1.0 - barycentric_coords.alpha - barycentric_coords.beta;
    return true;
}

bool DrawVisitor::IsInsideTriangle(const BarycentricCoords &barycentric_coords) const {
    return (barycentric_coords.alpha >= 0 && barycentric_coords.beta >= 0 && barycentric_coords.gamma >= 0);
}

bool DrawVisitor::UpdateZBuffer(int x, int y, double z) {
    if (z < z_buffer_[x][y]) {
        z_buffer_[x][y] = z;
        return true;
    }
    return false;
}

int DrawVisitor::CalculateIntensity(const Point &point, const QVector3D &normal) const {
    double intensity = 0;
    for (const auto &light: light_) {
        intensity += light->CalculateIntensityInPoint(point, normal);
    }
    return intensity * 255;
}


void DrawVisitor::Visit(const CarcasModel &carcas_model) {
    for (const auto &triangle : carcas_model.GetTriangles()) {
        Visit(Triangle(carcas_model.GetTrianglePoints(triangle),
                       carcas_model.GetTriangleNormals(triangle),
                       carcas_model.GetColor()));
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

DrawMappedVisitor::DrawMappedVisitor(const DrawVisitor &draw_visitor, const QImage &normal_map)
    : DrawVisitor(draw_visitor)
    , normal_map_(normal_map) {}


void DrawMappedVisitor::Visit(const Point &point) {
    DrawVisitor::Visit(point);
}

void DrawMappedVisitor::Visit(const Triangle &triangle) {
    DrawVisitor::Visit(triangle);
}

void DrawMappedVisitor::Visit(const CarcasModel &carcas_model) {
    for (const auto &triangle : carcas_model.GetTriangles()) {
        Triangle tmp_triangle(carcas_model.GetTrianglePoints(triangle),
                              carcas_model.GetTriangleNormals(triangle),
                              carcas_model.GetColor());
        tmp_triangle.SetUVCoords(carcas_model.GetTriangleUV(triangle));
        Visit(tmp_triangle);
    }
}

void DrawMappedVisitor::DrawTrianglePixel(const QPoint &point,
                                          const Triangle &triangle,
                                          const BarycentricCoords &barycentric_coords) {
    std::array<QVector3D, 3> normals = triangle.GetNormals();
    QVector3D normal_in_point = InterpolateValue<QVector3D>({normals[0], normals[1], normals[2]}, barycentric_coords);

    std::array<QVector2D, 3> uv_in_point = triangle.GetUVCoords();
    QVector2D uv_point = InterpolateValue<QVector2D>({uv_in_point[0], uv_in_point[1], uv_in_point[2]}, barycentric_coords);
    normal_in_point += GetNormalInPoint(uv_point);

    int intesity = CalculateIntensity(point, normal_in_point.normalized());

    SetColor(IntensityColor(triangle.GetColor(), intesity));
    painter_->drawPoint(point);
    ResetColor();
}


QVector3D DrawMappedVisitor::GetNormalInPoint(const QVector2D &uv) const {
    int x = static_cast<int>(uv.x() * normal_map_.width()) % normal_map_.width();
    int y = static_cast<int>(uv.y() * normal_map_.height()) % normal_map_.height();

    QColor color = normal_map_.pixelColor(x, y);
    QVector3D normal;
    normal.setX((color.red() / 255.0) * 2.0 - 1.0);
    normal.setY((color.green() / 255.0) * 2.0 - 1.0);
    normal.setZ(-((color.blue() / 255.0) * 2.0 - 1.0));

    return normal.normalized();
}

