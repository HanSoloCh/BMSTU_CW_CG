#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QColor>

#include "point.h"
#include "abstractmodel.h"
#include "strategy.h"


class Triangle: public AbstractModel {
public:
    Triangle(const Point &p1, const Point &p2, const Point &p3, const QColor &color = Qt::white);
    explicit Triangle(const QVector<Point> &points, const QColor &color = Qt::white);

    ~Triangle() = default;

    QVector<Point> GetPoints() const {
        return points_;
    }

    void Accept(BaseDrawVisitor &visitor) const override;

private:
    QVector<Point> points_;
};

#endif // TRIANGLE_H
