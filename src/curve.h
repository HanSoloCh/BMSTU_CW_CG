#ifndef CURVE_H
#define CURVE_H

#include <QVector>
#include <QPointF>

class Curve {
public:
    explicit Curve(const QVector<QPointF> &main_points,
                   const QVector<QPointF> &control_points = {},
                   int steps = 50);
    ~Curve() = default;

    QVector<QPointF> GetPoints() const noexcept;

private:
    QPointF deCasteljau(const QVector<QPointF> &main_points,
                        const QVector<QPointF> &control_points,
                        double t) const;
    void SortPoints(QVector<QPointF> &control_points, const QVector<QPointF> &main_points) const;

    QVector<QPointF> points_;
};


#endif // CURVE_H
