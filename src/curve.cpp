#include "curve.h"

Curve::Curve(const QVector<QPointF> &main_points,
             const QVector<QPointF> &control_points, int steps) {
    if (control_points.empty()) {
        QPointF start = main_points[0];
        QPointF end = main_points[1];
        for (int i = 0; i <= steps; ++i) {
            double t = static_cast<double>(i) / steps;
            points_.append((1 - t) * start + t * end);
        }
    } else {
        for (int step = 0; step <= steps; ++step) {
            double t = static_cast<double>(step) / steps;
            points_.append(deCasteljau(main_points, control_points, t));
        }
    }
}

QPointF Curve::deCasteljau(const QVector<QPointF> &main_points,
                           const QVector<QPointF> &control_points,
                           double t) const {
    QVector<QPointF> points = {main_points[0]};
    for (const auto &point : control_points) {
        points.append(point);
    }
    points.append(main_points[1]);
    while (points.size() > 1) {
        QVector<QPointF> next_level;
        for (int i = 0; i < points.size() - 1; ++i) {
            next_level.append((1 - t) * points[i] + t * points[i + 1]);
        }
        points = next_level;
    }
    return points[0];
}

QVector<QPointF> Curve::GetPoints() const noexcept { return points_; }
