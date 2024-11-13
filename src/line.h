#ifndef LINE_H
#define LINE_H

#include "abstractobject.h"

#include <QPointF>

class Line : public AbstractObject
{
public:
    explicit Line(QPair<QPointF, QPointF> line, QColor color, QObject *parent = nullptr);
    void draw(QPainter &painter) override;

private:
    QPair<QPointF, QPointF> _line;
    QColor _color;
};

#endif // LINE_H
