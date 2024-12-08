#ifndef CURVE_H
#define CURVE_H

#include <QVector>
#include <QPointF>

class Curve {
public:
    Curve(const QVector<QPointF> &mainPoints, int steps);
    ~Curve() = default;

    QVector<QPointF> calculateBezierCurve();

private:
    QPointF deCasteljau(double t);

    QVector<QPointF> mainPoints;
    int steps;
};


#endif // CURVE_H
