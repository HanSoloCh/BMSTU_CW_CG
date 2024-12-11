#ifndef POINT_H
#define POINT_H

#include <QColor>
#include <QVector3D>
#include <QPointF>

#include "abstractmodel.h"

class Point: public AbstractModel, public QVector3D {
public:
    explicit Point(double x = 0.0, double y = 0.0, double z = 0.0, const QColor &color = Qt::white);
    explicit Point(QPoint p, double z = 0.0, const QColor &color = Qt::white);
    explicit Point(QPointF p, double z = 0.0, const QColor &color = Qt::white);
    explicit Point(const QVector3D &vector, const QColor &color = Qt::white);

    ~Point() = default;


    operator QPointF() {
        return QPointF(x(), y());
    }

    void Accept(BaseDrawVisitor *visitor) const override;

    void Transform(const QMatrix4x4 &transform_matrix) override;

    bool operator==(const Point &p) const noexcept;
};

#endif // POINT_H
