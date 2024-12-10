#include "curve.h"


Curve::Curve(const QVector<QPointF> &main_points, int steps) {
    for (int step = 0; step <= steps; ++step) {
        double t = static_cast<double>(step) / steps;
        points_.append(deCasteljau(main_points, t));
    }
}

QPointF Curve::deCasteljau(const QVector<QPointF> &main_points, double t) {
    QVector<QPointF> points = main_points;
    while (points.size() > 1) {
        QVector<QPointF> nextLevel;
        for (int i = 0; i < points.size() - 1; ++i) {
            nextLevel.append((1 - t) * points[i] + t * points[i + 1]);
        }
        points = nextLevel;
    }
    return points[0];
}

QVector<QPointF> Curve::GetPoints() const noexcept {
    return points_;
}
