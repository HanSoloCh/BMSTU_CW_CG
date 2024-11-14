#ifndef ABSTRACTOBJECT_H
#define ABSTRACTOBJECT_H

#include <QObject>
#include <QPainter>

#include "strategy.h"

class AbstractObject
{
public:
    explicit AbstractObject() = default;
    virtual ~AbstractObject() = 0;

    virtual void draw(QPainter &painter, const ProjectionStrategy &strategy, QSize canvasSize) = 0;
    virtual void rotate(double angleX, double angleY, double angleZ) = 0;
};

#endif // ABSTRACTOBJECT_H
