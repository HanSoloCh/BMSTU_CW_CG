#include "strategy.h"

#include "point.h"

AbstractStrategyProjection::~AbstractStrategyProjection() {}

PerspectivStrategyProjection::PerspectivStrategyProjection(int viewport_width, int viewport_height, int distance)
    : kViewportWidth(viewport_width)
    , kViewportHeight(viewport_height)
    , kDistance(distance) {}

Point PerspectivStrategyProjection::ProjectPoint(const Point &point, const QSize &canvas_size) const {
    double x = point.x();
    double y = point.y();
    double z = point.z();
    QPointF project_point = ViewportToCanvas(x * kDistance / z, y * kDistance / z, canvas_size);
    return Point(project_point.x(), project_point.y(), z, point.GetColor());
}


QPointF PerspectivStrategyProjection::ViewportToCanvas(double x, double y, const QSize &canvas_size) const {
    return QPointF(x * canvas_size.width() / kViewportWidth + canvas_size.width() / 2,
                   -y * canvas_size.height() / kViewportHeight + canvas_size.height() / 2);
}



Point NewStrategy::ProjectPoint(const Point &point, const QSize &canvas_size) const
{
    return Point((point.x() / (1 - point.z() / kDistance) + canvas_size.width() / 2),
                 (-point.y() / (1 - point.z() / kDistance)) + canvas_size.height() / 2,
                 point.z());
}
