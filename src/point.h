#ifndef POINT_H
#define POINT_H

#include <QColor>
#include <QVector4D>
#include <QPointF>

#include "abstractmodel.h"

class Point: public AbstractModel, public QVector4D {
public:
    Point(double x = 0.0, double y = 0.0, double z = 0.0, const QColor &color = Qt::white);
    Point(QPoint p, double z, const QColor &color = Qt::white);
    Point(QPointF p, double z, const QColor &color = Qt::white);

    ~Point() = default;


    operator QPointF() {
        return QPointF(x(), y());
    }

    void Accept(BaseDrawVisitor &visitor) const override;

    // bool operator<(const Point &p);
    bool operator==(const Point &p) const noexcept;
};

#endif // POINT_H
