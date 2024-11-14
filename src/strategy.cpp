#include "strategy.h"

#include "point.h"

QPointF DefaultProjectionStrategy::project(const Point &point, const QSize &canvasSize) const
{
    // float distance = 200.0f;
    // float factor = distance / (distance + z);
    // float projectedX = x * factor + canvasSize.width() / 2.0f;
    // float projectedY = -y * factor + canvasSize.height() / 2.0f;
    return QPointF(point.getX() + canvasSize.width() / 2.0, -point.getY() + canvasSize.height() / 2.0);
}
