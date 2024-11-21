#include "drawvisitor.h"


QVector<double> DrawVisitor::Interpolate(double x0, double y0, double x1, double y1) {
    if (x0 == x1) {
        return {y0};
    }
    QVector<double > values;
    double k = (y1 - y0) / (x1- x0);
    double y = y0;
    for (int x = x0; x <= x1; ++x) {
        values.push_back(y);
        y += k;
    }
    return values;
}

BaseDrawVisitor::~BaseDrawVisitor() {}

BaseDrawVisitor::BaseDrawVisitor(QPainter *painter)
    : painter_(painter)
    , color_(painter->pen().color()) {}

DrawVisitor::DrawVisitor(QPainter *painter, QSize canvas_size, const AbstractStrategyProjection *projection)
    : BaseDrawVisitor(painter)
    , canvas_size_(canvas_size)
    , z_buffer_(canvas_size_.height(), QVector<double>(canvas_size_.width(), 0.0))
    , projection_(projection) {}


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


QColor IntensityColor(const QColor &color, double intensity) {
    if (intensity < 0) intensity = 0;
    if (intensity > 255) intensity = 255;

    QColor newColor(
        (color.red() * intensity) / 255,
        (color.green() * intensity) / 255,
        (color.blue() * intensity) / 255
        );
    return newColor;
}


int calcInd(const Point &p0, const Point &p1, const Point &p2) {
    QVector3D light(0, 0, -1);
    QVector3D n = QVector3D::normal(p2 - p0, p1 - p0);
    double ind = QVector3D::dotProduct(n, light);
    return ind * 255;
}

void DrawVisitor::Visit(const Triangle &triangle) {
    // Вот тут вообще писец происходит
    QVector<Point> points = triangle.GetPoints();

    int ind = calcInd(points[0], points[1], points[2]);
    if (std::isnan(ind) || ind <= 0) {
        return;
    }

    for (auto &point : points) {
        point = projection_->ProjectPoint(point, {canvas_size_.width(), canvas_size_.width()});
    }

    Triangle tmp_triangle(points);

    // // Сортировка по y
    std::sort(points.begin(), points.end(), [](const Point &p1, const Point &p2) {
        return p1.y() < p2.y();
    });

    SetColor(IntensityColor(triangle.GetColor(), ind));
    DrawTriangle(tmp_triangle);
    ResetColor();
}

void DrawVisitor::DrawTriangle(Triangle &triangle) {
    // bbox_min_x = std::max(bbox_min_x, 0);
    // bbox_min_y = std::max(bbox_min_y, 0);
    // bbox_max_x = std::min(bbox_max_x, static_cast<int>(z_buffer_.size()) - 1);
    // bbox_max_y = std::min(bbox_max_y, static_cast<int>(z_buffer_[0].size()) - 1);

    for (int x = triangle.GetMinX(); x <= static_cast<int>(triangle.GetMaxX()); ++x) {
        for (int y = triangle.GetMinY(); y <= static_cast<int>(triangle.GetMaxY()); ++y) {

            if (triangle.IsContains(Point(x, y))) {

                // Вот в этой части два раза одно и то же считается. Мб как-то поправить
                QVector<Point> points = triangle.GetPoints();
                // Интерполируем z-координату
                double z = triangle.InterpolateValue(points[0].z(), points[1].z(), points[2].z(), Point(x, y));
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


void DrawVisitor::Visit(const CarcasModel &carcas_model) {
    for (const auto &triangles : carcas_model.GetTriangles()) {
        triangles.Accept(*this);
    }
}

