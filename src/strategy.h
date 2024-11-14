#ifndef STRATEGY_H
#define STRATEGY_H

#include <QPointF>
#include <QSize>

class Point;

class ProjectionStrategy
{
public:
    ProjectionStrategy() = default;
    virtual ~ProjectionStrategy() = default;
    virtual QPointF project(const Point &point, const QSize &canvasSize) const = 0;
};

class DefaultProjectionStrategy : public ProjectionStrategy
{
public:
    DefaultProjectionStrategy() = default;
    ~DefaultProjectionStrategy() = default;
    QPointF project(const Point &point, const QSize &canvasSize) const override;
};

#endif // STRATEGY_H
