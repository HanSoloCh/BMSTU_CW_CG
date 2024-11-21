#ifndef POINT_H
#define POINT_H

#include <QColor>
#include <QVector3D>
#include <QPointF>

#include "abstractmodel.h"

class Point: public AbstractModel, public QVector3D {
public:
    Point(double x, double y, double z = 0, const QColor &color = Qt::white, double h = 1.0);
    ~Point() = default;

    double h() const {
        return h_;
    }

    operator QPointF() {
        return QPointF(x(), y());
    }

    void Accept(BaseDrawVisitor &visitor) const override;

    bool operator==(const Point &p);

private:
    double h_;
};

#endif // POINT_H
