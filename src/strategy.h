#ifndef STRATEGY_H
#define STRATEGY_H

#include <QPointF>
#include <QSize>

class Point;

class AbstractStrategyProjection {
public:
    AbstractStrategyProjection() = default;
    virtual ~AbstractStrategyProjection() = 0;

    virtual Point ProjectPoint(const Point &point, const QSize &canvas_size) const = 0;
};

class PerspectivStrategyProjection: public AbstractStrategyProjection
{
public:
    explicit PerspectivStrategyProjection(int viewport_width = 1,
                                           int viewport_height = 1,
                                           int distance = 1);

    Point ProjectPoint(const Point &point, const QSize &canvas_size) const override;

protected:
    QPointF ViewportToCanvas(double x, double y, const QSize &canvas_size) const;

private:
    const int kViewportWidth;
    const int kViewportHeight;
    const int kDistance;
};


#endif // STRATEGY_H
