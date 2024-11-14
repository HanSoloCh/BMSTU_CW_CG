#ifndef LINE_H
#define LINE_H

#include "abstractobject.h"

#include "point.h"

class Line : public AbstractObject
{
public:
    explicit Line(QPair<Point, Point> curLine, QColor curColor = Qt::white);
    void draw(QPainter &painter, const ProjectionStrategy &strategy, QSize canvasSize) override;
    void rotate(double angleX, double angleY, double angleZ) override;
private:
    QPair<Point, Point> line;
    QColor color;
};

#endif // LINE_H
