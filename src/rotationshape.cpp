#include "rotationshape.h"

#include "point.h"


RotationShape::RotationShape(const Line &forming, QColor color, bool isConst)
    : AbstractObject(color, isConst)
{
    int segments = 50;
    int rotationSteps = 10;
    int rotationAngle = 360;
    QVector<Point> segmentPoints;
    for (int i = 0; i <= segments; ++i) {
        double t = static_cast<double>(i) / segments;
        Point point = {
            forming.start().getX() * (1 - t) + forming.end().getX() * t,
            forming.start().getY() * (1 - t) + forming.end().getY() * t,
            forming.start().getZ() * (1 - t) + forming.end().getZ() * t,
        };
        segmentPoints.push_back(point);
    }

    // Вращаем каждую точку вокруг оси вращения
    for (int step = 0; step < rotationSteps; ++step) {
        double angle = step * rotationAngle;
        std::vector<Point> ring;

        for (const auto& point : segmentPoints) {
            Point rotatedPoint = {
                point.getX() * cos(angle) - point.getY() * sin(angle), // Поворачиваем вокруг оси
                point.getX() * sin(angle) + point.getY() * cos(angle),
                point.getZ()
            };
            ring.push_back(rotatedPoint);
        }
        surface.push_back(ring); // Добавляем кольцо
    }
}

void RotationShape::draw(QPainter &painter, const ProjectionStrategy &strategy, QSize canvasSize) const
{
    for (auto &level : surface) {
        for (auto &point : level) {
            point.draw(painter, strategy, canvasSize);
        }
    }
}

void RotationShape::move(double xMove, double yMove, double zMove)
{

}

void RotationShape::rotate(double xAngle, double yAngle, double zAngle)
{

}

void RotationShape::scale(double xScale, double yScale, double zScale)
{

}
