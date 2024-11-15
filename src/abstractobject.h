#ifndef ABSTRACTOBJECT_H
#define ABSTRACTOBJECT_H

#include <QObject>
#include <QPainter>

#include "strategy.h"

class AbstractObject
{
public:
    explicit AbstractObject(QColor curColor = Qt::white, bool isConst = false)
        : color(curColor)
        , constObjecct(isConst)
        {};
    virtual ~AbstractObject() = 0;

    bool isConst() const {
        return constObjecct;
    }

    virtual void draw(QPainter &painter, const ProjectionStrategy &strategy, QSize canvasSize) const = 0;

    virtual void move(double xMove, double yMove, double zMove) = 0;
    virtual void rotate(double xAngle, double yAngle, double zAngle) = 0;
    virtual void scale(double xScale, double yScale, double zScale) = 0;

protected:
    QColor color;

private:
    // Удалить
    bool constObjecct;
};

#endif // ABSTRACTOBJECT_H
