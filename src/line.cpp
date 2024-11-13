#include "line.h"


Line::Line(QPair<QPointF, QPointF> line, QColor color, QObject *parent):
    AbstractObject(parent),
    _line(line),
    _color(color)
{}

void Line::draw(QPainter &painter)
{
    painter.setPen(QPen(_color, 2));
    painter.drawLine(_line.first, _line.second);
}
