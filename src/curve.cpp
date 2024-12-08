#include "curve.h"

Curve::Curve(const QVector <QPointF> &mainPoints, int steps)
    : mainPoints(mainPoints), steps(steps) {}


QVector<QPointF> Curve::calculateBezierCurve() {
    QVector<QPointF> curvePoints;
    for (int step = 0; step <= steps; ++step) {
        double t = static_cast<double>(step) / steps;
        curvePoints.append(deCasteljau(t));
    }
    return curvePoints;
}


QPointF Curve::deCasteljau(double t) {
    QVector<QPointF> points = mainPoints;
    while (points.size() > 1) {
        QVector<QPointF> nextLevel;
        for (int i = 0; i < points.size() - 1; ++i) {
            nextLevel.append((1 - t) * points[i] + t * points[i + 1]);
        }
        points = nextLevel;
    }
    return points[0];
}
