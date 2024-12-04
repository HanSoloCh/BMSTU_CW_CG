#include "drawvisitor.h"

#include <QVector4D>

#include <QDebug>


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
    , z_buffer_(canvas_size_.height(), QVector<double>(canvas_size_.width(), 0.0))
    , projection_(projection)
    , light_(light) {}


void DrawVisitor::Visit(const Point &point) {
    Point project_point = projection_->ProjectPoint(point, canvas_size_);

    SetColor(point.GetColor());
    DrawPoint(project_point);
    ResetColor();
}

void DrawVisitor::DrawPoint(Point &point) {
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


int calcInd(const Triangle &triangle) {
    QVector3D light(0, 0, -1);
    double ind = QVector3D::dotProduct(triangle.CalculateNormal(), light);
    return ind * 255;
}

void DrawVisitor::Visit(const Triangle &triangle) {
    int ind = calcInd(triangle);

    std::array<Point, 3> points = triangle.GetPoints();

    for (auto &point : points) {
        point = projection_->ProjectPoint(point, {canvas_size_.width(), canvas_size_.width()});
    }

    // Сортировка по y
    std::sort(points.begin(), points.end(), [](const Point &p1, const Point &p2) {
        return p1.y() < p2.y();
    });

    SetColor(IntensityColor(triangle.GetColor(), ind));
    DrawTriangle({points[0], points[1], points[2]});
    ResetColor();
}



void DrawVisitor::DrawTriangle(const std::array<Point, 3> &pts) {

    // std::array<Point, 3> pts{p1, p2, p3};
    // Определение границ треугольника
    int bbox_min_x = std::min({pts[0].x(), pts[1].x(), pts[2].x()});
    int bbox_min_y = std::min({pts[0].y(), pts[1].y(), pts[2].y()});
    int bbox_max_x = std::max({pts[0].x(), pts[1].x(), pts[2].x()});
    int bbox_max_y = std::max({pts[0].y(), pts[1].y(), pts[2].y()});

    bbox_min_x = std::max(bbox_min_x, 0);
    bbox_min_y = std::max(bbox_min_y, 0);
    bbox_max_x = std::min(bbox_max_x, static_cast<int>(z_buffer_.size()) - 1);
    bbox_max_y = std::min(bbox_max_y, static_cast<int>(z_buffer_[0].size()) - 1);

    for (int x = bbox_min_x; x <= bbox_max_x; ++x) {
        for (int y = bbox_min_y; y <= bbox_max_y; ++y) {
            // Вычисляем барицентрические координаты
            double denom = (pts[1].y() - pts[2].y()) * (pts[0].x() - pts[2].x()) + (pts[2].x() - pts[1].x()) * (pts[0].y() - pts[2].y());
            double alpha = ((pts[1].y() - pts[2].y()) * (x - pts[2].x()) + (pts[2].x() - pts[1].x()) * (y - pts[2].y())) / denom;
            double beta = ((pts[2].y() - pts[0].y()) * (x - pts[2].x()) + (pts[0].x() - pts[2].x()) * (y - pts[2].y())) / denom;
            double gamma = 1.0 - alpha - beta;

            // Проверяем, находится ли точка внутри треугольника
            if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                // Интерполируем z-координату
                double z = alpha * pts[0].z() + beta * pts[1].z() + gamma * pts[2].z();
                z = 1 / z;
                // Проверяем и обновляем z-буфер
                if (z > z_buffer_[x][y]) {
                    z_buffer_[x][y] = z;
                    painter_->drawPoint(x, y);
                }
            }
        }
    }
}

// Происходит издевательство над паттернами и тасовым
void DrawVisitor::Visit(const Triangle &triangle, const std::array<QVector3D, 3> &normals) {
    std::array<Point, 3> points = triangle.GetPoints();

    for (auto &point : points) {
        point = projection_->ProjectPoint(point, {canvas_size_.width(), canvas_size_.width()});
    }

    // Сортировка по y
    std::sort(points.begin(), points.end(), [](const Point &p1, const Point &p2) {
        return p1.y() < p2.y();
    });

    // SetColor(triangle.GetColor());
    DrawTriangle({points[0], points[1], points[2]},
                 normals,
                 triangle.GetColor());
    // ResetColor();
}


void DrawVisitor::DrawTriangle(const std::array<Point, 3> &pts, const std::array<QVector3D, 3> &normals, const QColor &color) {
    int bbox_min_x = std::min({pts[0].x(), pts[1].x(), pts[2].x()});
    int bbox_min_y = std::min({pts[0].y(), pts[1].y(), pts[2].y()});
    int bbox_max_x = std::max({pts[0].x(), pts[1].x(), pts[2].x()});
    int bbox_max_y = std::max({pts[0].y(), pts[1].y(), pts[2].y()});

    bbox_min_x = std::max(bbox_min_x, 0);
    bbox_min_y = std::max(bbox_min_y, 0);
    bbox_max_x = std::min(bbox_max_x, static_cast<int>(z_buffer_.size()) - 1);
    bbox_max_y = std::min(bbox_max_y, static_cast<int>(z_buffer_[0].size()) - 1);

    for (int x = bbox_min_x; x <= bbox_max_x; ++x) {
        for (int y = bbox_min_y; y <= bbox_max_y; ++y) {
            // Вычисляем барицентрические координаты
            double denom = (pts[1].y() - pts[2].y()) * (pts[0].x() - pts[2].x()) + (pts[2].x() - pts[1].x()) * (pts[0].y() - pts[2].y());
            double alpha = ((pts[1].y() - pts[2].y()) * (x - pts[2].x()) + (pts[2].x() - pts[1].x()) * (y - pts[2].y())) / denom;
            double beta = ((pts[2].y() - pts[0].y()) * (x - pts[2].x()) + (pts[0].x() - pts[2].x()) * (y - pts[2].y())) / denom;
            double gamma = 1.0 - alpha - beta;

            // Проверяем, находится ли точка внутри треугольника
            if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                // Интерполируем z-координату
                double z = alpha * pts[0].z() + beta * pts[1].z() + gamma * pts[2].z();
                z = 1 / z;
                // Проверяем и обновляем z-буфер
                if (z > z_buffer_[x][y]) {
                    z_buffer_[x][y] = z;

                    QVector3D normal_in_point = alpha * normals[0] + beta * normals[1] + gamma * normals[2];
                    int intesity = calculateIntensity(Point(x, y), normal_in_point);
                    SetColor(IntensityColor(color, intesity));
                    painter_->drawPoint(x, y);
                    ResetColor();
                }
            }
        }
    }
}

int DrawVisitor::calculateIntensity(const Point &point, const QVector3D &normal) const {
    double intesity = 0;
    for (const auto &light: light_) {
        intesity += light->CalculateIntensityInPoint(point, normal);
    }
    return intesity * 255;
}

// #include "triangle.h"

void DrawVisitor::Visit(const CarcasModel &carcas_model) {
    int i = 0;
    for (const auto &triangle : carcas_model.GetTriangles()) {
        std::array<Point, 3> points = carcas_model.GetTrianglePoints(triangle);
        Visit(Triangle(points, carcas_model.colors_[i]), carcas_model.GetNormals(triangle));
        ++i;
    }
}

