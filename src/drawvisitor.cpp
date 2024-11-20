#include "drawvisitor.h"

BaseDrawVisitor::~BaseDrawVisitor() {}

BaseDrawVisitor::BaseDrawVisitor(QPainter *painter)
    : painter_(painter) {}

DrawVisitor::DrawVisitor(QPainter *painter, QSize canvas_size, const AbstractStrategyProjection *projection)
    : BaseDrawVisitor(painter)
    , canvas_size_(canvas_size)
    , z_buffer_(canvas_size_.height(), QVector<double>(canvas_size_.width(), 0.0))
    , projection_(projection) {
}


void DrawVisitor::Visit(const Point &point) {
    Point project_point = projection_->ProjectPoint(point, canvas_size_);
    DrawPoint(project_point);
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
    QVector<Point> points = triangle.GetPoints();

    int ind = calcInd(points[0], points[1], points[2]);
    if (std::isnan(ind) || ind < 0) {
        return;
    }

    for (auto &point : points) {
        point = projection_->ProjectPoint(point, canvas_size_);
    }

    // // Сортировка по y
    std::sort(points.begin(), points.end(), [](const Point &p1, const Point &p2) {
        return p1.y() < p2.y();
    });

    QColor color = painter_->pen().color();
    painter_->setPen(IntensityColor(triangle.GetColor(), ind));
    DrawTriangle(points[0], points[1], points[2]);
    painter_->setPen(color);
}

void DrawVisitor::Visit(const CarcasModel &carcas_model) {
    for (const auto &triangles : carcas_model.GetTriangles()) {
        triangles.Accept(*this);
    }
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

void DrawVisitor::DrawTriangle(const Point &p0, const Point &p1, const Point &p2) {
    QVector<double> x01 = Interpolate(p0.y(), p0.x(), p1.y(), p1.x());
    // QVector<double> h01 = Interpolate(p0.GetY(), p0.GetH(), p1.GetY(), p1.GetH());
    QVector<double> z01 = Interpolate(p0.y(), p0.z(), p1.y(), p1.z());

    QVector<double> x12 = Interpolate(p1.y(), p1.x(), p2.y(), p2.x());
    // QVector<double> h12 = Interpolate(p1.GetY(), p1.GetH(), p2.GetY(), p2.GetH());
    QVector<double> z12 = Interpolate(p1.y(), p1.z(), p2.y(), p2.z());

    QVector<double> x02 = Interpolate(p0.y(), p0.x(), p2.y(), p2.x());
    // QVector<double> h02 = Interpolate(p0.GetY(), p0.GetH(), p2.GetY(), p2.GetH());
    QVector<double> z02 = Interpolate(p0.y(), p0.z(), p2.y(), p2.z());

    x01.removeLast();
    // h01.removeLast();
    z01.removeLast();


    QVector<double> x012 = x01 + x12;
    // QVector<double> h012 = h01 + h12;
    QVector<double> z012 = z01 + z12;


    int mid = x012.length() / 2;

    QVector<double> xLeft = x012;
    QVector<double> xRight = x02;

    // QVector<double> hLeft = h012;
    // QVector<double> hRight = h02;

    QVector<double> zLeft = z012;
    QVector<double> zRight = z02;

    if (x02[mid] < x012[mid]) {
        std::swap(xLeft, xRight);
        std::swap(zLeft, zRight);
        // std::swap(hLeft, hRight);
    }

    for (int y = p0.y(); y <= p2.y(); ++y) {
        int curXLeft = xLeft[y - p0.y()];
        int curXRight = xRight[y - p0.y()];

        // QVector<double> hSegment = Interpolate(curXLeft, hLeft[y - p0.GetY()], curXRight, hRight[y - p0.GetY()]);
        QVector<double> zSegment = Interpolate(curXLeft, zLeft[y - p0.y()], curXRight, zRight[y - p0.y()]);

        for (int x = curXLeft; x <= curXRight; ++x) {
            double z = 1 / zSegment[x - curXLeft];
            // painter.setPen(IntensityColor(color, hSegment[x - curXLeft]));
            if (z > z_buffer_[x][y]) {
                painter_->drawPoint(x, y);
                z_buffer_[x][y] = z;
            }
        }
    }
}


// // TODO Дублирование кода
// void Triangle::DrawLine(QPointF p0, QPointF p1, QPainter &painter) {
//     if (abs(p1.x() - p0.x()) > abs(p1.y() - p0.y())) {
//         if (p0.x() > p1.x())
//             std::swap(p0, p1);

//         QVector<double> yValues = Interpolate(p0.x(), p0.y(), p1.x(), p1.y());
//         for (int x = p0.x(); x <= p1.x(); ++x) {
//             painter.drawPoint(x, yValues[x - p0.x()]);
//         }
//     } else {
//         if (p0.y() > p1.y())
//             std::swap(p0, p1);
//         QVector<double> xValues = Interpolate(p0.y(), p0.x(), p1.y(), p1.x());

//         for (int y = p0.y(); y <= p1.y(); ++y) {
//             painter.drawPoint(xValues[y - p0.y()], y);
//         }
//     }
// }

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
