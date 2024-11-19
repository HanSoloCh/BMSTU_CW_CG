#include "point.h"

#include "drawvisitor.h"

Point::Point(double x, double y, double z, const QColor &color, double h)
    : AbstractModel(color)
    , QVector3D(x, y, z)
    , h_(h)
{}

void Point::Accept(BaseDrawVisitor &visitor) const {
    visitor.Visit(*this);
}



