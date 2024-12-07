#ifndef LINE_H
#define LINE_H

#include "abstractobject.h"

#include "point.h"

class Line : public AbstractObject
{
public:
    explicit Line(QPair<Point, Point> curLine, QColor curColor = Qt::white, bool isConst = false);
    ~Line() = default;

    Point start() const;
    Point end() const;

    void draw(QPainter &painter, const ProjectionStrategy &strategy, QSize canvasSize) const override;

    void move(double xMove, double yMove, double zMove) override;
    void rotate(double xAngle, double yAngle, double zAngle) override;
    void scale(double xScale, double yScale, double zScale) override;

private:
    QPair<Point, Point> line;
};

#endif // LINE_H
